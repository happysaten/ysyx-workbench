# 指定使用的 shell 类型
SHELL := /bin/bash
WORK_DIR := /home/saten/MyBuild/yosys-sta
# 定义输出目录，默认为 $(PROJ_PATH)/result
O ?= result
# 定义设计名称，默认为 gcd
DESIGN ?= top
# 定义默认的时序约束文件路径
SDC_FILE ?= /home/saten/MyBuild/yosys-sta/scripts/default.sdc
# # 定义 RTL 文件路径，查找 example 目录下的所有 .v 文件
RTL_FILES ?= $(VSRCS)
# 导出时钟频率（单位 MHz），默认为 500 MHz
export CLK_FREQ_MHZ ?= 50
# 导出时钟端口名称，默认为 clk
export CLK_PORT_NAME ?= clk
# 定义使用的工艺库，默认为 nangate45
PDK = nangate45
# 定义 yosys 可执行文件路径
YOSYS := /home/saten/MyBuild/oss-cad-suite/bin/yosys

# 定义结果目录，包含设计名称和时钟频率
RESULT_DIR = $(O)/$(DESIGN)-$(CLK_FREQ_MHZ)MHz
# 定义脚本目录
SCRIPT_DIR = $(WORK_DIR)/scripts
# 定义综合后的网表文件路径
NETLIST_SYN_V   = $(RESULT_DIR)/$(DESIGN).netlist.syn.v
# 定义修复扇出后的网表文件路径
NETLIST_FIXED_V = $(RESULT_DIR)/$(DESIGN).netlist.fixed.v
# 定义时序分析报告文件路径
TIMING_RPT = $(RESULT_DIR)/$(DESIGN).rpt


# 综合目标：生成综合后的网表文件
# 依赖于 RTL 源文件和 Yosys 的综合脚本
syn: $(NETLIST_SYN_V)
$(NETLIST_SYN_V): $(RTL_FILES) $(SCRIPT_DIR)/yosys.tcl
	# 创建输出目录
	mkdir -p $(@D)
	# 调用 Yosys 运行综合脚本，生成综合后的网表
	echo tcl $(SCRIPT_DIR)/yosys.tcl $(DESIGN) $(PDK) \"$(RTL_FILES)\" $@ | $(YOSYS) -l $(@D)/yosys.log -s -

# 修复扇出目标：生成修复扇出后的网表文件
# 依赖于综合后的网表文件、SDC 文件和修复扇出的脚本
fix-fanout: $(NETLIST_FIXED_V)
$(NETLIST_FIXED_V): $(SCRIPT_DIR)/fix-fanout.tcl $(SDC_FILE) $(NETLIST_SYN_V)
	# 使用 iEDA 工具运行修复扇出的脚本，并生成日志
	set -o pipefail && $(WORK_DIR)/./bin/iEDA -script $^ $(DESIGN) $(PDK) $@ 2>&1 | tee $(RESULT_DIR)/fix-fanout.log
	# 调用 Yosys 运行面积优化脚本
	echo tcl $(SCRIPT_DIR)/yosys-area.tcl $(DESIGN) $(PDK) $@ | $(YOSYS) -l $(@D)/yosys-fixed.log -s -

# 静态时序分析目标：生成时序报告
# 依赖于修复扇出后的网表文件、SDC 文件和 STA 脚本
sta: $(TIMING_RPT)
$(TIMING_RPT): $(SCRIPT_DIR)/sta.tcl $(SDC_FILE) $(NETLIST_FIXED_V)
	# 使用 iEDA 工具运行 STA 脚本，并生成时序分析日志
	set -o pipefail && $(WORK_DIR)/./bin/iEDA -script $^ $(DESIGN) $(PDK) 2>&1 | tee $(RESULT_DIR)/sta.log

# 声明伪目标，避免与文件名冲突
.PHONY: syn fix-fanout sta
