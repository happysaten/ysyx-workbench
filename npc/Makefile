VSRCS = $(shell find $(abspath .)/vsrc -name "*.sv" -o -name "*.svh")
LINTSRC = $(shell find $(abspath .)/vsrc -name "*.vlt")
PERIP_DIR = $(abspath ..)/ysyxSoC/perip
PERIP_V = $(shell if [ -d "$(PERIP_DIR)" ]; then find "$(PERIP_DIR)" -name "*.v"; fi)
PERIP_INC = -I$(PERIP_DIR)/uart16550/rtl -I$(PERIP_DIR)/spi/rtl
IMG ?=
all:
	# 提交git
	# $(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	mkdir -p build
# 	verilator --cc --build -j 0 $^ $(LINTSRC) -Wall --top-module top --Mdir build/obj_dir --trace-fst

WORK_DIR = $(abspath .)
CVCFLAGS ?=
OBJ_DIR = $(NSIM_HOME)/src/verilator/obj_dir

TOP_MODULE ?= ysyxSoCFull
YSOC_BUILD_DIR = $(abspath ..)/ysyxSoC/build
YSOC_TOP_V = $(YSOC_BUILD_DIR)/ysyxSoCFull.v

verilator:
	# 提交git
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Generating Verilator sources..."
	verilator --cc --build -j 0\
		--top-module $(TOP_MODULE) --prefix Vtop \
		--trace-fst \
		-CFLAGS "$(CVCFLAGS) -I$(NSIM_HOME)/include" \
		--Mdir $(OBJ_DIR) \
		$(VSRCS) $(LINTSRC) $(PERIP_V)  $(PERIP_INC) $(YSOC_TOP_V) \
		`find $(NSIM_HOME)/src/verilator -name "*.cpp" | grep -Ev 'obj_dir' | tr '\n' ' '` \
		--timescale "1ns/1ns" --no-timing


clean:
	rm -rf $(OBJ_DIR)
	rm -rf result

include ../Makefile
include sta.mk
.PHONY: all clean verilator
