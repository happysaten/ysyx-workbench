// GPU 相关功能实现文件，用于处理图形输出和帧缓冲操作
#include <am.h>
#include <nemu.h>
#include <stdint.h>

// 定义同步地址，用于触发帧缓冲同步到屏幕
#define SYNC_ADDR (VGACTL_ADDR + 4)

// GPU 初始化函数，目前为空实现
void __am_gpu_init() {}

// 获取 GPU 配置信息，包括宽度、高度和视频内存大小
void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
    uint32_t vga_ctl = inl(VGACTL_ADDR);        // 从 VGA 控制地址读取配置
    uint32_t width = vga_ctl & 0xffff;          // 低 16 位为宽度
    uint32_t height = (vga_ctl >> 16) & 0xffff; // 高 16 位为高度
    uint32_t vmemsz =
        width * height * 4; // 视频内存大小：宽度 * 高度 * 4 字节/像素
    *cfg = (AM_GPU_CONFIG_T){.present = true,
                             .has_accel = false,
                             .width = width,
                             .height = height,
                             .vmemsz = vmemsz};
}

// 帧缓冲绘制函数，向屏幕 (x, y) 坐标绘制 w*h 矩形图像
// 像素数据按行优先存储在 pixels 中，每个像素为 32 位整数 (00RRGGBB 格式)
// 如果 sync 为 true，则同步帧缓冲到屏幕
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
    uint32_t *fb = (uint32_t *)FB_ADDR; // 计算帧缓冲基地址
    uint32_t *pixels = ctl->pixels; // 获取像素数据指针
    uint32_t width = inl(VGACTL_ADDR) & 0xffff; // 屏幕宽度

    // 遍历图像的每一行和每一列
    for (uint32_t row = 0; row < ctl->h; ++row) {
        for (uint32_t col = 0; col < ctl->w; ++col) {
            // 计算帧缓冲中的位置：(y + row) * width + (x + col)
            uint32_t fb_index = (ctl->y + row) * width + (ctl->x + col);
            // 像素数据索引：row * w + col
            uint32_t pixel_index = row * ctl->w + col;
            // 写入像素数据到帧缓冲
            fb[fb_index] = pixels[pixel_index];
        }
    }

    // 如果需要同步，则触发同步操作
    if (ctl->sync) {
        outl(SYNC_ADDR, 1);
    }
}

// 获取 GPU 状态，目前固定为 ready
void __am_gpu_status(AM_GPU_STATUS_T *status) { status->ready = true; }
