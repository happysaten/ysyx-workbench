CVSRCS = init.cpp \
				 exec.cpp \
				 dpi/mem.cpp \
				 dpi/gprs.cpp \
				 dpi/inst.cpp \

CVSRCS := $(addprefix $(NSIM_HOME)/src/verilator/, $(CVSRCS))

ifdef CONFIG_WTRACE
CVCFLAGS = -DCONFIG_WTRACE
else
CVCFLAGS =
endif

wv:
	gtkwave $(NSIM_HOME)/build/trace.fst &

.PHONY: wv
