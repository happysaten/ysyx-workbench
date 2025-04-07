#include <nvboard.h>
#include "Vtop.h"

void nvboard_bind_all_pins(Vtop* top) {
	nvboard_bind_pin( &top->seg, 42, SEG5A, SEG5B, SEG5C, SEG5D, SEG5E, SEG5F, SEG5G, SEG4A, SEG4B, SEG4C, SEG4D, SEG4E, SEG4F, SEG4G, SEG3A, SEG3B, SEG3C, SEG3D, SEG3E, SEG3F, SEG3G, SEG2A, SEG2B, SEG2C, SEG2D, SEG2E, SEG2F, SEG2G, SEG1A, SEG1B, SEG1C, SEG1D, SEG1E, SEG1F, SEG1G, SEG0A, SEG0B, SEG0C, SEG0D, SEG0E, SEG0F, SEG0G);
	nvboard_bind_pin( &top->ps2_clk, 1, PS2_CLK);
	nvboard_bind_pin( &top->ps2_data, 1, PS2_DAT);
	nvboard_bind_pin( &top->overflow, 1, LD0);
	nvboard_bind_pin( &top->ready, 1, LD1);
	nvboard_bind_pin( &top->nextdata_n, 1, LD2);
}
