
lib_LTLIBRARIES =
lib_LTLIBRARIES += aasm_out-elf32.la
lib_LTLIBRARIES += aasm_out-elf64.la

SRC = elf.c elf_arch.c elf_endian.c elf_obj_sections.c elf_reloc.c	\
	elf_sections.c elf_symbols.c module.c out.c elf.h elf_bits.h

aasm_out_elf32_la_SOURCES  = $(SRC)
aasm_out_elf32_la_LDFLAGS  = -module -avoid-version
aasm_out_elf32_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la
aasm_out_elf32_la_CFLAGS   = -I$(top_srcdir)/include

aasm_out_elf64_la_SOURCES  = $(SRC)
aasm_out_elf64_la_LDFLAGS  = -module -avoid-version
aasm_out_elf64_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la
aasm_out_elf64_la_CFLAGS   = -I$(top_srcdir)/include -DELF_64BITS

install-exec-hook:
	cd $(DESTDIR)$(libdir) ; rm -f aasm_default_out.so ; $(LN_S) aasm_out-elf32.so aasm_default_out.so
