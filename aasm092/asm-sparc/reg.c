/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -N asm_sparc_get_reg -L ANSI-C -k'*' -c -D -t reg.perf  */

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

#line 1 "reg.perf"
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
**    Copyright:
**    contact  :
*/

/*
+options+-N asm_sparc_get_reg -L ANSI-C -k* -c -D -t
*/

#include <string.h>

#include "aasm/size.h"

#include "asm.h"
#include "fields.h"

static unsigned int
hash			(const char			*str,
			 unsigned int			len);


/*
** EXPORTED FUNCTION
** Return register info struct from reg name
*/

struct register_s	*
asm_sparc_get_reg	(const char			*str,
			 unsigned int			len);

/*
** Registers list with info fields
*/

#define SZ_32		SIZE_FLAG_DWORD
#define SZ_64		SIZE_FLAG_QWORD
#define SZ_128		SIZE_FLAG_OWORD

#line 58 "reg.perf"
struct register_s
;

#define TOTAL_KEYWORDS 163
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 392
/* maximum key range = 391, duplicates = 0 */

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
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393,  19,  18,
       30,  75, 100, 220, 150, 193,  50,   0,  10,  90,
      165, 155,  65,  75, 145, 120, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393,   0, 393,   5,
      393, 393,   0,   4, 393, 190, 120, 110, 225,  35,
      393, 130, 150,   0,  10,  15,   0, 393, 393,  35,
       45, 100, 393,  10, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393, 393, 393, 393, 393, 393, 393,
      393, 393, 393, 393
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]+4];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]+1];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]+8];
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
struct register_s *
asm_sparc_get_reg (register const char *str, register unsigned int len)
{
  static struct register_s wordlist[] =
    {
#line 96 "reg.perf"
      {"f1",	1,  SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 114 "reg.perf"
      {"f19",	19, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 62 "reg.perf"
      {"g1",	1,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 161 "reg.perf"
      {"c1",	1, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 179 "reg.perf"
      {"c19",	19, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 97 "reg.perf"
      {"f2",	2,  SZ_64 | SZ_32,		OP_FSREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 124 "reg.perf"
      {"f29",	29, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 63 "reg.perf"
      {"g2",	2,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 162 "reg.perf"
      {"c2",	2, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 189 "reg.perf"
      {"c29",	29, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 106 "reg.perf"
      {"f11",	11, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 105 "reg.perf"
      {"f10",	10, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 143 "reg.perf"
      {"fsr",	0, SZ_32,			OP_REGOTHER(0), FLG_V8 | FLG_V9,	REG_OTHER_FSR},
#line 200 "reg.perf"
      {"asr8",	8, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 171 "reg.perf"
      {"c11",	11, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 170 "reg.perf"
      {"c10",	10, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 147 "reg.perf"
      {"csr",	0, SZ_32,			OP_REGOTHER(0), FLG_V8 | FLG_V9,	REG_OTHER_CSR},
#line 116 "reg.perf"
      {"f21",	21, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 115 "reg.perf"
      {"f20",	20, SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 107 "reg.perf"
      {"f12",	12, SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 201 "reg.perf"
      {"asr9",	9, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 181 "reg.perf"
      {"c21",	21, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 180 "reg.perf"
      {"c20",	20, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 172 "reg.perf"
      {"c12",	12, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 192 "reg.perf"
      {"asr0",	0, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 117 "reg.perf"
      {"f22",	22, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 93 "reg.perf"
      {"fp",	30, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 182 "reg.perf"
      {"c22",	22, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 95 "reg.perf"
      {"f0",	0,  SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 113 "reg.perf"
      {"f18",	18, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 193 "reg.perf"
      {"asr1",	1, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 207 "reg.perf"
      {"asr15",	15, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 61 "reg.perf"
      {"g0",	0,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 160 "reg.perf"
      {"c0",	0, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 178 "reg.perf"
      {"c18",	18, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 159 "reg.perf"
      {"fprs",	6, SZ_32,			OP_REGOTHER(0), FLG_V9,			REG_OTHER_ASR},
#line 76 "reg.perf"
      {"sp",	14, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 123 "reg.perf"
      {"f28",	28, SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 208 "reg.perf"
      {"asr16",	16, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 101 "reg.perf"
      {"f6",	6,  SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 188 "reg.perf"
      {"c28",	28, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 67 "reg.perf"
      {"g6",	6,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 166 "reg.perf"
      {"c6",	6, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 148 "reg.perf"
      {"wim",	0, SZ_32,			OP_REGOTHER(0), FLG_V8,			REG_OTHER_WIM},
#line 199 "reg.perf"
      {"asr7",	7, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 102 "reg.perf"
      {"f7",	7,  SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 108 "reg.perf"
      {"f13",	13, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 68 "reg.perf"
      {"g7",	7,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 167 "reg.perf"
      {"c7",	7, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 173 "reg.perf"
      {"c13",	13, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 141 "reg.perf"
      {"f60",	29, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 118 "reg.perf"
      {"f23",	23, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 98 "reg.perf"
      {"f3",	3,  SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 183 "reg.perf"
      {"c23",	23, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 64 "reg.perf"
      {"g3",	3,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 163 "reg.perf"
      {"c3",	3, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 142 "reg.perf"
      {"f62",	31, SZ_64,			OP_FDREG(0), FLG_V9},
#line 194 "reg.perf"
      {"asr2",	2, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 217 "reg.perf"
      {"asr25",	25, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 154 "reg.perf"
      {"y",	0, SZ_32,			OP_REGOTHER(0), FLG_V8 | FLG_V9,	REG_OTHER_ASR},
#line 145 "reg.perf"
      {"fq",	0, SZ_64,			OP_REGOTHER(0), FLG_V8 | FLG_V9,	REG_OTHER_FQ},
#line 109 "reg.perf"
      {"f14",	14, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 206 "reg.perf"
      {"asr14",	14, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 146 "reg.perf"
      {"cq",	0, SZ_64,			OP_REGOTHER(0), FLG_V8 | FLG_V9,	REG_OTHER_CQ},
#line 174 "reg.perf"
      {"c14",	14, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 218 "reg.perf"
      {"asr26",	26, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 126 "reg.perf"
      {"f31",	31, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 125 "reg.perf"
      {"f30",	30, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 119 "reg.perf"
      {"f24",	24, SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 191 "reg.perf"
      {"c31",	31, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 190 "reg.perf"
      {"c30",	30, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 184 "reg.perf"
      {"c24",	24, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 104 "reg.perf"
      {"f9",	9,  SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 127 "reg.perf"
      {"f32",	1, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 195 "reg.perf"
      {"asr3",	3, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 169 "reg.perf"
      {"c9",	9, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 155 "reg.perf"
      {"ccr",	2, SZ_32,			OP_REGOTHER(0), FLG_V9,			REG_OTHER_ASR},
#line 70 "reg.perf"
      {"o1",	9,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 149 "reg.perf"
      {"tbr",	0, SZ_32,			OP_REGOTHER(0), FLG_V8,			REG_OTHER_TBR},
#line 150 "reg.perf"
      {"fcc0",	0, 0,				OP_REGOTHER(0), FLG_V9,			REG_OTHER_FCC},
#line 71 "reg.perf"
      {"o2",	10, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 130 "reg.perf"
      {"f38",	7, SZ_64,			OP_FDREG(0), FLG_V9},
#line 209 "reg.perf"
      {"asr17",	17, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 103 "reg.perf"
      {"f8",	8,  SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 151 "reg.perf"
      {"fcc1",	1, 0,				OP_REGOTHER(0), FLG_V9,			REG_OTHER_FCC},
#line 216 "reg.perf"
      {"asr24",	24, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 168 "reg.perf"
      {"c8",	8, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 111 "reg.perf"
      {"f16",	16, SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 202 "reg.perf"
      {"asr10",	10, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 100 "reg.perf"
      {"f5",	5,  SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 176 "reg.perf"
      {"c16",	16, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 66 "reg.perf"
      {"g5",	5,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 165 "reg.perf"
      {"c5",	5, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 121 "reg.perf"
      {"f26",	26, SZ_64 | SZ_32,		OP_FREG(0) | OP_FDREG(0), FLG_V8 | FLG_V9},
#line 99 "reg.perf"
      {"f4",	4,  SZ_128 | SZ_64 | SZ_32,	OP_FREG(0), FLG_V8 | FLG_V9},
#line 186 "reg.perf"
      {"c26",	26, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 65 "reg.perf"
      {"g4",	4,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 164 "reg.perf"
      {"c4",	4, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 144 "reg.perf"
      {"psr",	0, SZ_32,			OP_REGOTHER(0), FLG_V8,			REG_OTHER_PSR},
#line 197 "reg.perf"
      {"asr5",	5, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 136 "reg.perf"
      {"f50",	19, SZ_64,			OP_FDREG(0), FLG_V9},
#line 69 "reg.perf"
      {"o0",	8,  SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 131 "reg.perf"
      {"f40",	9, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 137 "reg.perf"
      {"f52",	21, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 219 "reg.perf"
      {"asr27",	27, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 87 "reg.perf"
      {"i1",	25, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 128 "reg.perf"
      {"f34",	3, SZ_64,			OP_FDREG(0), FLG_V9},
#line 152 "reg.perf"
      {"fcc2",	2, 0,				OP_REGOTHER(0), FLG_V9,			REG_OTHER_FCC},
#line 112 "reg.perf"
      {"f17",	17, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 75 "reg.perf"
      {"o6",	14, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 132 "reg.perf"
      {"f42",	11, SZ_64,			OP_FDREG(0), FLG_V9},
#line 212 "reg.perf"
      {"asr20",	20, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 177 "reg.perf"
      {"c17",	17, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 88 "reg.perf"
      {"i2",	26, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 156 "reg.perf"
      {"asi",	3, SZ_32,			OP_REGOTHER(0), FLG_V9,			REG_OTHER_ASR},
#line 204 "reg.perf"
      {"asr12",	12, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 122 "reg.perf"
      {"f27",	27, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 77 "reg.perf"
      {"o7",	15, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 140 "reg.perf"
      {"f58",	27, SZ_64,			OP_FDREG(0), FLG_V9},
#line 211 "reg.perf"
      {"asr19",	19, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 187 "reg.perf"
      {"c27",	27, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 198 "reg.perf"
      {"asr6",	6, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 135 "reg.perf"
      {"f48",	17, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 153 "reg.perf"
      {"fcc3",	3, 0,				OP_REGOTHER(0), FLG_V9,			REG_OTHER_FCC},
#line 210 "reg.perf"
      {"asr18",	18, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 72 "reg.perf"
      {"o3",	11, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 110 "reg.perf"
      {"f15",	15, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 222 "reg.perf"
      {"asr30",	30, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 79 "reg.perf"
      {"l1",	17, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 175 "reg.perf"
      {"c15",	15, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 120 "reg.perf"
      {"f25",	25, SZ_32,			OP_FSREG(0), FLG_V8 | FLG_V9},
#line 157 "reg.perf"
      {"tick",	4, SZ_32,			OP_REGOTHER(0), FLG_V9,			REG_OTHER_ASR},
#line 80 "reg.perf"
      {"l2",	18, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 185 "reg.perf"
      {"c25",	25, SZ_32,			OP_CREG(0), FLG_V8 | FLG_V9},
#line 86 "reg.perf"
      {"i0",	24, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 129 "reg.perf"
      {"f36",	5, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 196 "reg.perf"
      {"asr4",	4, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 205 "reg.perf"
      {"asr13",	13, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 214 "reg.perf"
      {"asr22",	22, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 221 "reg.perf"
      {"asr29",	29, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 92 "reg.perf"
      {"i6",	30, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 138 "reg.perf"
      {"f54",	23, SZ_64,			OP_FDREG(0), FLG_V9},
#line 158 "reg.perf"
      {"pc",	5, SZ_32,			OP_REGOTHER(0), FLG_V9,			REG_OTHER_ASR},
#line 220 "reg.perf"
      {"asr28",	28, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 94 "reg.perf"
      {"i7",	31, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 133 "reg.perf"
      {"f44",	13, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 203 "reg.perf"
      {"asr11",	11, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 78 "reg.perf"
      {"l0",	16, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 89 "reg.perf"
      {"i3",	27, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 74 "reg.perf"
      {"o5",	13, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 84 "reg.perf"
      {"l6",	22, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 215 "reg.perf"
      {"asr23",	23, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 73 "reg.perf"
      {"o4",	12, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 85 "reg.perf"
      {"l7",	23, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 139 "reg.perf"
      {"f56",	25, SZ_128 | SZ_64,		OP_FDREG(0) | OP_FQREG(0), FLG_V9},
#line 81 "reg.perf"
      {"l3",	19, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 134 "reg.perf"
      {"f46",	15, SZ_64,			OP_FDREG(0), FLG_V9},
#line 213 "reg.perf"
      {"asr21",	21, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 223 "reg.perf"
      {"asr31",	31, SZ_32,			OP_ASRREG(0), FLG_V8 | FLG_V9},
#line 91 "reg.perf"
      {"i5",	29, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 90 "reg.perf"
      {"i4",	28, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 83 "reg.perf"
      {"l5",	21, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9},
#line 82 "reg.perf"
      {"l4",	20, SZ_64 | SZ_32,		OP_RREG(0), FLG_V8 | FLG_V9}
    };

  static short lookup[] =
    {
       -1,  -1,   0,   1,  -1,  -1,   2,   3,   4,  -1,
       -1,  -1,   5,   6,  -1,  -1,   7,   8,   9,  -1,
       -1,  10,  11,  12,  13,  -1,  14,  15,  16,  -1,
       -1,  17,  18,  19,  20,  -1,  21,  22,  23,  -1,
       -1,  -1,  24,  25,  -1,  -1,  -1,  26,  27,  -1,
       -1,  -1,  28,  29,  30,  31,  32,  33,  34,  35,
       -1,  -1,  36,  37,  -1,  38,  -1,  39,  40,  -1,
       -1,  41,  42,  43,  44,  -1,  -1,  45,  46,  -1,
       -1,  47,  48,  49,  -1,  -1,  -1,  50,  51,  -1,
       -1,  -1,  52,  53,  -1,  -1,  54,  55,  56,  57,
       58,  59,  60,  61,  -1,  62,  -1,  63,  64,  -1,
       65,  66,  67,  68,  -1,  -1,  69,  70,  71,  -1,
       -1,  -1,  72,  73,  74,  -1,  -1,  75,  76,  -1,
       -1,  -1,  77,  78,  -1,  -1,  -1,  79,  -1,  -1,
       -1,  -1,  80,  81,  -1,  82,  -1,  83,  -1,  84,
       85,  -1,  86,  87,  -1,  88,  -1,  89,  90,  -1,
       -1,  91,  92,  93,  -1,  -1,  -1,  94,  95,  -1,
       -1,  96,  97,  98,  99,  -1,  -1, 100,  -1,  -1,
       -1,  -1, 101,  -1,  -1,  -1,  -1, 102, 103,  -1,
      104,  -1, 105, 106, 107,  -1, 108, 109, 110,  -1,
      111, 112, 113, 114,  -1, 115, 116, 117, 118,  -1,
      119, 120,  -1,  -1,  -1,  -1,  -1, 121, 122, 123,
      124,  -1, 125, 126,  -1, 127,  -1, 128, 129,  -1,
       -1,  -1,  -1, 130, 131,  -1,  -1, 132, 133,  -1,
       -1,  -1, 134, 135, 136,  -1,  -1,  -1, 137,  -1,
      138,  -1,  -1,  -1,  -1, 139,  -1, 140, 141,  -1,
       -1,  -1, 142,  -1,  -1, 143,  -1, 144, 145,  -1,
       -1,  -1,  -1,  -1,  -1, 146,  -1, 147,  -1,  -1,
       -1,  -1, 148,  -1,  -1,  -1,  -1, 149,  -1,  -1,
       -1,  -1, 150, 151,  -1,  -1,  -1, 152,  -1,  -1,
       -1,  -1, 153,  -1,  -1,  -1,  -1,  -1, 154,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 155, 156,  -1,
      157,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 158,  -1, 159,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 160,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 161,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 162
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
#line 224 "reg.perf"


/*
** EXPORTED FUNCTION
** Return register size flag from info struct
*/

unsigned int
asm_sparc_reg_size	(const struct register_s	*reg)
{
  return (reg->size);
}

/*
** EXPORTED FUNCTION
** Return register name from info structure
*/

const char		*
asm_sparc_reg_name	(const struct register_s	*reg)
{
  return (reg->name);
}

