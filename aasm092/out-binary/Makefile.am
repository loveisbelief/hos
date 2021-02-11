
lib_LTLIBRARIES = aasm_out-binary.la

aasm_out_binary_la_SOURCES = binary.c binary.h binary_reloc.c	\
binary_symbols.c module.c out.c out.h

aasm_out_binary_la_LDFLAGS  = -module -avoid-version
aasm_out_binary_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la

aasm_out_binary_la_CFLAGS = -I$(top_srcdir)/include
