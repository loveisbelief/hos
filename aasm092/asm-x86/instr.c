/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -N asm_x86_get_instr_perf -L ANSI-C -k'*' -c -D -t instr.perf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "instr.perf"
 /* -*- c -*- */

/*
**    This file is part of AASM.
**
**    AASM is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
**    AASM is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with AASM; if not, write to the Free Software
**    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**    Copyright Alexandre Becoulet, 2002
**    contact : alexandre.becoulet@free.fr
*/

/*
+options+ -N asm_x86_get_instr_perf -L ANSI-C -k* -c -D -t
*/

#include <string.h>

#include "aasm/types.h"
#include "aasm/instr.h"

#include "asm.h"
#include "flags.h"

#include "opcodes_id.h"
#include "opcodes.h"

static unsigned int	hash(const char		*str,
			     unsigned int	len);

struct instr_perf_s	*asm_x86_get_instr_perf(const char	*str,
						unsigned int	len);

#line 46 "instr.perf"
struct instr_perf_s
;

#define TOTAL_KEYWORDS 586
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 11
#define MIN_HASH_VALUE 18
#define MAX_HASH_VALUE 4602
/* maximum key range = 4585, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,    5,    0,
       675,    0,    5,    5, 4603,    5,    5, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603,  750,  995,  100,
         5,   10,   15,  985,  260,  525,  860,  185,  520,   45,
       105,   25,   20,  225,  350,    0,   40,   25,  365,   45,
       300,    0, 1319, 1509,    0,  325, 4603,    0, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603, 4603,
      4603
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]+3];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]+1];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]+5];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct instr_perf_s *
asm_x86_get_instr_perf (register const char *str, register unsigned int len)
{
  static struct instr_perf_s wordlist[] =
    {
#line 544 "instr.perf"
      {"fst", opcodes_fst},
#line 499 "instr.perf"
      {"setc", opcodes_setc},
#line 354 "instr.perf"
      {"sete", opcodes_sete},
#line 84 "instr.perf"
      {"seto", opcodes_seto},
#line 78 "instr.perf"
      {"setnp", opcodes_setnp},
#line 388 "instr.perf"
      {"setna", opcodes_setna},
#line 309 "instr.perf"
      {"setnb", opcodes_setnb},
#line 425 "instr.perf"
      {"out", opcodes_out},
#line 485 "instr.perf"
      {"sets", opcodes_sets},
#line 528 "instr.perf"
      {"setnc", opcodes_setnc},
#line 162 "instr.perf"
      {"setnae", opcodes_setnae},
#line 253 "instr.perf"
      {"setl", opcodes_setl},
#line 395 "instr.perf"
      {"setnbe", opcodes_setnbe},
#line 443 "instr.perf"
      {"setnl", opcodes_setnl},
#line 601 "instr.perf"
      {"pop", opcodes_pop},
#line 235 "instr.perf"
      {"pfacc", opcodes_pfacc},
#line 249 "instr.perf"
      {"setnle", opcodes_setnle},
#line 352 "instr.perf"
      {"str", opcodes_str},
#line 413 "instr.perf"
      {"pfrcp", opcodes_pfrcp},
#line 66 "instr.perf"
      {"por", opcodes_por},
#line 63 "instr.perf"
      {"outsb", opcodes_outsb},
#line 589 "instr.perf"
      {"psadbw", opcodes_psadbw},
#line 82 "instr.perf"
      {"setb", opcodes_setb},
#line 579 "instr.perf"
      {"fprem", opcodes_fprem},
#line 145 "instr.perf"
      {"fprem1", opcodes_fprem1},
#line 561 "instr.perf"
      {"not", opcodes_not},
#line 595 "instr.perf"
      {"stosb", opcodes_stosb},
#line 325 "instr.perf"
      {"sti", opcodes_sti},
#line 570 "instr.perf"
      {"fsin", opcodes_fsin},
#line 79 "instr.perf"
      {"psrlq", opcodes_psrlq},
#line 471 "instr.perf"
      {"nop", opcodes_nop},
#line 462 "instr.perf"
      {"enter", opcodes_enter},
#line 384 "instr.perf"
      {"scasb", opcodes_scasb},
#line 626 "instr.perf"
      {"cmp", opcodes_cmp},
#line 532 "instr.perf"
      {"fdiv", opcodes_fdiv},
#line 419 "instr.perf"
      {"fdivp", opcodes_fdivp},
#line 492 "instr.perf"
      {"fpatan", opcodes_fpatan},
#line 102 "instr.perf"
      {"fsincos", opcodes_fsincos},
#line 431 "instr.perf"
      {"fcos", opcodes_fcos},
#line 219 "instr.perf"
      {"fdivr", opcodes_fdivr},
#line 324 "instr.perf"
      {"fchs", opcodes_fchs},
#line 233 "instr.perf"
      {"fdivrp", opcodes_fdivrp},
#line 85 "instr.perf"
      {"cmpsb", opcodes_cmpsb},
#line 190 "instr.perf"
      {"cmpordss", opcodes_cmpordss},
#line 196 "instr.perf"
      {"cmpeqss", opcodes_cmpeqss},
#line 460 "instr.perf"
      {"cmpordsd", opcodes_cmpordsd},
#line 232 "instr.perf"
      {"cmovp", opcodes_cmovp},
#line 153 "instr.perf"
      {"cmpeqsd", opcodes_cmpeqsd},
#line 399 "instr.perf"
      {"cmpnless", opcodes_cmpnless},
#line 630 "instr.perf"
      {"setp", opcodes_setp},
#line 302 "instr.perf"
      {"cmova", opcodes_cmova},
#line 201 "instr.perf"
      {"cmpnlesd", opcodes_cmpnlesd},
#line 174 "instr.perf"
      {"fstp", opcodes_fstp},
#line 627 "instr.perf"
      {"cmovb", opcodes_cmovb},
#line 291 "instr.perf"
      {"cmovpe", opcodes_cmovpe},
#line 361 "instr.perf"
      {"cmpordps", opcodes_cmpordps},
#line 552 "instr.perf"
      {"cmovc", opcodes_cmovc},
#line 55 "instr.perf"
      {"cmovae", opcodes_cmovae},
#line 447 "instr.perf"
      {"cmpeqps", opcodes_cmpeqps},
#line 498 "instr.perf"
      {"cmpordpd", opcodes_cmpordpd},
#line 526 "instr.perf"
      {"cmovbe", opcodes_cmovbe},
#line 382 "instr.perf"
      {"cmpeqpd", opcodes_cmpeqpd},
#line 598 "instr.perf"
      {"cmpnleps", opcodes_cmpnleps},
#line 52 "instr.perf"
      {"cmovl", opcodes_cmovl},
#line 306 "instr.perf"
      {"cmovpo", opcodes_cmovpo},
#line 67 "instr.perf"
      {"cmpnlepd", opcodes_cmpnlepd},
#line 137 "instr.perf"
      {"fcom", opcodes_fcom},
#line 59 "instr.perf"
      {"fcomp", opcodes_fcomp},
#line 347 "instr.perf"
      {"cmpltss", opcodes_cmpltss},
#line 186 "instr.perf"
      {"cmpnltss", opcodes_cmpnltss},
#line 226 "instr.perf"
      {"cmovle", opcodes_cmovle},
#line 146 "instr.perf"
      {"cmpltsd", opcodes_cmpltsd},
#line 110 "instr.perf"
      {"cmpnltsd", opcodes_cmpnltsd},
#line 572 "instr.perf"
      {"setg", opcodes_setg},
#line 297 "instr.perf"
      {"dec", opcodes_dec},
#line 210 "instr.perf"
      {"fcompp", opcodes_fcompp},
#line 516 "instr.perf"
      {"cmpltps", opcodes_cmpltps},
#line 374 "instr.perf"
      {"cmpnltps", opcodes_cmpnltps},
#line 504 "instr.perf"
      {"cmpltpd", opcodes_cmpltpd},
#line 346 "instr.perf"
      {"cmpnltpd", opcodes_cmpnltpd},
#line 295 "instr.perf"
      {"pfpnacc", opcodes_pfpnacc},
#line 256 "instr.perf"
      {"mul", opcodes_mul},
#line 594 "instr.perf"
      {"setne", opcodes_setne},
#line 410 "instr.perf"
      {"sqrtps", opcodes_sqrtps},
#line 351 "instr.perf"
      {"stc", opcodes_stc},
#line 517 "instr.perf"
      {"sqrtpd", opcodes_sqrtpd},
#line 206 "instr.perf"
      {"shr", opcodes_shr},
#line 429 "instr.perf"
      {"shrd", opcodes_shrd},
#line 265 "instr.perf"
      {"setle", opcodes_setle},
#line 92 "instr.perf"
      {"psllq", opcodes_psllq},
#line 208 "instr.perf"
      {"fsave", opcodes_fsave},
#line 543 "instr.perf"
      {"fucom", opcodes_fucom},
#line 375 "instr.perf"
      {"ffree", opcodes_ffree},
#line 342 "instr.perf"
      {"ret", opcodes_ret},
#line 263 "instr.perf"
      {"pxor", opcodes_pxor},
#line 54 "instr.perf"
      {"setnz", opcodes_setnz},
#line 632 "instr.perf"
      {"fucomp", opcodes_fucomp},
#line 519 "instr.perf"
      {"xor", opcodes_xor},
#line 529 "instr.perf"
      {"setbe", opcodes_setbe},
#line 315 "instr.perf"
      {"or", opcodes_or},
#line 160 "instr.perf"
      {"test", opcodes_test},
#line 534 "instr.perf"
      {"ftst", opcodes_ftst},
#line 363 "instr.perf"
      {"fucompp", opcodes_fucompp},
#line 522 "instr.perf"
      {"rep", opcodes_rep},
#line 436 "instr.perf"
      {"fnop", opcodes_fnop},
#line 149 "instr.perf"
      {"setno", opcodes_setno},
#line 170 "instr.perf"
      {"repe", opcodes_repe},
#line 296 "instr.perf"
      {"setns", opcodes_setns},
#line 77 "instr.perf"
      {"cmc", opcodes_cmc},
#line 72 "instr.perf"
      {"orpd", opcodes_orpd},
#line 290 "instr.perf"
      {"rdtsc", opcodes_rdtsc},
#line 386 "instr.perf"
      {"ror", opcodes_ror},
#line 106 "instr.perf"
      {"verr", opcodes_verr},
#line 114 "instr.perf"
      {"fsqrt", opcodes_fsqrt},
#line 345 "instr.perf"
      {"fxam", opcodes_fxam},
#line 617 "instr.perf"
      {"orps", opcodes_orps},
#line 509 "instr.perf"
      {"emms", opcodes_emms},
#line 409 "instr.perf"
      {"pextrw", opcodes_pextrw},
#line 258 "instr.perf"
      {"cmovns", opcodes_cmovns},
#line 203 "instr.perf"
      {"fxtract", opcodes_fxtract},
#line 65 "instr.perf"
      {"cmovne", opcodes_cmovne},
#line 262 "instr.perf"
      {"cdq", opcodes_cdq},
#line 169 "instr.perf"
      {"cmovnp", opcodes_cmovnp},
#line 118 "instr.perf"
      {"cmpless", opcodes_cmpless},
#line 588 "instr.perf"
      {"cmovno", opcodes_cmovno},
#line 412 "instr.perf"
      {"cmplesd", opcodes_cmplesd},
#line 602 "instr.perf"
      {"shl", opcodes_shl},
#line 480 "instr.perf"
      {"cmove", opcodes_cmove},
#line 358 "instr.perf"
      {"pmaxsw", opcodes_pmaxsw},
#line 531 "instr.perf"
      {"rcr", opcodes_rcr},
#line 487 "instr.perf"
      {"shld", opcodes_shld},
#line 355 "instr.perf"
      {"setpe", opcodes_setpe},
#line 310 "instr.perf"
      {"fcmove", opcodes_fcmove},
#line 403 "instr.perf"
      {"cmpleps", opcodes_cmpleps},
#line 300 "instr.perf"
      {"rdpmc", opcodes_rdpmc},
#line 316 "instr.perf"
      {"cmplepd", opcodes_cmplepd},
#line 525 "instr.perf"
      {"fnstcw", opcodes_fnstcw},
#line 56 "instr.perf"
      {"fcmovu", opcodes_fcmovu},
#line 607 "instr.perf"
      {"setge", opcodes_setge},
#line 95 "instr.perf"
      {"lea", opcodes_lea},
#line 298 "instr.perf"
      {"unpckhps", opcodes_unpckhps},
#line 457 "instr.perf"
      {"pfrsqrt", opcodes_pfrsqrt},
#line 188 "instr.perf"
      {"unpckhpd", opcodes_unpckhpd},
#line 563 "instr.perf"
      {"cmovz", opcodes_cmovz},
#line 475 "instr.perf"
      {"cmovnc", opcodes_cmovnc},
#line 593 "instr.perf"
      {"std", opcodes_std},
#line 396 "instr.perf"
      {"cmpss", opcodes_cmpss},
#line 348 "instr.perf"
      {"cmovo", opcodes_cmovo},
#line 74 "instr.perf"
      {"setpo", opcodes_setpo},
#line 241 "instr.perf"
      {"oio", opcodes_oio},
#line 139 "instr.perf"
      {"cmovs", opcodes_cmovs},
#line 489 "instr.perf"
      {"rol", opcodes_rol},
#line 596 "instr.perf"
      {"sysret", opcodes_sysret},
#line 568 "instr.perf"
      {"fcmovne", opcodes_fcmovne},
#line 501 "instr.perf"
      {"ltr", opcodes_ltr},
#line 179 "instr.perf"
      {"pminsw", opcodes_pminsw},
#line 397 "instr.perf"
      {"fcmovnu", opcodes_fcmovnu},
#line 536 "instr.perf"
      {"fstenv", opcodes_fstenv},
#line 81 "instr.perf"
      {"in", opcodes_in},
#line 140 "instr.perf"
      {"int", opcodes_int},
#line 283 "instr.perf"
      {"neg", opcodes_neg},
#line 90 "instr.perf"
      {"int3", opcodes_int3},
#line 144 "instr.perf"
      {"pfcmpeq", opcodes_pfcmpeq},
#line 557 "instr.perf"
      {"smsw", opcodes_smsw},
#line 143 "instr.perf"
      {"into", opcodes_into},
#line 272 "instr.perf"
      {"pfrcpit1", opcodes_pfrcpit1},
#line 129 "instr.perf"
      {"pinsrw", opcodes_pinsrw},
#line 111 "instr.perf"
      {"clts", opcodes_clts},
#line 439 "instr.perf"
      {"sqrtss", opcodes_sqrtss},
#line 496 "instr.perf"
      {"sqrtsd", opcodes_sqrtsd},
#line 125 "instr.perf"
      {"cwd", opcodes_cwd},
#line 494 "instr.perf"
      {"repne", opcodes_repne},
#line 136 "instr.perf"
      {"rcl", opcodes_rcl},
#line 551 "instr.perf"
      {"idiv", opcodes_idiv},
#line 268 "instr.perf"
      {"cmpneqss", opcodes_cmpneqss},
#line 540 "instr.perf"
      {"cwde", opcodes_cwde},
#line 373 "instr.perf"
      {"cmpneqsd", opcodes_cmpneqsd},
#line 115 "instr.perf"
      {"rsm", opcodes_rsm},
#line 391 "instr.perf"
      {"mulss", opcodes_mulss},
#line 611 "instr.perf"
      {"cmpneqps", opcodes_cmpneqps},
#line 331 "instr.perf"
      {"xchg", opcodes_xchg},
#line 541 "instr.perf"
      {"cmpneqpd", opcodes_cmpneqpd},
#line 470 "instr.perf"
      {"pfsub", opcodes_pfsub},
#line 128 "instr.perf"
      {"verw", opcodes_verw},
#line 98 "instr.perf"
      {"cli", opcodes_cli},
#line 261 "instr.perf"
      {"pfrsqit1", opcodes_pfrsqit1},
#line 608 "instr.perf"
      {"repnz", opcodes_repnz},
#line 230 "instr.perf"
      {"lsl", opcodes_lsl},
#line 191 "instr.perf"
      {"stmxcsr", opcodes_stmxcsr},
#line 275 "instr.perf"
      {"cmpps", opcodes_cmpps},
#line 537 "instr.perf"
      {"rsqrtss", opcodes_rsqrtss},
#line 472 "instr.perf"
      {"daa", opcodes_daa},
#line 294 "instr.perf"
      {"rdmsr", opcodes_rdmsr},
#line 109 "instr.perf"
      {"fild", opcodes_fild},
#line 195 "instr.perf"
      {"pfmul", opcodes_pfmul},
#line 243 "instr.perf"
      {"hlt", opcodes_hlt},
#line 619 "instr.perf"
      {"pand", opcodes_pand},
#line 217 "instr.perf"
      {"fcomi", opcodes_fcomi},
#line 631 "instr.perf"
      {"rsqrtps", opcodes_rsqrtps},
#line 538 "instr.perf"
      {"punpckhwd", opcodes_punpckhwd},
#line 134 "instr.perf"
      {"ucomiss", opcodes_ucomiss},
#line 234 "instr.perf"
      {"sar", opcodes_sar},
#line 121 "instr.perf"
      {"ucomisd", opcodes_ucomisd},
#line 237 "instr.perf"
      {"fcomip", opcodes_fcomip},
#line 468 "instr.perf"
      {"unpcklps", opcodes_unpcklps},
#line 202 "instr.perf"
      {"loop", opcodes_loop},
#line 334 "instr.perf"
      {"wrmsr", opcodes_wrmsr},
#line 250 "instr.perf"
      {"fninit", opcodes_fninit},
#line 158 "instr.perf"
      {"unpcklpd", opcodes_unpcklpd},
#line 451 "instr.perf"
      {"lss", opcodes_lss},
#line 197 "instr.perf"
      {"fxrstor", opcodes_fxrstor},
#line 328 "instr.perf"
      {"lds", opcodes_lds},
#line 378 "instr.perf"
      {"les", opcodes_les},
#line 86 "instr.perf"
      {"lfs", opcodes_lfs},
#line 530 "instr.perf"
      {"ficom", opcodes_ficom},
#line 488 "instr.perf"
      {"femms", opcodes_femms},
#line 433 "instr.perf"
      {"leave", opcodes_leave},
#line 576 "instr.perf"
      {"fnstsw", opcodes_fnstsw},
#line 387 "instr.perf"
      {"js", opcodes_js},
#line 200 "instr.perf"
      {"xlat", opcodes_xlat},
#line 215 "instr.perf"
      {"ficomp", opcodes_ficomp},
#line 96 "instr.perf"
      {"fist", opcodes_fist},
#line 605 "instr.perf"
      {"fistp", opcodes_fistp},
#line 135 "instr.perf"
      {"fucomi", opcodes_fucomi},
#line 603 "instr.perf"
      {"je", opcodes_je},
#line 245 "instr.perf"
      {"push", opcodes_push},
#line 600 "instr.perf"
      {"pusha", opcodes_pusha},
#line 177 "instr.perf"
      {"jp", opcodes_jp},
#line 616 "instr.perf"
      {"clc", opcodes_clc},
#line 141 "instr.perf"
      {"rcpss", opcodes_rcpss},
#line 604 "instr.perf"
      {"pushad", opcodes_pushad},
#line 108 "instr.perf"
      {"jo", opcodes_jo},
#line 620 "instr.perf"
      {"mulps", opcodes_mulps},
#line 322 "instr.perf"
      {"fucomip", opcodes_fucomip},
#line 582 "instr.perf"
      {"inc", opcodes_inc},
#line 147 "instr.perf"
      {"setng", opcodes_setng},
#line 427 "instr.perf"
      {"segcs", opcodes_segcs},
#line 402 "instr.perf"
      {"segds", opcodes_segds},
#line 251 "instr.perf"
      {"setnge", opcodes_setnge},
#line 266 "instr.perf"
      {"seges", opcodes_seges},
#line 101 "instr.perf"
      {"jpo", opcodes_jpo},
#line 73 "instr.perf"
      {"wait", opcodes_wait},
#line 465 "instr.perf"
      {"sfence", opcodes_sfence},
#line 80 "instr.perf"
      {"jmp", opcodes_jmp},
#line 463 "instr.perf"
      {"segss", opcodes_segss},
#line 520 "instr.perf"
      {"insd", opcodes_insd},
#line 422 "instr.perf"
      {"sysenter", opcodes_sysenter},
#line 260 "instr.perf"
      {"jc", opcodes_jc},
#line 577 "instr.perf"
      {"xorps", opcodes_xorps},
#line 62 "instr.perf"
      {"pshufd", opcodes_pshufd},
#line 556 "instr.perf"
      {"jnz", opcodes_jnz},
#line 621 "instr.perf"
      {"punpckhdq", opcodes_punpckhdq},
#line 426 "instr.perf"
      {"mfence", opcodes_mfence},
#line 344 "instr.perf"
      {"fincstp", opcodes_fincstp},
#line 369 "instr.perf"
      {"sal", opcodes_sal},
#line 247 "instr.perf"
      {"minss", opcodes_minss},
#line 127 "instr.perf"
      {"cmovnl", opcodes_cmovnl},
#line 212 "instr.perf"
      {"jna", opcodes_jna},
#line 299 "instr.perf"
      {"fwait", opcodes_fwait},
#line 618 "instr.perf"
      {"fnclex", opcodes_fnclex},
#line 599 "instr.perf"
      {"cmovnle", opcodes_cmovnle},
#line 423 "instr.perf"
      {"jnp", opcodes_jnp},
#line 506 "instr.perf"
      {"jnae", opcodes_jnae},
#line 359 "instr.perf"
      {"pshufw", opcodes_pshufw},
#line 368 "instr.perf"
      {"jno", opcodes_jno},
#line 613 "instr.perf"
      {"seta", opcodes_seta},
#line 432 "instr.perf"
      {"pandn", opcodes_pandn},
#line 629 "instr.perf"
      {"sub", opcodes_sub},
#line 505 "instr.perf"
      {"adc", opcodes_adc},
#line 254 "instr.perf"
      {"pswapd", opcodes_pswapd},
#line 586 "instr.perf"
      {"insb", opcodes_insb},
#line 224 "instr.perf"
      {"bt", opcodes_bt},
#line 592 "instr.perf"
      {"wb", opcodes_wb},
#line 273 "instr.perf"
      {"bsr", opcodes_bsr},
#line 341 "instr.perf"
      {"frndint", opcodes_frndint},
#line 155 "instr.perf"
      {"punpcklwd", opcodes_punpcklwd},
#line 408 "instr.perf"
      {"pfadd", opcodes_pfadd},
#line 330 "instr.perf"
      {"loopne", opcodes_loopne},
#line 68 "instr.perf"
      {"das", opcodes_das},
#line 239 "instr.perf"
      {"popf", opcodes_popf},
#line 339 "instr.perf"
      {"fld", opcodes_fld},
#line 222 "instr.perf"
      {"pfsubr", opcodes_pfsubr},
#line 107 "instr.perf"
      {"popa", opcodes_popa},
#line 469 "instr.perf"
      {"rcpps", opcodes_rcpps},
#line 381 "instr.perf"
      {"sldt", opcodes_sldt},
#line 192 "instr.perf"
      {"loope", opcodes_loope},
#line 473 "instr.perf"
      {"sidt", opcodes_sidt},
#line 75 "instr.perf"
      {"outsd", opcodes_outsd},
#line 335 "instr.perf"
      {"btr", opcodes_btr},
#line 223 "instr.perf"
      {"cmovg", opcodes_cmovg},
#line 329 "instr.perf"
      {"cbw", opcodes_cbw},
#line 178 "instr.perf"
      {"psrld", opcodes_psrld},
#line 461 "instr.perf"
      {"fxch", opcodes_fxch},
#line 175 "instr.perf"
      {"cmovge", opcodes_cmovge},
#line 130 "instr.perf"
      {"stosd", opcodes_stosd},
#line 481 "instr.perf"
      {"call", opcodes_call},
#line 547 "instr.perf"
      {"lodsb", opcodes_lodsb},
#line 228 "instr.perf"
      {"fldl2e", opcodes_fldl2e},
#line 437 "instr.perf"
      {"f2xm1", opcodes_f2xm1},
#line 242 "instr.perf"
      {"pmaddwd", opcodes_pmaddwd},
#line 633 "instr.perf"
      {"loopz", opcodes_loopz},
#line 518 "instr.perf"
      {"scasd", opcodes_scasd},
#line 327 "instr.perf"
      {"cld", opcodes_cld},
#line 466 "instr.perf"
      {"cmpunordss", opcodes_cmpunordss},
#line 336 "instr.perf"
      {"pfnacc", opcodes_pfnacc},
#line 574 "instr.perf"
      {"cmpunordsd", opcodes_cmpunordsd},
#line 58 "instr.perf"
      {"fldl2t", opcodes_fldl2t},
#line 199 "instr.perf"
      {"seggs", opcodes_seggs},
#line 308 "instr.perf"
      {"minps", opcodes_minps},
#line 440 "instr.perf"
      {"lmsw", opcodes_lmsw},
#line 225 "instr.perf"
      {"cmpunordps", opcodes_cmpunordps},
#line 198 "instr.perf"
      {"arpl", opcodes_arpl},
#line 406 "instr.perf"
      {"cmpunordpd", opcodes_cmpunordpd},
#line 353 "instr.perf"
      {"wbinvd", opcodes_wbinvd},
#line 535 "instr.perf"
      {"fnstenv", opcodes_fnstenv},
#line 142 "instr.perf"
      {"bsf", opcodes_bsf},
#line 319 "instr.perf"
      {"pcmpeqd", opcodes_pcmpeqd},
#line 216 "instr.perf"
      {"jnl", opcodes_jnl},
#line 194 "instr.perf"
      {"punpckhqdq", opcodes_punpckhqdq},
#line 204 "instr.perf"
      {"cmpsd", opcodes_cmpsd},
#line 303 "instr.perf"
      {"jnle", opcodes_jnle},
#line 304 "instr.perf"
      {"cmovna", opcodes_cmovna},
#line 289 "instr.perf"
      {"cvtdq2ps", opcodes_cvtdq2ps},
#line 313 "instr.perf"
      {"fisub", opcodes_fisub},
#line 482 "instr.perf"
      {"cmovnae", opcodes_cmovnae},
#line 495 "instr.perf"
      {"cvtdq2pd", opcodes_cvtdq2pd},
#line 445 "instr.perf"
      {"jnc", opcodes_jnc},
#line 280 "instr.perf"
      {"punpckldq", opcodes_punpckldq},
#line 464 "instr.perf"
      {"pcmpeqw", opcodes_pcmpeqw},
#line 50 "instr.perf"
      {"insw", opcodes_insw},
#line 450 "instr.perf"
      {"ldmxcsr", opcodes_ldmxcsr},
#line 376 "instr.perf"
      {"fbld", opcodes_fbld},
#line 380 "instr.perf"
      {"fptan", opcodes_fptan},
#line 545 "instr.perf"
      {"pshufhw", opcodes_pshufhw},
#line 441 "instr.perf"
      {"jns", opcodes_jns},
#line 76 "instr.perf"
      {"setae", opcodes_setae},
#line 459 "instr.perf"
      {"pslld", opcodes_pslld},
#line 394 "instr.perf"
      {"add", opcodes_add},
#line 385 "instr.perf"
      {"fimul", opcodes_fimul},
#line 591 "instr.perf"
      {"btc", opcodes_btc},
#line 49 "instr.perf"
      {"fadd", opcodes_fadd},
#line 189 "instr.perf"
      {"faddp", opcodes_faddp},
#line 559 "instr.perf"
      {"fscale", opcodes_fscale},
#line 117 "instr.perf"
      {"fdecstp", opcodes_fdecstp},
#line 193 "instr.perf"
      {"lar", opcodes_lar},
#line 94 "instr.perf"
      {"paddb", opcodes_paddb},
#line 404 "instr.perf"
      {"mulsd", opcodes_mulsd},
#line 244 "instr.perf"
      {"psrldq", opcodes_psrldq},
#line 453 "instr.perf"
      {"lock", opcodes_lock},
#line 259 "instr.perf"
      {"pfrcpit2", opcodes_pfrcpit2},
#line 278 "instr.perf"
      {"cflush", opcodes_cflush},
#line 383 "instr.perf"
      {"bts", opcodes_bts},
#line 414 "instr.perf"
      {"fbstp", opcodes_fbstp},
#line 350 "instr.perf"
      {"fstcw", opcodes_fstcw},
#line 150 "instr.perf"
      {"retf", opcodes_retf},
#line 583 "instr.perf"
      {"paddq", opcodes_paddq},
#line 444 "instr.perf"
      {"syscall", opcodes_syscall},
#line 269 "instr.perf"
      {"pmovmskb", opcodes_pmovmskb},
#line 401 "instr.perf"
      {"fstsw", opcodes_fstsw},
#line 307 "instr.perf"
      {"pmaxub", opcodes_pmaxub},
#line 205 "instr.perf"
      {"jl", opcodes_jl},
#line 218 "instr.perf"
      {"and", opcodes_and},
#line 105 "instr.perf"
      {"cmppd", opcodes_cmppd},
#line 138 "instr.perf"
      {"frstor", opcodes_frstor},
#line 477 "instr.perf"
      {"pushf", opcodes_pushf},
#line 634 "instr.perf"
      {"pfcmpge", opcodes_pfcmpge},
#line 159 "instr.perf"
      {"cvttps2dq", opcodes_cvttps2dq},
#line 571 "instr.perf"
      {"cvttpd2dq", opcodes_cvttpd2dq},
#line 156 "instr.perf"
      {"andnps", opcodes_andnps},
#line 615 "instr.perf"
      {"outsw", opcodes_outsw},
#line 119 "instr.perf"
      {"andnpd", opcodes_andnpd},
#line 458 "instr.perf"
      {"subss", opcodes_subss},
#line 370 "instr.perf"
      {"fmul", opcodes_fmul},
#line 418 "instr.perf"
      {"fmulp", opcodes_fmulp},
#line 558 "instr.perf"
      {"fnsave", opcodes_fnsave},
#line 252 "instr.perf"
      {"pfcmpgt", opcodes_pfcmpgt},
#line 184 "instr.perf"
      {"psrlw", opcodes_psrlw},
#line 286 "instr.perf"
      {"cmpxchg", opcodes_cmpxchg},
#line 236 "instr.perf"
      {"fsub", opcodes_fsub},
#line 164 "instr.perf"
      {"fsubp", opcodes_fsubp},
#line 167 "instr.perf"
      {"stosw", opcodes_stosw},
#line 154 "instr.perf"
      {"cmovng", opcodes_cmovng},
#line 163 "instr.perf"
      {"finit", opcodes_finit},
#line 103 "instr.perf"
      {"lfence", opcodes_lfence},
#line 214 "instr.perf"
      {"psubb", opcodes_psubb},
#line 267 "instr.perf"
      {"punpcklqdq", opcodes_punpcklqdq},
#line 356 "instr.perf"
      {"cmovnb", opcodes_cmovnb},
#line 597 "instr.perf"
      {"cmovnge", opcodes_cmovnge},
#line 132 "instr.perf"
      {"jecxz", opcodes_jecxz},
#line 442 "instr.perf"
      {"fsubr", opcodes_fsubr},
#line 372 "instr.perf"
      {"cmovnbe", opcodes_cmovnbe},
#line 515 "instr.perf"
      {"pfmin", opcodes_pfmin},
#line 340 "instr.perf"
      {"scasw", opcodes_scasw},
#line 542 "instr.perf"
      {"psubq", opcodes_psubq},
#line 124 "instr.perf"
      {"fsubrp", opcodes_fsubrp},
#line 549 "instr.perf"
      {"fcmovb", opcodes_fcmovb},
#line 165 "instr.perf"
      {"jng", opcodes_jng},
#line 377 "instr.perf"
      {"pminub", opcodes_pminub},
#line 389 "instr.perf"
      {"fcmovbe", opcodes_fcmovbe},
#line 274 "instr.perf"
      {"jnge", opcodes_jnge},
#line 180 "instr.perf"
      {"pmullw", opcodes_pmullw},
#line 51 "instr.perf"
      {"pslldq", opcodes_pslldq},
#line 87 "instr.perf"
      {"mulpd", opcodes_mulpd},
#line 479 "instr.perf"
      {"aaa", opcodes_aaa},
#line 438 "instr.perf"
      {"setz", opcodes_setz},
#line 610 "instr.perf"
      {"pshuflw", opcodes_pshuflw},
#line 176 "instr.perf"
      {"maxss", opcodes_maxss},
#line 221 "instr.perf"
      {"cmpsw", opcodes_cmpsw},
#line 490 "instr.perf"
      {"sgdt", opcodes_sgdt},
#line 452 "instr.perf"
      {"fiadd", opcodes_fiadd},
#line 116 "instr.perf"
      {"cvtss2sd", opcodes_cvtss2sd},
#line 448 "instr.perf"
      {"fisubr", opcodes_fisubr},
#line 88 "instr.perf"
      {"mov", opcodes_mov},
#line 312 "instr.perf"
      {"xorpd", opcodes_xorpd},
#line 507 "instr.perf"
      {"xadd", opcodes_xadd},
#line 69 "instr.perf"
      {"fcmovnb", opcodes_fcmovnb},
#line 474 "instr.perf"
      {"movd", opcodes_movd},
#line 231 "instr.perf"
      {"lldt", opcodes_lldt},
#line 523 "instr.perf"
      {"lidt", opcodes_lidt},
#line 100 "instr.perf"
      {"minsd", opcodes_minsd},
#line 314 "instr.perf"
      {"fcmovnbe", opcodes_fcmovnbe},
#line 61 "instr.perf"
      {"subps", opcodes_subps},
#line 211 "instr.perf"
      {"pcmpgtd", opcodes_pcmpgtd},
#line 337 "instr.perf"
      {"ja", opcodes_ja},
#line 365 "instr.perf"
      {"psllw", opcodes_psllw},
#line 622 "instr.perf"
      {"fxsave", opcodes_fxsave},
#line 434 "instr.perf"
      {"movlps", opcodes_movlps},
#line 97 "instr.perf"
      {"movsb", opcodes_movsb},
#line 284 "instr.perf"
      {"movlpd", opcodes_movlpd},
#line 271 "instr.perf"
      {"pcmpgtw", opcodes_pcmpgtw},
#line 181 "instr.perf"
      {"callf", opcodes_callf},
#line 71 "instr.perf"
      {"paddusw", opcodes_paddusw},
#line 546 "instr.perf"
      {"movdqu", opcodes_movdqu},
#line 364 "instr.perf"
      {"pmulhw", opcodes_pmulhw},
#line 172 "instr.perf"
      {"fldenv", opcodes_fldenv},
#line 338 "instr.perf"
      {"movntps", opcodes_movntps},
#line 99 "instr.perf"
      {"movntpd", opcodes_movntpd},
#line 486 "instr.perf"
      {"addss", opcodes_addss},
#line 533 "instr.perf"
      {"pmulhuw", opcodes_pmulhuw},
#line 93 "instr.perf"
      {"comiss", opcodes_comiss},
#line 91 "instr.perf"
      {"cvtsi2ss", opcodes_cvtsi2ss},
#line 321 "instr.perf"
      {"comisd", opcodes_comisd},
#line 282 "instr.perf"
      {"cvtsi2sd", opcodes_cvtsi2sd},
#line 587 "instr.perf"
      {"maxps", opcodes_maxps},
#line 512 "instr.perf"
      {"cvttps2pi", opcodes_cvttps2pi},
#line 398 "instr.perf"
      {"paddsw", opcodes_paddsw},
#line 612 "instr.perf"
      {"cvttpd2pi", opcodes_cvttpd2pi},
#line 456 "instr.perf"
      {"fld1", opcodes_fld1},
#line 305 "instr.perf"
      {"jpe", opcodes_jpe},
#line 567 "instr.perf"
      {"fyl2x", opcodes_fyl2x},
#line 625 "instr.perf"
      {"cvtps2pd", opcodes_cvtps2pd},
#line 317 "instr.perf"
      {"iret", opcodes_iret},
#line 104 "instr.perf"
      {"fyl2xp1", opcodes_fyl2xp1},
#line 57 "instr.perf"
      {"punpckhbw", opcodes_punpckhbw},
#line 240 "instr.perf"
      {"cmovnz", opcodes_cmovnz},
#line 446 "instr.perf"
      {"sahf", opcodes_sahf},
#line 207 "instr.perf"
      {"minpd", opcodes_minpd},
#line 584 "instr.perf"
      {"psubusw", opcodes_psubusw},
#line 332 "instr.perf"
      {"fabs", opcodes_fabs},
#line 288 "instr.perf"
      {"aas", opcodes_aas},
#line 623 "instr.perf"
      {"lgs", opcodes_lgs},
#line 182 "instr.perf"
      {"fldpi", opcodes_fldpi},
#line 606 "instr.perf"
      {"jne", opcodes_jne},
#line 566 "instr.perf"
      {"movlhps", opcodes_movlhps},
#line 554 "instr.perf"
      {"jg", opcodes_jg},
#line 454 "instr.perf"
      {"aam", opcodes_aam},
#line 379 "instr.perf"
      {"psubsw", opcodes_psubsw},
#line 417 "instr.perf"
      {"jb", opcodes_jb},
#line 255 "instr.perf"
      {"fclex", opcodes_fclex},
#line 285 "instr.perf"
      {"addps", opcodes_addps},
#line 220 "instr.perf"
      {"movntq", opcodes_movntq},
#line 276 "instr.perf"
      {"segfs", opcodes_segfs},
#line 503 "instr.perf"
      {"movntdq", opcodes_movntdq},
#line 400 "instr.perf"
      {"pf2id", opcodes_pf2id},
#line 53 "instr.perf"
      {"repz", opcodes_repz},
#line 421 "instr.perf"
      {"cvtpi2ps", opcodes_cvtpi2ps},
#line 292 "instr.perf"
      {"cvtpi2pd", opcodes_cvtpi2pd},
#line 152 "instr.perf"
      {"jmpf", opcodes_jmpf},
#line 133 "instr.perf"
      {"fidiv", opcodes_fidiv},
#line 113 "instr.perf"
      {"movq", opcodes_movq},
#line 60 "instr.perf"
      {"imul", opcodes_imul},
#line 524 "instr.perf"
      {"movups", opcodes_movups},
#line 122 "instr.perf"
      {"jnb", opcodes_jnb},
#line 624 "instr.perf"
      {"movupd", opcodes_movupd},
#line 183 "instr.perf"
      {"pmuludq", opcodes_pmuludq},
#line 555 "instr.perf"
      {"cvtps2dq", opcodes_cvtps2dq},
#line 277 "instr.perf"
      {"jnbe", opcodes_jnbe},
#line 424 "instr.perf"
      {"packssdw", opcodes_packssdw},
#line 420 "instr.perf"
      {"andps", opcodes_andps},
#line 209 "instr.perf"
      {"sbb", opcodes_sbb},
#line 131 "instr.perf"
      {"movss", opcodes_movss},
#line 527 "instr.perf"
      {"bswap", opcodes_bswap},
#line 411 "instr.perf"
      {"fldln2", opcodes_fldln2},
#line 333 "instr.perf"
      {"pmulhrw", opcodes_pmulhrw},
#line 362 "instr.perf"
      {"aad", opcodes_aad},
#line 483 "instr.perf"
      {"punpcklbw", opcodes_punpcklbw},
#line 578 "instr.perf"
      {"subsd", opcodes_subsd},
#line 392 "instr.perf"
      {"div", opcodes_div},
#line 628 "instr.perf"
      {"psrad", opcodes_psrad},
#line 187 "instr.perf"
      {"popad", opcodes_popad},
#line 497 "instr.perf"
      {"lgdt", opcodes_lgdt},
#line 500 "instr.perf"
      {"cvttss2si", opcodes_cvttss2si},
#line 343 "instr.perf"
      {"cvttsd2si", opcodes_cvttsd2si},
#line 575 "instr.perf"
      {"cvtss2si", opcodes_cvtss2si},
#line 416 "instr.perf"
      {"lodsd", opcodes_lodsd},
#line 287 "instr.perf"
      {"movhps", opcodes_movhps},
#line 320 "instr.perf"
      {"movhpd", opcodes_movhpd},
#line 112 "instr.perf"
      {"maxsd", opcodes_maxsd},
#line 89 "instr.perf"
      {"invd", opcodes_invd},
#line 246 "instr.perf"
      {"movhlps", opcodes_movhlps},
#line 449 "instr.perf"
      {"movq2dq", opcodes_movq2dq},
#line 569 "instr.perf"
      {"cvtsd2ss", opcodes_cvtsd2ss},
#line 415 "instr.perf"
      {"movnti", opcodes_movnti},
#line 371 "instr.perf"
      {"jz", opcodes_jz},
#line 455 "instr.perf"
      {"pcmpeqb", opcodes_pcmpeqb},
#line 270 "instr.perf"
      {"pf2iw", opcodes_pf2iw},
#line 318 "instr.perf"
      {"subpd", opcodes_subpd},
#line 511 "instr.perf"
      {"jle", opcodes_jle},
#line 161 "instr.perf"
      {"maskmovq", opcodes_maskmovq},
#line 126 "instr.perf"
      {"movmskps", opcodes_movmskps},
#line 564 "instr.perf"
      {"movmskpd", opcodes_movmskpd},
#line 366 "instr.perf"
      {"fidivr", opcodes_fidivr},
#line 157 "instr.perf"
      {"cvtps2pi", opcodes_cvtps2pi},
#line 478 "instr.perf"
      {"maskmovdqu", opcodes_maskmovdqu},
#line 548 "instr.perf"
      {"paddd", opcodes_paddd},
#line 590 "instr.perf"
      {"lahf", opcodes_lahf},
#line 502 "instr.perf"
      {"addsd", opcodes_addsd},
#line 248 "instr.perf"
      {"maxpd", opcodes_maxpd},
#line 323 "instr.perf"
      {"loopnz", opcodes_loopnz},
#line 349 "instr.perf"
      {"cvtpd2ps", opcodes_cvtpd2ps},
#line 539 "instr.perf"
      {"psraw", opcodes_psraw},
#line 435 "instr.perf"
      {"movdqa", opcodes_movdqa},
#line 609 "instr.perf"
      {"fldcw", opcodes_fldcw},
#line 311 "instr.perf"
      {"pause", opcodes_pause},
#line 393 "instr.perf"
      {"sysexit", opcodes_sysexit},
#line 553 "instr.perf"
      {"psubd", opcodes_psubd},
#line 151 "instr.perf"
      {"lodsw", opcodes_lodsw},
#line 614 "instr.perf"
      {"cmpxchg8b", opcodes_cmpxchg8b},
#line 227 "instr.perf"
      {"divss", opcodes_divss},
#line 148 "instr.perf"
      {"jae", opcodes_jae},
#line 510 "instr.perf"
      {"addpd", opcodes_addpd},
#line 550 "instr.perf"
      {"pi2fd", opcodes_pi2fd},
#line 585 "instr.perf"
      {"movdq2q", opcodes_movdq2q},
#line 257 "instr.perf"
      {"pavgb", opcodes_pavgb},
#line 229 "instr.perf"
      {"shufps", opcodes_shufps},
#line 120 "instr.perf"
      {"fldz", opcodes_fldz},
#line 560 "instr.perf"
      {"shufpd", opcodes_shufpd},
#line 279 "instr.perf"
      {"movaps", opcodes_movaps},
#line 360 "instr.perf"
      {"movapd", opcodes_movapd},
#line 562 "instr.perf"
      {"cvtpd2dq", opcodes_cvtpd2dq},
#line 521 "instr.perf"
      {"andpd", opcodes_andpd},
#line 281 "instr.perf"
      {"pcmpgtb", opcodes_pcmpgtb},
#line 301 "instr.perf"
      {"paddusb", opcodes_paddusb},
#line 491 "instr.perf"
      {"movsd", opcodes_movsd},
#line 580 "instr.perf"
      {"paddw", opcodes_paddw},
#line 238 "instr.perf"
      {"divps", opcodes_divps},
#line 166 "instr.perf"
      {"fldlg2", opcodes_fldlg2},
#line 357 "instr.perf"
      {"paddsb", opcodes_paddsb},
#line 430 "instr.perf"
      {"cvtsd2si", opcodes_cvtsd2si},
#line 123 "instr.perf"
      {"jge", opcodes_jge},
#line 565 "instr.perf"
      {"jbe", opcodes_jbe},
#line 83 "instr.perf"
      {"psubusb", opcodes_psubusb},
#line 514 "instr.perf"
      {"psubw", opcodes_psubw},
#line 407 "instr.perf"
      {"jcxz", opcodes_jcxz},
#line 484 "instr.perf"
      {"psubsb", opcodes_psubsb},
#line 168 "instr.perf"
      {"pi2fw", opcodes_pi2fw},
#line 264 "instr.perf"
      {"prefetch", opcodes_prefetch},
#line 173 "instr.perf"
      {"prefetch1", opcodes_prefetch1},
#line 213 "instr.perf"
      {"prefetch0", opcodes_prefetch0},
#line 508 "instr.perf"
      {"pfmax", opcodes_pfmax},
#line 70 "instr.perf"
      {"packuswb", opcodes_packuswb},
#line 64 "instr.perf"
      {"cvtpd2pi", opcodes_cvtpd2pi},
#line 367 "instr.perf"
      {"prefetchw", opcodes_prefetchw},
#line 493 "instr.perf"
      {"movsw", opcodes_movsw},
#line 476 "instr.perf"
      {"packsswb", opcodes_packsswb},
#line 573 "instr.perf"
      {"divsd", opcodes_divsd},
#line 513 "instr.perf"
      {"movsx", opcodes_movsx},
#line 171 "instr.perf"
      {"invlpg", opcodes_invlpg},
#line 185 "instr.perf"
      {"divpd", opcodes_divpd},
#line 467 "instr.perf"
      {"cpuid", opcodes_cpuid},
#line 428 "instr.perf"
      {"bound", opcodes_bound},
#line 405 "instr.perf"
      {"prefetch2", opcodes_prefetch2},
#line 293 "instr.perf"
      {"prefetchnta", opcodes_prefetchnta},
#line 326 "instr.perf"
      {"pavgusb", opcodes_pavgusb},
#line 581 "instr.perf"
      {"pavgw", opcodes_pavgw},
#line 390 "instr.perf"
      {"movzx", opcodes_movzx}
    };

  static short lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,   1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   2,
       -1,  -1,  -1,  -1,   3,  -1,  -1,  -1,  -1,  -1,
        4,  -1,  -1,  -1,  -1,   5,  -1,  -1,  -1,  -1,
        6,  -1,  -1,   7,   8,   9,  10,  -1,  -1,  11,
       -1,  12,  -1,  -1,  -1,  13,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  14,  -1,  15,  16,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  -1,
       18,  -1,  -1,  19,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  20,  21,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  22,  -1,  -1,  -1,  -1,  -1,
       23,  24,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  25,  -1,  26,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  27,  28,
       -1,  -1,  -1,  -1,  -1,  29,  -1,  -1,  30,  -1,
       31,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       32,  -1,  -1,  33,  34,  35,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  36,  37,  -1,  -1,  -1,  -1,  -1,  -1,  38,
       39,  -1,  -1,  -1,  40,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  41,  -1,  -1,  -1,  42,  -1,  -1,  43,  -1,
       -1,  -1,  44,  45,  -1,  46,  -1,  47,  48,  49,
       50,  -1,  -1,  51,  52,  53,  54,  -1,  55,  -1,
       56,  57,  58,  59,  -1,  -1,  60,  61,  62,  -1,
       63,  64,  -1,  65,  66,  67,  -1,  68,  69,  -1,
       -1,  70,  71,  72,  73,  -1,  -1,  -1,  74,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  75,  76,  77,  -1,
       -1,  -1,  78,  79,  -1,  -1,  -1,  80,  81,  -1,
       82,  83,  -1,  84,  -1,  -1,  85,  -1,  86,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  87,
       88,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  89,  -1,  -1,  -1,  -1,
       90,  -1,  -1,  -1,  -1,  91,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  92,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  93,  94,  95,  96,  -1,  -1,  -1,
       -1,  -1,  -1,  97,  -1,  98,  -1,  99,  -1, 100,
       -1,  -1,  -1,  -1, 101,  -1,  -1, 102, 103, 104,
      105,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 106, 107,  -1,  -1, 108,  -1,
       -1,  -1,  -1,  -1, 109, 110,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 111, 112,  -1,  -1,  -1,  -1,  -1,
      113,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 114,
       -1,  -1,  -1,  -1, 115,  -1,  -1,  -1,  -1, 116,
       -1,  -1,  -1,  -1,  -1,  -1, 117,  -1,  -1,  -1,
       -1, 118,  -1,  -1,  -1,  -1,  -1, 119,  -1,  -1,
       -1, 120,  -1, 121,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 122, 123,  -1,  -1,  -1, 124, 125, 126,  -1,
       -1,  -1,  -1,  -1,  -1, 127, 128,  -1, 129, 130,
      131, 132, 133,  -1,  -1, 134,  -1, 135,  -1,  -1,
       -1, 136,  -1,  -1,  -1,  -1, 137,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 138,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 139,  -1,
       -1,  -1,  -1, 140,  -1,  -1,  -1, 141, 142,  -1,
      143, 144,  -1,  -1,  -1,  -1,  -1,  -1, 145,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      146,  -1,  -1,  -1,  -1, 147,  -1,  -1,  -1,  -1,
      148,  -1,  -1, 149,  -1,  -1,  -1,  -1,  -1,  -1,
      150,  -1,  -1, 151,  -1,  -1, 152, 153, 154,  -1,
       -1, 155,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 156,  -1,  -1,  -1, 157,  -1,  -1,  -1,
       -1,  -1, 158, 159,  -1,  -1,  -1,  -1, 160, 161,
       -1,  -1, 162,  -1,  -1,  -1,  -1,  -1,  -1, 163,
       -1,  -1,  -1,  -1, 164,  -1,  -1,  -1, 165,  -1,
       -1, 166,  -1,  -1, 167,  -1, 168,  -1,  -1,  -1,
       -1, 169,  -1, 170,  -1, 171,  -1,  -1, 172,  -1,
       -1,  -1,  -1,  -1, 173,  -1,  -1,  -1, 174, 175,
       -1,  -1,  -1, 176,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 177,  -1, 178,  -1,  -1, 179, 180,
       -1,  -1,  -1, 181,  -1, 182,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 183,  -1,  -1,  -1, 184,  -1,
       -1,  -1,  -1, 185,  -1,  -1,  -1,  -1,  -1,  -1,
      186,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 187,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 188,  -1,  -1, 189,  -1, 190,  -1,  -1,
       -1,  -1,  -1, 191,  -1, 192,  -1,  -1,  -1, 193,
      194,  -1,  -1, 195, 196, 197,  -1, 198,  -1, 199,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 200, 201,  -1,
       -1,  -1, 202,  -1,  -1,  -1, 203,  -1,  -1,  -1,
       -1,  -1,  -1, 204, 205, 206, 207,  -1, 208,  -1,
       -1,  -1,  -1, 209,  -1,  -1,  -1, 210, 211,  -1,
       -1,  -1,  -1, 212,  -1,  -1,  -1,  -1, 213,  -1,
       -1,  -1,  -1,  -1,  -1, 214,  -1,  -1,  -1,  -1,
      215,  -1,  -1,  -1,  -1, 216,  -1,  -1,  -1,  -1,
       -1, 217, 218,  -1, 219,  -1, 220,  -1,  -1, 221,
      222, 223, 224,  -1, 225,  -1,  -1,  -1,  -1,  -1,
      226,  -1, 227, 228,  -1, 229, 230, 231,  -1,  -1,
      232,  -1, 233, 234,  -1,  -1,  -1,  -1,  -1,  -1,
      235,  -1,  -1,  -1,  -1, 236,  -1,  -1,  -1,  -1,
      237, 238,  -1,  -1,  -1, 239,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 240,  -1,  -1,  -1,  -1,  -1, 241,
       -1, 242,  -1, 243,  -1,  -1,  -1,  -1,  -1,  -1,
      244,  -1,  -1,  -1, 245,  -1,  -1,  -1, 246,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 247,  -1,  -1, 248, 249,  -1, 250, 251,
       -1,  -1,  -1,  -1,  -1,  -1, 252, 253, 254,  -1,
      255, 256,  -1, 257,  -1, 258,  -1,  -1,  -1,  -1,
       -1, 259, 260, 261,  -1,  -1,  -1,  -1,  -1, 262,
       -1,  -1,  -1,  -1,  -1,  -1, 263,  -1, 264, 265,
      266,  -1,  -1, 267,  -1,  -1,  -1,  -1, 268,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 269,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 270,  -1,  -1, 271,  -1,  -1,
       -1,  -1, 272, 273,  -1,  -1,  -1, 274,  -1, 275,
      276, 277,  -1,  -1,  -1,  -1,  -1,  -1, 278, 279,
       -1,  -1,  -1, 280,  -1,  -1, 281,  -1,  -1, 282,
      283,  -1,  -1,  -1, 284, 285,  -1,  -1,  -1, 286,
      287,  -1,  -1, 288,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 289,  -1,  -1, 290,  -1,
      291,  -1,  -1,  -1, 292,  -1, 293,  -1,  -1,  -1,
      294,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 295, 296, 297,  -1,  -1,  -1,
      298,  -1, 299,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      300,  -1,  -1,  -1,  -1, 301,  -1,  -1, 302,  -1,
      303, 304,  -1,  -1,  -1, 305, 306,  -1,  -1,  -1,
      307,  -1,  -1,  -1,  -1, 308,  -1,  -1,  -1, 309,
      310,  -1,  -1,  -1, 311, 312,  -1,  -1,  -1,  -1,
       -1, 313, 314, 315,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 316, 317,  -1, 318,  -1,  -1,  -1,  -1,
      319,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 320,
       -1, 321,  -1,  -1,  -1,  -1,  -1,  -1, 322,  -1,
      323,  -1, 324, 325,  -1,  -1,  -1,  -1, 326, 327,
       -1,  -1, 328,  -1, 329,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 330,  -1, 331,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      332,  -1,  -1,  -1,  -1,  -1,  -1, 333, 334,  -1,
      335,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      336,  -1,  -1, 337,  -1, 338,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 339,  -1,
       -1,  -1,  -1,  -1, 340, 341,  -1,  -1,  -1,  -1,
       -1, 342,  -1,  -1,  -1,  -1,  -1, 343, 344,  -1,
      345,  -1,  -1,  -1,  -1, 346, 347,  -1,  -1, 348,
       -1,  -1,  -1, 349,  -1,  -1, 350,  -1, 351,  -1,
      352,  -1,  -1,  -1, 353,  -1,  -1,  -1,  -1, 354,
      355,  -1,  -1,  -1,  -1,  -1,  -1, 356, 357,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 358,
       -1, 359, 360, 361,  -1, 362,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 363,  -1,  -1,  -1,
      364,  -1, 365,  -1, 366,  -1,  -1,  -1,  -1, 367,
       -1, 368,  -1,  -1, 369,  -1, 370,  -1,  -1,  -1,
      371,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 372, 373,
       -1, 374, 375,  -1, 376,  -1,  -1, 377, 378, 379,
       -1,  -1,  -1,  -1, 380,  -1, 381,  -1,  -1,  -1,
      382, 383,  -1,  -1, 384, 385, 386, 387,  -1,  -1,
      388,  -1,  -1,  -1, 389,  -1,  -1, 390,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 391,  -1,  -1,  -1, 392,
       -1,  -1,  -1,  -1, 393, 394, 395,  -1, 396,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 397, 398,  -1,  -1,
       -1,  -1,  -1,  -1, 399, 400, 401,  -1,  -1,  -1,
      402,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 403,  -1,
       -1,  -1,  -1, 404,  -1,  -1,  -1, 405,  -1,  -1,
      406,  -1,  -1,  -1, 407,  -1,  -1,  -1,  -1, 408,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 409,  -1,  -1, 410,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 411,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 412,  -1,  -1, 413,  -1,  -1,  -1, 414,
       -1,  -1, 415, 416, 417,  -1,  -1,  -1,  -1, 418,
      419,  -1,  -1, 420,  -1, 421,  -1, 422,  -1,  -1,
       -1,  -1, 423,  -1, 424,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 425,  -1,  -1,  -1,
      426,  -1,  -1,  -1, 427, 428,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 429,  -1,  -1,
      430,  -1,  -1,  -1,  -1,  -1,  -1, 431,  -1,  -1,
      432,  -1,  -1,  -1,  -1, 433,  -1,  -1,  -1,  -1,
       -1, 434,  -1,  -1,  -1,  -1, 435,  -1,  -1,  -1,
       -1, 436,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      437, 438,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 439,  -1, 440,  -1,
       -1, 441,  -1, 442,  -1, 443,  -1,  -1,  -1, 444,
       -1, 445,  -1,  -1, 446,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 447,
       -1,  -1,  -1, 448,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 449,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 450, 451,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 452,  -1,  -1, 453,
      454,  -1,  -1,  -1, 455, 456,  -1,  -1,  -1,  -1,
       -1, 457,  -1,  -1, 458,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 459,  -1,  -1,  -1,  -1, 460,  -1,
      461,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 462,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 463,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 464,  -1,  -1,
       -1,  -1,  -1, 465,  -1, 466,  -1, 467,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 468,
       -1,  -1,  -1,  -1,  -1, 469,  -1,  -1,  -1,  -1,
      470,  -1,  -1,  -1,  -1, 471, 472,  -1,  -1,  -1,
      473,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 474,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 475,  -1,  -1,  -1,  -1, 476, 477,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      478,  -1,  -1, 479,  -1,  -1,  -1,  -1, 480,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      481,  -1,  -1, 482,  -1, 483,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 484,  -1, 485, 486,
       -1,  -1,  -1, 487,  -1, 488,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 489,  -1,  -1,  -1,  -1,  -1, 490,
       -1,  -1,  -1,  -1,  -1, 491,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 492,  -1,  -1,  -1,  -1, 493,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 494,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 495,
      496,  -1, 497,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      498,  -1,  -1,  -1,  -1, 499,  -1,  -1,  -1, 500,
       -1,  -1,  -1,  -1, 501,  -1,  -1,  -1,  -1, 502,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 503,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      504,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      505,  -1,  -1,  -1,  -1, 506,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      507,  -1,  -1, 508,  -1,  -1, 509,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 510,  -1, 511,  -1,  -1,  -1,  -1,  -1,  -1,
      512, 513, 514,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 515, 516,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 517,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 518,  -1,
       -1,  -1, 519,  -1,  -1,  -1,  -1, 520,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 521,  -1, 522,  -1,  -1,  -1,  -1,  -1,  -1,
      523,  -1,  -1,  -1,  -1, 524,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 525,  -1,  -1,  -1,  -1,  -1,
      526,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 527,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      528,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 529,  -1,
       -1,  -1,  -1,  -1, 530, 531,  -1,  -1,  -1, 532,
       -1,  -1,  -1,  -1, 533,  -1, 534,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 535,
       -1,  -1,  -1,  -1, 536,  -1,  -1,  -1,  -1, 537,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 538,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 539,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 540,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 541,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 542,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 543,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      544,  -1,  -1, 545,  -1, 546,  -1,  -1,  -1,  -1,
      547,  -1,  -1,  -1,  -1, 548,  -1,  -1, 549,  -1,
       -1,  -1,  -1,  -1,  -1, 550,  -1, 551,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 552,  -1, 553,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 554,
       -1,  -1,  -1,  -1, 555,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 556,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 557,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 558,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 559,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 560,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 561,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 562,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 563,  -1,
       -1,  -1,  -1,  -1,  -1, 564,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 565,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 566, 567,  -1,  -1,  -1,  -1, 568,
       -1,  -1,  -1,  -1, 569,  -1,  -1,  -1, 570,  -1,
       -1,  -1,  -1, 571,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 572,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 573,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 574,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 575,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 576,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 577,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 578,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 579,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 580,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 581,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 582,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 583,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 584,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 585
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist[index];
            }
        }
    }
  return 0;
}
#line 635 "instr.perf"

