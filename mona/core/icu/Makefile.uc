ifndef $(MONADIR)
export MONADIR=$(shell cd $(PWD)/../../../mona; pwd)
endif

LIB_STATIC   = libicuuc.a
TARGET       = $(LIB_STATIC)

LIB_IMPORT   = libicuuc-imp.a
LIB_DYNAMIC  = ICUUC.DLL
LIB_DYNAMIC5 = ICUUC.DL5
TARGET      += $(LIB_IMPORT) $(LIB_DYNAMIC) $(LIB_DYNAMIC5)

include $(MONADIR)/share/configs/Makefile.inc
include $(MONADIR)/share/configs/dirnames.inc


_COMMON_CXXSOURCES= \
errorcode.cpp \
uobject.cpp \
ucol_swp.cpp \
uhash_us.cpp \
ustrenum.cpp \
uvector.cpp \
ustack.cpp \
uvectr32.cpp \
uvectr64.cpp \
resbund.cpp \
resbund_cnv.cpp \
locid.cpp \
locutil.cpp \
locavailable.cpp \
locdispnames.cpp \
loclikely.cpp \
locresdata.cpp \
bytestream.cpp \
stringpiece.cpp \
unistr_cnv.cpp \
unistr.cpp \
unistr_case.cpp \
unistr_props.cpp \
utext.cpp \
normalizer2impl.cpp \
normalizer2.cpp \
filterednormalizer2.cpp \
normlzr.cpp \
unorm.cpp \
unormcmp.cpp \
chariter.cpp \
schriter.cpp \
uchriter.cpp \
uiter.cpp \
uprops.cpp \
propname.cpp \
utrie2.cpp \
bmpset.cpp \
unisetspan.cpp \
uset_props.cpp \
uniset_props.cpp \
uset.cpp \
uniset.cpp \
usetiter.cpp \
ruleiter.cpp \
caniter.cpp \
unifilt.cpp \
unifunct.cpp \
brkiter.cpp \
ubrk.cpp \
brkeng.cpp \
dictbe.cpp \
triedict.cpp \
rbbi.cpp \
rbbidata.cpp \
rbbinode.cpp \
rbbirb.cpp \
rbbiscan.cpp \
rbbisetb.cpp \
rbbistbl.cpp \
rbbitblb.cpp \
serv.cpp \
servnotf.cpp \
servls.cpp \
servlk.cpp \
servlkf.cpp \
servrbf.cpp \
servslkf.cpp \
uidna.cpp \
usprep.cpp \
util.cpp \
util_props.cpp \
parsepos.cpp \
locbased.cpp \
mutex.cpp \
dtintrv.cpp \
ucnvsel.cpp

_COMMON_CSOURCES= \
putil.c \
umath.c \
utypes.c \
uinvchar.c \
umutex.c \
ucln_cmn.c \
uinit.c \
cmemory.c \
udata.c \
ucmndata.c \
udatamem.c \
umapfile.c \
udataswp.c \
utrace.c \
uhash.c \
uenum.c \
ucnv.c \
ucnv_bld.c \
ucnv_cnv.c \
ucnv_io.c \
ucnv_cb.c \
ucnv_err.c \
ucnvlat1.c \
ucnv_u7.c \
ucnv_u8.c \
ucnv_u16.c \
ucnv_u32.c \
ucnvscsu.c \
ucnvbocu.c \
ucnv_ext.c \
ucnvmbcs.c \
ucnv2022.c \
ucnvhz.c \
ucnv_lmb.c \
ucnvisci.c \
ucnvdisp.c \
ucnv_set.c \
uresbund.c \
ures_cnv.c \
uresdata.c \
ucat.c \
locmap.c \
uloc.c \
ustr_cnv.c \
utf_impl.c \
ustring.c \
ustrcase.c \
ucasemap.c \
cstring.c \
ustrfmt.c \
ustrtrns.c \
ustr_wcs.c \
unorm_it.c \
uchar.c \
ucase.c \
ubidi_props.c \
ubidi.c \
ubidiwrt.c \
ubidiln.c \
ushape.c \
uscript.c \
usc_impl.c \
unames.c \
utrie.c \
utrie2_builder.c \
uarrsort.c \
punycode.c \
cwchar.c \
wintz.c \
propsvec.c \
ulist.c \
uloc_tag.c \
icudataver.c \
icuplug.c

COMMON_CXXSOURCES=$(patsubst %.cpp, icu/source/common/%.cpp, $(_COMMON_CXXSOURCES))
COMMON_CSOURCES=$(patsubst %.c, icu/source/common/%.c, $(_COMMON_CSOURCES))

COMMON_CFLAGS=-D_REENTRANT -I icu/source/common -I icu/source/i18n   "-DDEFAULT_ICU_PLUGINS=\"/usr/local/lib/icu\" "  -DU_COMMON_IMPLEMENTATION -g -O2 -Wall -ansi -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -fvisibility=hidden #-pedantic
COMMON_CXXFLAGS=-D_REENTRANT -I icu/source/common -I icu/source/i18n   "-DDEFAULT_ICU_PLUGINS=\"/usr/local/lib/icu\" "  -DU_COMMON_IMPLEMENTATION -g -O2 -Wall -ansi -Wshadow -Wpointer-arith -Wwrite-strings -Wno-long-long  -fvisibility=hidden

OBJECTS = $(COMMON_CSOURCES:.c=.o) $(COMMON_CXXSOURCES:.cpp=.o) icudt44l_dat.o

INCLUDE =  -I. -I.. -I$(INCDIR) -I$(INCDIR)/STLport-5.2.1/stlport -I$(INCDIR)/monalibc -I$(INCDIR)/c++

all : $(TARGET)

$(LIB_STATIC): $(OBJECTS)
	rm -f $@
	$(AR) $@ $(OBJECTS)
	$(RANLIB) $@

ifneq ($(BUILD_TARGET),ELF)
$(LIB_IMPORT): $(LIB_DYNAMIC)
$(LIB_DYNAMIC): $(OBJECTS)
	$(LD) -e _dllmain --export-all-symbols --enable-auto-import --out-implib $(LIB_IMPORT) -o $@ $(OBJECTS) -L$(MONADIR)/lib -lmonapi-imp -lmonalibc-imp
	$(STRIP) $@

$(LIB_DYNAMIC5): $(LIB_DYNAMIC)
	`which bim2bin` in:$(LIB_DYNAMIC) out:$@ -osacmp -tek5 BS:0 eprm:z0
endif

icudt44l_dat.o : icudt44l_dat.s
	$(CC) -g -O2 -Wall -ansi -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -fno-common -c -I icu/common -I  -dynamic -o icudt44l_dat.o icudt44l_dat.s

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) -c -o $(<:.cpp=.o) $(CXXFLAGS) $(COMMON_CXXFLAGS) $(INCLUDE) $<

.SUFFIXES: .o .c
.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(COMMON_CFLAGS) $(INCLUDE) -fno-builtin

depend:
	$(CXX) -MM $(INCLUDE) $(CXXFLAGS)  $(SOURCES) > dependencies

clean:
	rm -f $(TARGET) $(OBJECTS)

distclean: clean
	rm dependencies
	touch dependencies

install: $(TARGET)

uninstall:

-include dependencies

