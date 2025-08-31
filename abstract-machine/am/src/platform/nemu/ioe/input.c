#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
    // 从键盘地址读取16位键盘码
    uint32_t code = inw(KBD_ADDR);
    // // 判断是否按下
    // kbd->keydown = (code & KEYDOWN_MASK) ? 1 : 0;
    // // 设置按键码, 如果未按下则设置为AM_KEY_NONE
    // kbd->keycode = kbd->keydown ? (code & ~KEYDOWN_MASK) : AM_KEY_NONE;
    if (code != 0) {
        // 提取键码（低15位）和按下状态（高位）
        kbd->keycode = code & ~KEYDOWN_MASK;
        kbd->keydown = (code & KEYDOWN_MASK) ? 1 : 0;
    } else {
        // 无按键时，返回AM_KEY_NONE
        kbd->keydown = 0;
        kbd->keycode = AM_KEY_NONE;
    }
}
