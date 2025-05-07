#include <nvboard.h>
#include "Vtop.h"

void nvboard_bind_all_pins(Vtop* top) {
	nvboard_bind_pin( &top->btn, 1, BTNC);
	nvboard_bind_pin( &top->seg, 14, SEG1A, SEG1B, SEG1C, SEG1D, SEG1E, SEG1F, SEG1G, SEG0A, SEG0B, SEG0C, SEG0D, SEG0E, SEG0F, SEG0G);
}
