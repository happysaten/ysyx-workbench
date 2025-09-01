/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NSIM is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <SDL2/SDL.h>
#include <SDL2/SDL_mutex.h>
#include <common.h>
#include <device/map.h>

// 音频设备寄存器定义
enum {
    reg_freq,      // 音频频率寄存器
    reg_channels,  // 声道数寄存器
    reg_samples,   // 采样数寄存器
    reg_sbuf_size, // 流缓冲区大小寄存器（只读）
    reg_init,      // 初始化寄存器（写入触发SDL初始化）
    reg_count,     // 当前缓冲区使用大小寄存器（只读）
    nr_reg         // 寄存器总数
};

static uint8_t *sbuf = NULL;        // 流缓冲区指针
static uint32_t *audio_base = NULL; // 音频寄存器基地址
static SDL_mutex *audio_mutex = NULL; // 音频线程安全互斥锁

// SDL音频回调函数 - 从流缓冲区读取数据并填充到SDL缓冲区
// 注意：此函数在SDL音频线程中运行，需要线程安全保护
static void audio_callback(void *userdata, Uint8 *stream, int len) {
    // 边界检查：确保所有必要的指针和参数都有效
    if (len <= 0 || !stream || !sbuf || !audio_base || !audio_mutex) {
        return;
    }
    
    // 获取互斥锁，保证与主线程的数据访问同步
    SDL_LockMutex(audio_mutex);
    
    // 清零输出缓冲区
    SDL_memset(stream, 0, len);
    
    // 类型转换确保一致性，避免有符号/无符号比较警告
    uint32_t available = audio_base[reg_count];    // 当前缓冲区可用数据大小
    uint32_t request_len = (uint32_t)len;          // SDL请求的数据大小
    
    if (available >= request_len) {
        // 情况1：缓冲区数据足够SDL请求
        // 复制请求的数据量到SDL输出流
        SDL_memcpy(stream, sbuf, request_len);
        
        // 更新剩余数据大小
        audio_base[reg_count] = available - request_len;
        
        // 如果还有剩余数据，将其移动到缓冲区前部
        // 使用memmove而非循环，效率更高且处理内存重叠安全
        if (audio_base[reg_count] > 0) {
            memmove(sbuf, sbuf + request_len, audio_base[reg_count]);
        }
    } else {
        // 情况2：缓冲区数据不足SDL请求
        if (available > 0) {
            // 复制所有可用数据到SDL输出流
            SDL_memcpy(stream, sbuf, available);
        }
        
        // 剩余部分填充静音（已经在开始时清零，这里是为了明确逻辑）
        SDL_memset(stream + available, 0, request_len - available);
        
        // 缓冲区已用完
        audio_base[reg_count] = 0;
    }
    
    // 释放互斥锁
    SDL_UnlockMutex(audio_mutex);
}

// 初始化SDL音频子系统
static void audio_init_sdl() {
    SDL_AudioSpec s = {};
    s.format =
        AUDIO_S16SYS;  // 假设系统中音频数据的格式总是使用16位有符号数来表示
    s.userdata = NULL; // 不使用
    s.freq = audio_base[reg_freq];         // 从寄存器读取频率
    s.channels = audio_base[reg_channels]; // 从寄存器读取声道数
    s.samples = audio_base[reg_samples];   // 从寄存器读取采样数
    s.callback = audio_callback;           // 设置回调函数

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    SDL_OpenAudio(&s, NULL);
    SDL_PauseAudio(0); // 开始播放
}

// 音频设备寄存器读写处理函数
static void audio_io_handler(uint32_t offset, int len, bool is_write) {
    uint32_t reg_idx = offset / sizeof(uint32_t);

    if (is_write) {
        // 写寄存器操作
        switch (reg_idx) {
        case reg_init:
            // 写入init寄存器时，触发SDL音频子系统初始化
            if (audio_base[reg_init] != 0) {
                audio_init_sdl();
                audio_base[reg_init] = 0; // 重置init寄存器
            }
            break;
        default: break;
        }
    }
}


void init_audio() {
    // 计算寄存器空间大小
    uint32_t space_size = sizeof(uint32_t) * nr_reg;
    // 分配寄存器空间
    audio_base = (uint32_t *)new_space(space_size);

    // 初始化音频线程安全互斥锁
    audio_mutex = SDL_CreateMutex();
    if (!audio_mutex) {
        // 互斥锁创建失败的错误处理
        // 在实际项目中应该有适当的错误处理机制
    }

    // 初始化寄存器默认值
    audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
    audio_base[reg_count] = 0;

    // 根据配置选择端口I/O或内存映射I/O
#ifdef CONFIG_HAS_PORT_IO
    add_pio_map("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size,
                audio_io_handler);
#else
    add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size,
                 audio_io_handler);
#endif

    // 分配并映射流缓冲区
    sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
    // 正确实现中直接将sbuf映射为MMIO，让硬件直接写入
    add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
}
