TARGET  = TERMINAL
SOURCES = terminal.cpp test.cpp

ifndef $(MONADIR)
export MONADIR=$(shell cd $(PWD)/../../../mona; pwd)
endif

INSTDIR = $(BINDIR)/APPS/MONAGUI
INSTFILES= $(TARGET).map
SHAREDIR   = $(MONADIR)/share
ifneq ($(BUILD_TARGET),ELF)
ADDLINK    = -lamonagui-imp -lmonalibc-imp --enable-auto-import
ADDLINKDEP = $(MONADIR)/lib/libamonagui-imp.a $(MONADIR)/lib/libmonalibc-imp.a
include $(SHAREDIR)/configs/monapi-ex5.inc
else
ADDLINK    = -lamonagui-imp -lmonalibc-imp -lmonapi-imp
ADDLINKDEP = $(MONADIR)/lib/libamonagui.a $(MONADIR)/lib/libmonalibc.a
include $(SHAREDIR)/configs/monapi-el5.inc
endif

CXXFLAGS += -DDEBUG
INCLUDE += -I .

-include dependencies
