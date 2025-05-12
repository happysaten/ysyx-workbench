all:
	@echo "Write this Makefile by your self."


# sim:
# 	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	verilator --cc --exe --build -j 0 -Wall csrc/sim_main.cpp vsrc/top.v --trace-fst
# 	obj_dir/Vtop
# 	gtkwave obj_dir/Vtop_wave.fst

VSRCS = $(shell find $(abspath ./vsrc) -name "*.sv")
LINTSRC = $(shell find $(abspath ./vsrc) -name "*.vlt")

# UTILS = vsrc/bcd7seg.sv vsrc/ps2_keyboard.sv 
include $(NVBOARD_HOME)/scripts/nvboard.mk
# sim: csrc/main.cpp vsrc/top.sv $(UTILS)
sim: csrc/main.cpp $(VSRCS)
	# 提交git
	# $(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	# 生成约束函数
	mkdir -p build
	python $(NVBOARD_HOME)/scripts/auto_pin_bind.py constr/top.nxdc constr/auto_bind.cpp

	# 生成nvboard库
	make nvboard-archive

	# 执行verilator
	verilator --cc --exe --build -j 0 $^ $(LINTSRC) constr/auto_bind.cpp -Wall --top-module top $(NVBOARD_ARCHIVE) $(addprefix -LDFLAGS , $(LDFLAGS)) -CFLAGS -I$(NVBOARD_HOME)/include --Mdir build/obj_dir
	

	# 执行可执行文件
	build/obj_dir/Vtop

clean:
	rm -rf build
	-rm -rf result

.PHONY: all clean sim

include ../Makefile
include sta.mk
