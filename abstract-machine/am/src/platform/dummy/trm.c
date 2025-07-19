#include <am.h>

Area heap = RANGE(NULL, NULL);

void putch(char ch) {
}

void halt(int code) {
  // while (1);
  asm volatile("mv a0, %0; ebreak" : :"r"(code));
}
