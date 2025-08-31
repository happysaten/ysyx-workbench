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

// SDL音频回调函数 - 从流缓冲区读取数据并填充到SDL缓冲区
static void audio_callback(void *userdata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_base[reg_count] > len) {
        SDL_memcpy(stream, sbuf, len);
        audio_base[reg_count] = audio_base[reg_count] - len;
        // 简单地移动剩余数据到缓冲区前部
        for (uint32_t i = 0; i < audio_base[reg_count]; i++)
            sbuf[i] = sbuf[len + i];
    } else {
        SDL_memcpy(stream, sbuf, audio_base[reg_count]);
        SDL_memset(stream + audio_base[reg_count], 0,
                   len - audio_base[reg_count]);
        audio_base[reg_count] = 0;
    }
}

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
