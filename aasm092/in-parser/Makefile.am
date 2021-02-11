
lib_LTLIBRARIES = aasm_in-parser.la

aasm_in_parser_la_SOURCES = build_expr.c build_num.c directive.c		\
directive_align.c directive_data.c directive_ifdef.c					\
directive_include.c directive_macro.c directive_mod.c					\
directive_objinfo.c directive_proc.c directive_section.c				\
directive_symbols.c directive_warn_err.c keyword.c module.c operator.c	\
parse.c scan.c

aasm_in_parser_la_SOURCES += build_expr.h directive.h keyword.h	\
operator.h parse.h scan.h

aasm_in_parser_la_LDFLAGS  = -module -avoid-version
aasm_in_parser_la_LIBADD   = $(top_builddir)/libcore/libaasm_core.la

aasm_in_parser_la_CFLAGS = -I$(top_srcdir)/include

BUILT_SOURCES = directive.c operator.c

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

install-exec-hook:
	cd $(DESTDIR)$(libdir) ; rm -f aasm_default_in.so ; $(LN_S) aasm_in-parser.so aasm_default_in.so
