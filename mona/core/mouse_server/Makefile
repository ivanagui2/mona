SRCTOP=../..
include $(SRCTOP)/Makefile.inc

TARGET  = MOUSE
SOURCES = main.cpp
INSTDIR = $(BINDIR)/SERVERS

ADDLINK    = --enable-auto-import

SHAREDIR   = $(MONADIR)/share
ifneq ($(BUILD_TARGET),ELF)
include $(SHAREDIR)/configs/monapi-ex5.inc
else
include $(SHAREDIR)/configs/monapi-el5.inc
endif

-include dependencies
