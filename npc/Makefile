VSRCS = $(shell find $(abspath ./vsrc) -name "*.sv" -o -name "*.vlt")
IMG ?=
all: obj_dir/Vtop

WORK_DIR = $(abspath .)
CVCFLAGS ?=

obj_dir/Vtop: $(VSRCS)
	verilator --cc --build \
		--top-module top \
		--trace-fst \
		-CFLAGS "$(CVCFLAGS) -I$(NSIM_HOME)/include" \
		--Mdir obj_dir \
		$(VSRCS)

verilator: obj_dir/Vtop

clean:
	rm -rf obj_dir

.PHONY: all clean verilator