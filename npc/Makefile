VSRCS = $(shell find $(abspath ./vsrc) -name "*.sv" -o -name "*.vlt")
IMG ?=
all:
	# 提交git
	# $(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	mkdir -p build
# 	verilator --cc --build -j 0 $^ $(LINTSRC) -Wall --top-module top --Mdir build/obj_dir --trace-fst

WORK_DIR = $(abspath .)
CVCFLAGS ?=

verilator:
	@verilator --cc --build \
		--top-module top \
		--trace-fst \
		-CFLAGS "$(CVCFLAGS) -I$(NSIM_HOME)/include" \
		--Mdir $(NSIM_HOME)/src/verilator/obj_dir \
		$(VSRCS) \
		`find $(NSIM_HOME)/src/verilator -name "*.cpp" | grep -Ev 'obj_dir' | tr '\n' ' '`

clean:
	rm -rf $(NSIM_HOME)/src/verilator/obj_dir

include ../Makefile
.PHONY: all clean verilator
