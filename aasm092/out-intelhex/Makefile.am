
lib_LTLIBRARIES = aasm_out-intelhex.la

aasm_out_intelhex_la_SOURCES = intelhex.h intelhex.c module.c	\
intelhex_reloc.c intelhex_symbols.c

aasm_out_intelhex_la_LDFLAGS  = -module -avoid-version
aasm_out_intelhex_la_LIBADD   = ../libcore/libaasm_core.la

aasm_out_intelhex_la_CFLAGS = -I$(top_srcdir)/include
