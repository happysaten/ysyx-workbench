#include <cstdint>

// typedef uint32_t word_t;
// typedef uint32_t vaddr_t;
// typedef uint32_t paddr_t;

struct InstInfo {
  int       inst;
  long long dnpc;
  // bool      kill;
  // bool      invalid;
  // bool      device;
  bool      en;
};
