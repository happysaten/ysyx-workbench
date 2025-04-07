all:
	@echo "Write this Makefile by your self."


# sim:
# 	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	verilator --cc --exe --build -j 0 -Wall csrc/sim_main.cpp vsrc/top.v --trace-fst
# 	obj_dir/Vtop
# 	gtkwave obj_dir/Vtop_wave.fst

# VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
UTILS = vsrc/bcd7seg.sv vsrc/ps2_keyboard.sv 
include $(NVBOARD_HOME)/scripts/nvboard.mk
sim: csrc/main.cpp vsrc/top.sv obj_dir/auto_bind.cpp $(UTILS)
	# 提交git
	# $(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	# 生成约束函数
	mkdir -p obj_dir
	python $(NVBOARD_HOME)/scripts/auto_pin_bind.py constr/top.nxdc obj_dir/auto_bind.cpp

	# 生成nvboard库
	make nvboard-archive

	# 执行verilator
	verilator --cc --exe --build -j 0 $^ -Wall --top-module top $(NVBOARD_ARCHIVE) $(addprefix -LDFLAGS , $(LDFLAGS)) -CFLAGS -I$(NVBOARD_HOME)/include
	

	# 执行可执行文件
	obj_dir/Vtop

clean:
	rm -rf obj_dir

include ../Makefile
