SRCTOP=../..
include $(SRCTOP)/Makefile.inc

TARGET  = PROCESS
SOURCES = main.cpp
INSTDIR = $(BINDIR)

ADDLINK    = -lmonalibc
ADDLINKDEP = $(MONADIR)/lib/libmonalibc.a
SHAREDIR   = $(MONADIR)/share
include $(SHAREDIR)/configs/monapi-bin.inc

-include dependencies
