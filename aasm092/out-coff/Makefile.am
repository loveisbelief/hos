
lib_LTLIBRARIES = aasm_out-coff.la

aasm_out_coff_la_SOURCES = coff.c coff_arch.c coff_endian.c				\
coff_reloc.c coff_sections.c coff_symbols.c module.c coff_directive.c

aasm_out_coff_la_SOURCES += coff.h coff_bits.h realloc.h

aasm_out_coff_la_LDFLAGS  = -module -avoid-version
aasm_out_coff_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la

aasm_out_coff_la_CFLAGS = -I$(top_srcdir)/include

CLEANFILES = coff_directive.c
BUILT_SOURCES = coff_directive.c

EXTRA_DIST = coff_directive.wlex

coff_directive.c: $(srcdir)/coff_directive.wlex $(top_srcdir)/build/word_lex.pl
	perl $(top_srcdir)/build/word_lex.pl $<

