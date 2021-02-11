
lib_LTLIBRARIES = aasm_out-src.la

aasm_out_src_la_SOURCES  = src.h src.c module.c

aasm_out_src_la_LDFLAGS  = -module -avoid-version
aasm_out_src_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la

aasm_out_src_la_CFLAGS = -I$(top_srcdir)/include
