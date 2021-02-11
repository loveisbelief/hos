
AC_INIT(aasm, 0.9.2)

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])

AM_INIT_AUTOMAKE(aasm, 0.9.2)
AC_CONFIG_HEADERS(config.h)

AC_CHECK_PROGS(GPERF, gperf, [no])
if test $GPERF = "no"; then
       AC_MSG_ERROR([gperf tool must be available to build aasm])
fi

AC_CHECK_PROGS(PERL, perl, [no])
if test $PERL = "no"; then
       AC_MSG_ERROR([perl interpreter must be available to build aasm])
fi

AC_DISABLE_STATIC

AC_PROG_LN_S
AM_PATH_LISPDIR

AC_LANG(C)
AC_PROG_LIBTOOL

AC_CONFIG_FILES([
    Makefile
    libcore/Makefile
    aasm/Makefile
    doc/Makefile
    examples/Makefile
    extra/Makefile
    asm-sparc/Makefile
    include/Makefile
    include/aasm/Makefile
    asm-x86/Makefile
    in-parser/Makefile
    out-binary/Makefile
    out-coff/Makefile
    out-elf/Makefile
    out-intelhex/Makefile
    out-src/Makefile
])

CFLAGS+=" -Wall -Wno-unused-parameter "
#LDFLAGS+=" -release 0.9.2"

libdir="${libdir}/aasm"

AC_OUTPUT

