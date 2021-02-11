
lib_LTLIBRARIES = aasm_asm-x86.la

aasm_asm_x86_la_SOURCES = asm.c asm_help.c asm_mem.c asm_mem_table.c	\
asm_process.c asm_reloc.c asm_x86.c module.c asm_directive.c instr.c	\
reg.c asm.h asm_mem_table.h flags.h reg_id.h opcodes_id.h

aasm_asm_x86_la_LDFLAGS  = -module -avoid-version
aasm_asm_x86_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la
aasm_asm_x86_la_CFLAGS = -I$(top_srcdir)/include

CLEANFILES = opcodes_id.h asm_directive.c instr.c instr.perf instr.skel opcodes.h
BUILT_SOURCES = opcodes_id.h asm_directive.c instr.c reg.c

OPCOD_LISTS=opcodes_3dnow.list opcodes_3dnow2.list opcodes_fpu.list		\
opcodes_i386.list opcodes_i486.list opcodes_i586.list					\
opcodes_i686.list opcodes_mmx.list opcodes_mmx2.list opcodes_sse.list	\
opcodes_sse2.list

EXTRA_DIST = gen_opcodes.pl $(OPCOD_LISTS) instr.skel asm_directive.wlex reg.perf

%.c: %.perf
	if $(GPERF) `grep +options+ $< | cut -f 3- -d + ` \
	   $< >$@t; then \
	  mv $@t $@; \
	elif $(GPERF) --version >/dev/null 2>&1; then \
	  rm $@t; \
	  exit 1; \
	else \
	  rm $@t; \
	  touch $@; \
	fi

instr.perf opcodes_id.h: $(srcdir)/gen_opcodes.pl $(OPCOD_LISTS) instr.skel
	test -r instr.skel || cp $(filter %.skel,$^) .
	perl $< $(filter %.list,$^)

install-exec-hook:
	cd $(DESTDIR)$(libdir) ; rm -f aasm_default_asm.so ; $(LN_S) aasm_asm-x86.so aasm_default_asm.so

asm_directive.c: asm_directive.wlex $(top_srcdir)/build/word_lex.pl
	perl $(top_srcdir)/build/word_lex.pl $<
