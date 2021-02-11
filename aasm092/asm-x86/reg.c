/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -N asm_x86_get_reg -L ANSI-C -k'*' -c -D -t reg.perf  */

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
**    Copyright Alexandre Becoulet, 2002
**    contact : alexandre.becoulet@free.fr
*/

/*
+options+-N asm_x86_get_reg -L ANSI-C -k* -c -D -t
*/

#include <string.h>

#include "asm_mem_table.h"
#include "asm.h"
#include "flags.h"
#include "reg_id.h"

static unsigned int	hash(const char		*str,
		  	     unsigned int	len);

struct register_s	*asm_x86_get_reg(const char	*str,
					 unsigned int	len);

#line 42 "reg.perf"
struct register_s
;

#define TOTAL_KEYWORDS 69
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 4
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 208
/* maximum key range = 206, duplicates = 0 */

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
  static unsigned char asso_values[] =
    {
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209,  45,  22,
       30,  15,   0,  12,   2,  80, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209,  80, 209,  50,  35,  10,
        5,  60,  30,   0,  85,  50, 209, 209,  75,   0,
      209, 209,  25, 209,   0,  15,  45, 209, 209, 209,
        0, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
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
struct register_s *
asm_x86_get_reg (register const char *str, register unsigned int len)
{
  static struct register_s wordlist[] =
    {
#line 82 "reg.perf"
      {"mm4",	REG_NUM_MM4,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM4,	1,			0},
#line 90 "reg.perf"
      {"xmm4",	REG_NUM_XMM4,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM4,	1,			0},
#line 84 "reg.perf"
      {"mm6",	REG_NUM_MM6,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM6,	1,			0},
#line 92 "reg.perf"
      {"xmm6",	REG_NUM_XMM6,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM6,	1,			0},
#line 57 "reg.perf"
      {"dx",	REG_NUM_DX,	OP_TYPE_DATA,	SIZE_FLAG_WORD,	REG_ID_DX,	REG_WORD_LO_PART,	0},
#line 110 "reg.perf"
      {"dr4",	REG_NUM_DR4,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR4,	1,			0},
#line 112 "reg.perf"
      {"dr6",	REG_NUM_DR6,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR6,	1,			0},
#line 56 "reg.perf"
      {"cx",	REG_NUM_CX,	OP_TYPE_COUNTER,	SIZE_FLAG_WORD,	REG_ID_CX,	REG_WORD_LO_PART,	0},
#line 105 "reg.perf"
      {"cr4",	REG_NUM_CR4,	OP_TYPE_CONTROL,	SIZE_FLAG_DWORD,	REG_ID_CR4,	1,			0},
#line 83 "reg.perf"
      {"mm5",	REG_NUM_MM5,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM5,	1,			0},
#line 91 "reg.perf"
      {"xmm5",	REG_NUM_XMM5,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM5,	1,			0},
#line 77 "reg.perf"
      {"gs",	REG_NUM_GS,	OP_TYPE_SEG_NEW,	SIZE_FLAG_WORD,	REG_ID_GS,	1,			0},
#line 81 "reg.perf"
      {"mm3",	REG_NUM_MM3,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM3,	1,			0},
#line 89 "reg.perf"
      {"xmm3",	REG_NUM_XMM3,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM3,	1,			0},
#line 111 "reg.perf"
      {"dr5",	REG_NUM_DR5,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR5,	1,			0},
#line 75 "reg.perf"
      {"ds",	REG_NUM_DS,	OP_TYPE_SEG_OLD,	SIZE_FLAG_WORD,	REG_ID_DS,	1,			0},
#line 109 "reg.perf"
      {"dr3",	REG_NUM_DR3,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR3,	1,			0},
#line 79 "reg.perf"
      {"mm1",	REG_NUM_MM1,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM1,	1,			0},
#line 87 "reg.perf"
      {"xmm1",	REG_NUM_XMM1,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM1,	1,			0},
#line 73 "reg.perf"
      {"cs",	REG_NUM_CS,	OP_TYPE_SEG_CS,	SIZE_FLAG_WORD,	REG_ID_CS,	1,			0},
#line 104 "reg.perf"
      {"cr3",	REG_NUM_CR3,	OP_TYPE_CONTROL,	SIZE_FLAG_DWORD,	REG_ID_CR3,	1,			0},
#line 107 "reg.perf"
      {"dr1",	REG_NUM_DR1,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR1,	1,			0},
#line 74 "reg.perf"
      {"ss",	REG_NUM_SS,	OP_TYPE_SEG_OLD,	SIZE_FLAG_WORD,	REG_ID_SS,	1,			0},
#line 80 "reg.perf"
      {"mm2",	REG_NUM_MM2,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM2,	1,			0},
#line 88 "reg.perf"
      {"xmm2",	REG_NUM_XMM2,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM2,	1,			0},
#line 58 "reg.perf"
      {"bx",	REG_NUM_BX,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_BX,	REG_WORD_LO_PART,	X86_MEM_BASE_PRESENT | REG_NUM_BX | X86_MEM_BASE_WORD},
#line 108 "reg.perf"
      {"dr2",	REG_NUM_DR2,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR2,	1,			0},
#line 59 "reg.perf"
      {"sp",	REG_NUM_SP,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_SP,	REG_WORD_LO_PART,	0},
#line 103 "reg.perf"
      {"cr2",	REG_NUM_CR2,	OP_TYPE_CONTROL,	SIZE_FLAG_DWORD,	REG_ID_CR2,	1,			0},
#line 76 "reg.perf"
      {"fs",	REG_NUM_FS,	OP_TYPE_SEG_NEW,	SIZE_FLAG_WORD,	REG_ID_FS,	1,			0},
#line 78 "reg.perf"
      {"mm0",	REG_NUM_MM0,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM0,	1,			0},
#line 86 "reg.perf"
      {"xmm0",	REG_NUM_XMM0,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM0,	1,			0},
#line 54 "reg.perf"
      {"ax",	REG_NUM_AX,	OP_TYPE_ACCUMULATOR,	SIZE_FLAG_WORD,	REG_ID_AX,	REG_WORD_LO_PART,	0},
#line 106 "reg.perf"
      {"dr0",	REG_NUM_DR0,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR0,	1,			0},
#line 62 "reg.perf"
      {"di",	REG_NUM_DI,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_DI,	REG_WORD_LO_PART,	X86_MEM_BASE_PRESENT | REG_NUM_DI | X86_MEM_BASE_WORD},
#line 102 "reg.perf"
      {"cr0",	REG_NUM_CR0,	OP_TYPE_CONTROL,	SIZE_FLAG_DWORD,	REG_ID_CR0,	1,			0},
#line 60 "reg.perf"
      {"bp",	REG_NUM_BP,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_BP,	REG_WORD_LO_PART,	X86_MEM_BASE_PRESENT | REG_NUM_BP | X86_MEM_BASE_WORD},
#line 98 "reg.perf"
      {"st4",	REG_NUM_ST4,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST4,	1,			0},
#line 100 "reg.perf"
      {"st6",	REG_NUM_ST6,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST6,	1,			0},
#line 61 "reg.perf"
      {"si",	REG_NUM_SI,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_SI,	REG_WORD_LO_PART,	X86_MEM_BASE_PRESENT | REG_NUM_SI | X86_MEM_BASE_WORD},
#line 48 "reg.perf"
      {"edx",	REG_NUM_EDX,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_EDX,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EDX},
#line 47 "reg.perf"
      {"ecx",	REG_NUM_ECX,	OP_TYPE_COUNTER,	SIZE_FLAG_DWORD,	REG_ID_ECX,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_ECX},
#line 99 "reg.perf"
      {"st5",	REG_NUM_ST5,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST5,	1,			0},
#line 72 "reg.perf"
      {"es",	REG_NUM_ES,	OP_TYPE_SEG_OLD,	SIZE_FLAG_WORD,	REG_ID_ES,	1,			0},
#line 97 "reg.perf"
      {"st3",	REG_NUM_ST3,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST3,	1,			0},
#line 66 "reg.perf"
      {"dl",	REG_NUM_DL,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_DL,	REG_BYTE_LO_PART,	0},
#line 85 "reg.perf"
      {"mm7",	REG_NUM_MM7,	OP_TYPE_MMX,		SIZE_FLAG_QWORD,	REG_ID_MM7,	1,			0},
#line 93 "reg.perf"
      {"xmm7",	REG_NUM_XMM7,	OP_TYPE_SSE,		SIZE_FLAG_OWORD,	REG_ID_XMM7,	1,			0},
#line 95 "reg.perf"
      {"st1",	REG_NUM_ST1,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST1,	1,			0},
#line 65 "reg.perf"
      {"cl",	REG_NUM_CL,	OP_TYPE_COUNTER,	SIZE_FLAG_BYTE,	REG_ID_CL,	REG_BYTE_LO_PART,	0},
#line 113 "reg.perf"
      {"dr7",	REG_NUM_DR7,	OP_TYPE_DEBUG,	SIZE_FLAG_DWORD,	REG_ID_DR7,	1,			0},
#line 70 "reg.perf"
      {"dh",	REG_NUM_DH,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_DH,	REG_BYTE_HI_PART,	0},
#line 96 "reg.perf"
      {"st2",	REG_NUM_ST2,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST2,	1,			0},
#line 69 "reg.perf"
      {"ch",	REG_NUM_CH,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_CH,	REG_BYTE_HI_PART,	0},
#line 49 "reg.perf"
      {"ebx",	REG_NUM_EBX,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_EBX,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EBX},
#line 50 "reg.perf"
      {"esp",	REG_NUM_ESP,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_ESP,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_ESP},
#line 94 "reg.perf"
      {"st0",	REG_NUM_ST0,	OP_TYPE_FPU_TOP,	SIZE_FLAG_TWORD,	REG_ID_ST0,	1,			0},
#line 67 "reg.perf"
      {"bl",	REG_NUM_BL,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_BL,	REG_BYTE_LO_PART,	0},
#line 45 "reg.perf"
      {"eax",	REG_NUM_EAX,	OP_TYPE_ACCUMULATOR,	SIZE_FLAG_DWORD,	REG_ID_EAX,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EAX},
#line 53 "reg.perf"
      {"edi",	REG_NUM_EDI,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_EDI,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EDI},
#line 71 "reg.perf"
      {"bh",	REG_NUM_BH,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_BH,	REG_BYTE_HI_PART,	0},
#line 51 "reg.perf"
      {"ebp",	REG_NUM_EBP,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_EBP,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EBP},
#line 63 "reg.perf"
      {"al",	REG_NUM_AL,	OP_TYPE_ACCUMULATOR,	SIZE_FLAG_BYTE,	REG_ID_AL,	REG_BYTE_LO_PART,	0},
#line 52 "reg.perf"
      {"esi",	REG_NUM_ESI,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_ESI,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_ESI},
#line 55 "reg.perf"
      {"ax_",	REG_NUM_AX,	OP_TYPE_GENERAL,	SIZE_FLAG_WORD,	REG_ID_AX,	REG_WORD_LO_PART,	0},
#line 68 "reg.perf"
      {"ah",	REG_NUM_AH,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_AH,	REG_BYTE_HI_PART,	0},
#line 101 "reg.perf"
      {"st7",	REG_NUM_ST7,	OP_TYPE_FPU,		SIZE_FLAG_TWORD,	REG_ID_ST7,	1,			0},
#line 46 "reg.perf"
      {"eax_",	REG_NUM_EAX,	OP_TYPE_GENERAL,	SIZE_FLAG_DWORD,	REG_ID_EAX,	REG_DWORD_PART,		X86_MEM_BASE_PRESENT | REG_NUM_EAX},
#line 64 "reg.perf"
      {"al_",	REG_NUM_AL,	OP_TYPE_GENERAL,	SIZE_FLAG_BYTE,	REG_ID_AL,	REG_BYTE_LO_PART,	0}
    };

  static signed char lookup[] =
    {
      -1, -1, -1,  0,  1,  2,  3,  4,  5, -1,  6, -1,  7,  8,
      -1,  9, 10, 11, 12, 13, 14, -1, 15, 16, -1, 17, 18, 19,
      20, -1, 21, -1, 22, 23, 24, -1, -1, 25, 26, -1, -1, -1,
      27, 28, -1, -1, -1, 29, 30, 31, -1, -1, 32, 33, -1, -1,
      -1, 34, 35, -1, -1, -1, 36, 37, -1, 38, -1, 39, 40, -1,
      -1, -1, -1, 41, -1, 42, -1, 43, 44, -1, -1, -1, 45, 46,
      47, 48, -1, 49, 50, -1, -1, -1, 51, 52, -1, -1, -1, 53,
      54, -1, -1, -1, -1, 55, -1, -1, -1, -1, 56, -1, -1, -1,
      57, 58, -1, -1, -1, -1, 59, -1, -1, -1, 60, 61, -1, -1,
      -1, 62, 63, -1, -1, -1, -1, 64, -1, -1, -1, 65, -1, -1,
      -1, -1, -1, 66, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 67, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 68
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
#line 114 "reg.perf"


unsigned int	asm_x86_reg_size(const struct register_s	*reg)
{
  return (reg->r_size);
}

const char	*asm_x86_reg_name(const struct register_s	*reg)
{
  return (reg->name);
}

