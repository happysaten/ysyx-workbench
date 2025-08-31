#include <am.h>
#include <nemu.h>
#include <sys/types.h>

void __am_timer_init() {}

// AM系统启动时间, 可读出系统启动后的微秒数.
void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
    uint64_t us_1 = inl(RTC_ADDR + 4);
    uint64_t us_0 = inl(RTC_ADDR);
    uptime->us = (us_1 << 32) | us_0;
}

// AM实时时钟(RTC, Real Time Clock), 可读出当前的年月日时分秒
void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
    rtc->second = 0;
    rtc->minute = 0;
    rtc->hour = 0;
    rtc->day = 0;
    rtc->month = 0;
    rtc->year = 1900;
}
