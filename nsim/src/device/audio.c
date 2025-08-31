/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NSIM is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

// 音频设备寄存器定义
enum {
  reg_freq,        // 音频频率寄存器
  reg_channels,    // 声道数寄存器
  reg_samples,     // 采样数寄存器
  reg_sbuf_size,   // 流缓冲区大小寄存器（只读）
  reg_init,        // 初始化寄存器（写入触发SDL初始化）
  reg_count,       // 当前缓冲区使用大小寄存器（只读）
  nr_reg           // 寄存器总数
};

static uint8_t *sbuf = NULL;        // 流缓冲区指针
static uint32_t *audio_base = NULL; // 音频寄存器基地址

// 音频流缓冲区管理变量
static int buf_front = 0;    // 队列头指针
static int buf_rear = 0;     // 队列尾指针
static int buf_count = 0;    // 当前缓冲区使用大小

// SDL音频回调函数 - 从流缓冲区读取数据并填充到SDL缓冲区
static void audio_callback(void *userdata, uint8_t *stream, int len) {
    int nread = len;
    
    // 如果请求的数据量大于当前缓冲区数据量，只能提供现有数据
    if (buf_count < len) {
        nread = buf_count;
        // 清零SDL缓冲区剩余部分，避免噪音
        memset(stream + nread, 0, len - nread);
    }
    
    // 从环形缓冲区复制数据到SDL流
    int first_copy = nread;
    if (buf_front + nread > CONFIG_SB_SIZE) {
        // 需要分两段复制（环形缓冲区回绕）
        first_copy = CONFIG_SB_SIZE - buf_front;
        memcpy(stream, sbuf + buf_front, first_copy);
        memcpy(stream + first_copy, sbuf, nread - first_copy);
    } else {
        // 一次性复制
        memcpy(stream, sbuf + buf_front, nread);
    }
    
    // 更新队列状态
    buf_front = (buf_front + nread) % CONFIG_SB_SIZE;
    buf_count -= nread;
}

static void audio_init_sdl() {
    SDL_AudioSpec s = {};
    s.format = AUDIO_S16SYS;  // 假设系统中音频数据的格式总是使用16位有符号数来表示
    s.userdata = NULL;        // 不使用
    s.freq = audio_base[reg_freq];      // 从寄存器读取频率
    s.channels = audio_base[reg_channels]; // 从寄存器读取声道数
    s.samples = audio_base[reg_samples];   // 从寄存器读取采样数
    s.callback = audio_callback;           // 设置回调函数
    
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    SDL_OpenAudio(&s, NULL);
    SDL_PauseAudio(0);  // 开始播放
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
    uint32_t reg_idx = offset / sizeof(uint32_t);
    
    if (is_write) {
        // 写寄存器操作
        switch (reg_idx) {
            case reg_freq:
            case reg_channels:
            case reg_samples:
                // 这些寄存器允许写入，用于设置SDL音频参数
                break;
            case reg_init:
                // 写入init寄存器时，触发SDL音频子系统初始化
                if (audio_base[reg_init] != 0) {
                    audio_init_sdl();
                }
                break;
            case reg_sbuf_size:
            case reg_count:
                // 这些寄存器只读，忽略写入
                break;
        }
    } else {
        // 读寄存器操作
        switch (reg_idx) {
            case reg_sbuf_size:
                // 返回流缓冲区大小
                audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
                break;
            case reg_count:
                // 返回当前流缓冲区已使用大小
                audio_base[reg_count] = buf_count;
                break;
        }
    }
}

// 流缓冲区写入处理函数
static void audio_sbuf_handler(uint32_t offset, int len, bool is_write) {
    if (is_write) {
        // 向流缓冲区写入音频数据
        int space_left = CONFIG_SB_SIZE - buf_count;
        int nwrite = (len <= space_left) ? len : space_left;
        
        if (nwrite > 0) {
            // 更新队列尾指针和计数
            buf_rear = (buf_rear + nwrite) % CONFIG_SB_SIZE;
            buf_count += nwrite;
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
    add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
    add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

    // 分配并映射流缓冲区
    sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
    add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, audio_sbuf_handler);
    
    // 初始化流缓冲区状态
    buf_front = buf_rear = buf_count = 0;
}
