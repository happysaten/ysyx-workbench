// 音频设备IOE实现，提供音频播放功能
#include <am.h>
#include <nemu.h>

// 音频设备寄存器地址定义
#define AUDIO_FREQ_ADDR (AUDIO_ADDR + 0x00)      // 采样频率寄存器
#define AUDIO_CHANNELS_ADDR (AUDIO_ADDR + 0x04)  // 声道数寄存器
#define AUDIO_SAMPLES_ADDR (AUDIO_ADDR + 0x08)   // 样本缓冲区大小寄存器
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c) // 音频缓冲区总大小寄存器
#define AUDIO_INIT_ADDR (AUDIO_ADDR + 0x10)      // 初始化触发寄存器
#define AUDIO_COUNT_ADDR (AUDIO_ADDR + 0x14)     // 当前缓冲区数据量寄存器

// 音频设备初始化
void __am_audio_init() {
    // 向初始化寄存器写入1，触发音频设备初始化
    outl(AUDIO_INIT_ADDR, 1);
}

// 获取音频设备配置信息
void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
    cfg->present = true;                      // 音频设备存在
    cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR); // 从硬件读取音频缓冲区大小
}

// 设置音频控制参数
void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
    // 初始化音频设备
    __am_audio_init();
    // 设置采样频率（如8000Hz）
    outl(AUDIO_FREQ_ADDR, ctrl->freq);
    // 设置声道数（1=单声道，2=立体声）
    outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
    // 设置每次播放的样本数量
    outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
}

// 获取音频设备状态
void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
    // 读取当前缓冲区中未播放的数据字节数
    // 当count为0时表示缓冲区为空，音频播放完成
    stat->count = inl(AUDIO_COUNT_ADDR);
}

// 播放音频数据
void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
    // 计算音频数据长度（字节数）
    uint32_t len = (uint8_t *)ctl->buf.end - (uint8_t *)ctl->buf.start;
    // 获取当前缓冲区中已有的数据量
    int count = inl(AUDIO_COUNT_ADDR);
    // 如果缓冲区已满，直接返回
    if (count + len > inl(AUDIO_SBUF_SIZE_ADDR))
        return;

    for (int i = 0; i < len; i++) {
        // 将音频数据逐字节写入音频缓冲区
        outb(AUDIO_SBUF_ADDR + count + i, ((uint8_t *)ctl->buf.start)[i]);
    }

    // 通知音频设备有新的数据可以播放
    // 写入数据长度到计数寄存器，触发音频播放
    outl(AUDIO_COUNT_ADDR, count + len);
}

// void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
//   int len = ctl->buf.end - ctl->buf.start;
//   int count = inl(AUDIO_COUNT_ADDR);

//   uintptr_t sbufAddr = (uintptr_t)AUDIO_SBUF_ADDR + count;
//   for (int i = 0; i < len; i++){
//     outb(sbufAddr + i, *(uint8_t *)(ctl->buf.start + i));
//   }
//   outl(AUDIO_COUNT_ADDR, count + len);
// }