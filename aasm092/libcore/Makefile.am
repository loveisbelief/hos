
lib_LTLIBRARIES = libaasm_core.la

libaasm_core_la_SOURCES = alloc.c comments.c disp.c error.c expr.c	\
expr_reduce.c instr.c location.c macro.c memory.c modules.c num.c	\
num_add.c num_cmp.c num_div.c num_logical.c num_mul.c num_neg.c		\
num_pow.c num_shift.c num_sub.c object.c oper_b_add.c oper_b_and.c	\
oper_b_div.c oper_b_eq.c oper_b_mod.c oper_b_mul.c oper_b_or.c		\
oper_b_pow.c oper_b_shl.c oper_b_shr.c oper_b_sub.c oper_b_xor.c	\
oper_parentheses.c oper_u_brack.c oper_u_neg.c oper_u_not.c		\
oper_u_plus.c oper_u_size.c proc.c refcount.c reloc.c scope.c		\
section.c size.c string.c symbol.c

libaasm_core_la_CFLAGS = -I$(top_srcdir)/include -DAASM_PREFIX=\"${prefix}\"
