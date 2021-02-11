
lib_LTLIBRARIES = aasm_asm-sparc.la

aasm_asm_sparc_la_SOURCES = asm.c asm_help.c asm_process.c asm_reloc.c	\
module.c asm_directive.c instr.c reg.c asm.h fields.h opcodes_id.h

aasm_asm_sparc_la_LDFLAGS  = -module -avoid-version
aasm_asm_sparc_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la

aasm_asm_sparc_la_CFLAGS = -I$(top_srcdir)/include

CLEANFILES = opcodes_id.h asm_directive.c instr.c reg.c instr.perf instr.skel opcodes.h
BUILT_SOURCES = opcodes_id.h asm_directive.c instr.c reg.c

OPCOD_LISTS=opcodes_v8_arith.list opcodes_v8_branch.list			\
opcodes_v8_float.list opcodes_v8_logical.list opcodes_v8_misc.list	\
opcodes_v8_storeload.list opcodes_v9.list

EXTRA_DIST = gen_opcodes.pl $(OPCOD_LISTS) instr.skel asm_directive.wlex reg.perf inc/v8.def

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

asm_directive.c: asm_directive.wlex $(top_srcdir)/build/word_lex.pl
	perl $(top_srcdir)/build/word_lex.pl $<

install-data-hook:
	mkdir -p $(DESTDIR)$(prefix)/include/aasm/sparc
	cp $(srcdir)/inc/*.def $(DESTDIR)$(prefix)/include/aasm/sparc

uninstall-hook:
	rm -rf $(DESTDIR)$(prefix)/include/aasm/sparc

