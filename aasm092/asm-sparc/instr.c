/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -N asm_sparc_get_instr_perf -L ANSI-C -k'*' -c -D -t instr.perf  */

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
**    the Free Software Foundation; either version 3 of the License, or
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
+options+ -N asm_sparc_get_instr_perf -L ANSI-C -k* -c -D -t
*/

#include <string.h>

#include "aasm/instr.h"
#include "aasm/expr.h"
#include "aasm/error.h"

#include "asm.h"
#include "fields.h"

#include "opcodes_id.h"

static void
asm_opcode_write	(opcode_bin_t		opcode,
			 aasm_u8_t		*bin)
{
  bin[0] = (opcode >> 24) & 0xff;
  bin[1] = (opcode >> 16) & 0xff;
  bin[2] = (opcode >> 8) & 0xff;
  bin[3] = opcode & 0xff;
}

/* FIXME use expr_dup on expr->right for relocation ? */

static opcode_bin_t
asm_get_memop		(struct instr_s		*instr,
			 struct section_s	*sec,
			 struct expr_s		*expr,
			 unsigned int		imm_size)
{
  switch (expr->flag & EXPR_TYPE_MASK)
    {
      /* [imm] */
    case (EXPR_TYPE_NUM):
      return (FLD_IBIT(1)
	      | asm_set_simm(instr, imm_size, 0, sec, &expr));

      /* [reg] */
    case (EXPR_TYPE_REGISTER):
      return (FLD_RS1(expr->val.reg->num));

    case (EXPR_TYPE_OPERATION):
      if (expr->val.op == &oper_b_add)
	{
	  /* [? + ?] */

	  if (expr->right->flag & expr->left->flag & EXPR_TYPE_REGISTER)
	    {
	      /* [reg + reg] */
	      
	      return (FLD_RS1(expr->left->val.reg->num)
		      | FLD_RS2(expr->right->val.reg->num));
	    }

	  /* [reg + ?] */
	  if (expr->right->flag & EXPR_TYPE_REGISTER)
	    return (FLD_RS1(expr->right->val.reg->num) | FLD_IBIT(1)
		    | asm_set_simm(instr, imm_size, 0, sec, &expr->left));

	  /* [? + reg] */
	  if (expr->left->flag & EXPR_TYPE_REGISTER)
	    return (FLD_RS1(expr->left->val.reg->num) | FLD_IBIT(1)
		    | asm_set_simm(instr, imm_size, 0, sec, &expr->right));
	}

    default:
      /* handle mem relocation here */
      error(ERRTYP_ERROR, "sparc: invalid address expression", expr->location);
    }
  return (0);
}

/*
** void
*/

static void
asm_format_void		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;

  asm_opcode_write(info->fix, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 1A
** label (disp30)
*/

static void
asm_format_I30		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_set_rimm(instr, 30, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 2A
** const22 , reg
*/

static void
asm_format_I22Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | asm_set_imm(instr, 22, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 2B
** label (disp22)
*/

static void
asm_format_I22		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_set_rimm(instr, 22, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3A
** [addr], reg
*/

static void
asm_format_M13Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RD(instr->operands[1]->val.reg->num)
    | asm_get_memop(instr, sec, instr->operands[0]->right, 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3A short
** [addr]
*/

static void
asm_format_M13		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_get_memop(instr, sec, instr->operands[0]->right, 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3A ADDR
** addr, reg
*/

static void
asm_format_A13Rd	(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RD(instr->operands[1]->val.reg->num)
    | asm_get_memop(instr, sec, instr->operands[0], 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3AR
** reg, [addr]
*/

static void
asm_format_RdM13		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RD(instr->operands[0]->val.reg->num)
    | asm_get_memop(instr, sec, instr->operands[1]->right, 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3B
** [reg], ASI, reg
*/

static void
asm_format_Rs1I8Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode 
    |= FLD_RD(instr->operands[2]->val.reg->num)
    |  FLD_RS1(instr->operands[0]->right->val.reg->num)
    |  asm_set_imm(instr, 8, 5, sec, &instr->operands[1]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3BR
** reg, [reg], ASI
*/

static void
asm_format_RdRs1I8		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode 
    |= FLD_RD(instr->operands[0]->val.reg->num)
    |  FLD_RS1(instr->operands[1]->right->val.reg->num)
    |  asm_set_imm(instr, 8, 5, sec, &instr->operands[2]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3CR
** regother, [addr]
*/

static void
asm_format__M13		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_get_memop(instr, sec, instr->operands[1]->right, 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3G
** regother, reg
*/

static void
asm_format__Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RD(instr->operands[1]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D
** reg, reg , reg
*/

static void
asm_format_Rs1Rs2Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[2]->val.reg->num)
    | FLD_RS2(instr->operands[1]->val.reg->num)
    | FLD_RS1(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short rd
** regrd, reg , regrd
*/

static void
asm_format_Rs2Rds1		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | FLD_RS2(instr->operands[0]->val.reg->num)
    | FLD_RS1(instr->operands[1]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short rd
** reg
*/

static void
asm_format_Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RD(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short
** reg, reg
*/

static void
asm_format_Rs1Rs2		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RS2(instr->operands[1]->val.reg->num)
    | FLD_RS1(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D reg other
** regother, reg, reg
*/

static void
asm_format_RdRs1Rs2		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RS2(instr->operands[2]->val.reg->num)
    | FLD_RS1(instr->operands[1]->val.reg->num)
    | FLD_RD(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short rs2
** reg
*/

static void
asm_format_Rs2		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= FLD_RS2(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short rs1, rd
** reg (, reg)
*/

static void
asm_format_Rs1Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | FLD_RS1(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

static void
asm_format_Rds1			(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[0]->val.reg->num)
    | FLD_RS1(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3D short rs2, rd
** reg (, reg)
*/

static void
asm_format_Rs2Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | FLD_RS2(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

static void
asm_format_Rds2			(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[0]->val.reg->num)
    | FLD_RS2(instr->operands[0]->val.reg->num);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3E
** reg, imm , reg
*/

static void
asm_format_Rs1I13Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[2]->val.reg->num)
    | FLD_RS1(instr->operands[0]->val.reg->num)
    | asm_set_simm(instr, 13, 0, sec, &instr->operands[1]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3E short rd
** imm, reg
*/

static void
asm_format_I13Rds1		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | FLD_RS1(instr->operands[1]->val.reg->num)
    | asm_set_simm(instr, 13, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3E short g0
** reg, imm, g0
*/

static void
asm_format_Rs1I13		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RS1(instr->operands[0]->val.reg->num)
    | asm_set_simm(instr, 13, 0, sec, &instr->operands[1]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3E short g0
** g0, reg, imm
*/

static void
asm_format_I13Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[1]->val.reg->num)
    | asm_set_simm(instr, 13, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3E short g0
** imm, reg, g0
*/

static void
asm_format_I13Rs1		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RS1(instr->operands[1]->val.reg->num)
    | asm_set_simm(instr, 13, 0, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3F ADDR
** addr
*/

static void
asm_format_A13		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_get_memop(instr, sec, instr->operands[0], 13);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

/*
** 3POP
** imm9, reg, reg , reg
*/

static void
asm_format_I9Rs1Rs2Rd		(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode
    |= FLD_RD(instr->operands[3]->val.reg->num)
    | FLD_RS2(instr->operands[2]->val.reg->num)
    | FLD_RS1(instr->operands[1]->val.reg->num)
    | asm_set_imm(instr, 9, 5, sec, &instr->operands[0]);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

static void
asm_format_A7	(ASM_INSTR_PARAMS)
{
  const struct instr_info_s	*info = instr->info;
  opcode_bin_t			opcode = info->fix;

  opcode |= asm_get_memop(instr, sec, instr->operands[0], 7);

  asm_opcode_write(opcode, instr->val.bin);
  instr_set_asm(instr, sizeof (opcode_bin_t));
}

#include "opcodes.h"

static unsigned int	
hash			(const char		*str,
			 unsigned int		len);

struct instr_perf_s	*
asm_sparc_get_instr_perf(const char		*str,
			 unsigned int		len);

/*
** Instruction list with pointer to info struct
*/

#line 651 "instr.perf"
struct instr_perf_s
;

#define TOTAL_KEYWORDS 266
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 877
/* maximum key range = 866, duplicates = 0 */

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
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878,   0,   5, 878, 878, 335,  50,
        5,  70, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878,   0,  60,   5,
       90,   5,  20, 280, 150, 290, 170, 878,  15, 270,
      235, 200,   0, 235, 280,  35,  50,  10, 175, 418,
      105, 878, 105,  15, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878, 878, 878, 878,
      878, 878, 878, 878, 878, 878, 878
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]+1];
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
asm_sparc_get_instr_perf (register const char *str, register unsigned int len)
{
  static struct instr_perf_s wordlist[] =
    {
#line 815 "instr.perf"
      {"cb", opcodes_cb},
#line 700 "instr.perf"
      {"cba", opcodes_cba},
#line 810 "instr.perf"
      {"cb,a", opcodes_cb_a},
#line 843 "instr.perf"
      {"cba,a", opcodes_cba_a},
#line 745 "instr.perf"
      {"cb2", opcodes_cb2},
#line 804 "instr.perf"
      {"cb2,a", opcodes_cb2_a},
#line 881 "instr.perf"
      {"ld", opcodes_ld},
#line 693 "instr.perf"
      {"lda", opcodes_lda},
#line 828 "instr.perf"
      {"fb", opcodes_fb},
#line 735 "instr.perf"
      {"fba", opcodes_fba},
#line 854 "instr.perf"
      {"fb,a", opcodes_fb_a},
#line 679 "instr.perf"
      {"fba,a", opcodes_fba_a},
#line 777 "instr.perf"
      {"fbe", opcodes_fbe},
#line 898 "instr.perf"
      {"fbe,a", opcodes_fbe_a},
#line 915 "instr.perf"
      {"fbu", opcodes_fbu},
#line 886 "instr.perf"
      {"fbu,a", opcodes_fbu_a},
#line 690 "instr.perf"
      {"fbl", opcodes_fbl},
#line 789 "instr.perf"
      {"fbue", opcodes_fbue},
#line 724 "instr.perf"
      {"fbl,a", opcodes_fbl_a},
#line 801 "instr.perf"
      {"fbue,a", opcodes_fbue_a},
#line 798 "instr.perf"
      {"st", opcodes_st},
#line 800 "instr.perf"
      {"sta", opcodes_sta},
#line 675 "instr.perf"
      {"fble", opcodes_fble},
#line 759 "instr.perf"
      {"fble,a", opcodes_fble_a},
#line 876 "instr.perf"
      {"fbul", opcodes_fbul},
#line 868 "instr.perf"
      {"fbul,a", opcodes_fbul_a},
#line 811 "instr.perf"
      {"fbule", opcodes_fbule},
#line 863 "instr.perf"
      {"b", opcodes_b},
#line 682 "instr.perf"
      {"fbule,a", opcodes_fbule_a},
#line 809 "instr.perf"
      {"cb1", opcodes_cb1},
#line 699 "instr.perf"
      {"cb1,a", opcodes_cb1_a},
#line 701 "instr.perf"
      {"tz", opcodes_tz},
#line 722 "instr.perf"
      {"b,a", opcodes_b_a},
#line 784 "instr.perf"
      {"cb12", opcodes_cb12},
#line 785 "instr.perf"
      {"cb12,a", opcodes_cb12_a},
#line 911 "instr.perf"
      {"te", opcodes_te},
#line 891 "instr.perf"
      {"sra", opcodes_sra},
#line 728 "instr.perf"
      {"bz", opcodes_bz},
#line 904 "instr.perf"
      {"bz,a", opcodes_bz_a},
#line 895 "instr.perf"
      {"be", opcodes_be},
#line 865 "instr.perf"
      {"cb3", opcodes_cb3},
#line 844 "instr.perf"
      {"be,a", opcodes_be_a},
#line 871 "instr.perf"
      {"cb3,a", opcodes_cb3_a},
#line 658 "instr.perf"
      {"srl", opcodes_srl},
#line 760 "instr.perf"
      {"cb23", opcodes_cb23},
#line 797 "instr.perf"
      {"cb23,a", opcodes_cb23_a},
#line 684 "instr.perf"
      {"ldub", opcodes_ldub},
#line 849 "instr.perf"
      {"lduba", opcodes_lduba},
#line 706 "instr.perf"
      {"add", opcodes_add},
#line 772 "instr.perf"
      {"call", opcodes_call},
#line 730 "instr.perf"
      {"calla", opcodes_calla},
#line 733 "instr.perf"
      {"stb", opcodes_stb},
#line 834 "instr.perf"
      {"stba", opcodes_stba},
#line 821 "instr.perf"
      {"addcc", opcodes_addcc},
#line 792 "instr.perf"
      {"ta", opcodes_ta},
#line 860 "instr.perf"
      {"ldd", opcodes_ldd},
#line 713 "instr.perf"
      {"ldda", opcodes_ldda},
#line 869 "instr.perf"
      {"dec", opcodes_dec},
#line 710 "instr.perf"
      {"ldsb", opcodes_ldsb},
#line 660 "instr.perf"
      {"ldsba", opcodes_ldsba},
#line 686 "instr.perf"
      {"ba", opcodes_b},
#line 758 "instr.perf"
      {"ba,a", opcodes_ba_a},
#line 857 "instr.perf"
      {"deccc", opcodes_deccc},
#line 832 "instr.perf"
      {"cb13", opcodes_cb13},
#line 754 "instr.perf"
      {"cb13,a", opcodes_cb13_a},
#line 875 "instr.perf"
      {"std", opcodes_std},
#line 742 "instr.perf"
      {"stda", opcodes_stda},
#line 850 "instr.perf"
      {"cb123", opcodes_cb123},
#line 814 "instr.perf"
      {"cb123,a", opcodes_cb123_a},
#line 852 "instr.perf"
      {"swap", opcodes_swap},
#line 721 "instr.perf"
      {"swapa", opcodes_swapa},
#line 808 "instr.perf"
      {"tcc", opcodes_tcc},
#line 726 "instr.perf"
      {"tst", opcodes_tst},
#line 714 "instr.perf"
      {"bcc", opcodes_bcc},
#line 670 "instr.perf"
      {"btst", opcodes_btst},
#line 705 "instr.perf"
      {"bcc,a", opcodes_bcc_a},
#line 773 "instr.perf"
      {"bset", opcodes_bset},
#line 672 "instr.perf"
      {"tcs", opcodes_tcs},
#line 880 "instr.perf"
      {"srax", opcodes_srax},
#line 822 "instr.perf"
      {"fsubs", opcodes_fsubs},
#line 708 "instr.perf"
      {"ldstub", opcodes_ldstub},
#line 781 "instr.perf"
      {"ldstuba", opcodes_ldstuba},
#line 674 "instr.perf"
      {"lduh", opcodes_lduh},
#line 817 "instr.perf"
      {"lduha", opcodes_lduha},
#line 788 "instr.perf"
      {"tsubcc", opcodes_tsubcc},
#line 879 "instr.perf"
      {"bcs", opcodes_bcs},
#line 719 "instr.perf"
      {"bcs,a", opcodes_bcs_a},
#line 874 "instr.perf"
      {"srlx", opcodes_srlx},
#line 842 "instr.perf"
      {"sth", opcodes_sth},
#line 825 "instr.perf"
      {"stha", opcodes_stha},
#line 906 "instr.perf"
      {"tg", opcodes_tg},
#line 661 "instr.perf"
      {"addx", opcodes_addx},
#line 867 "instr.perf"
      {"tge", opcodes_tge},
#line 819 "instr.perf"
      {"ldsh", opcodes_ldsh},
#line 738 "instr.perf"
      {"ldsha", opcodes_ldsha},
#line 744 "instr.perf"
      {"bg", opcodes_bg},
#line 704 "instr.perf"
      {"tgu", opcodes_tgu},
#line 805 "instr.perf"
      {"bg,a", opcodes_bg_a},
#line 723 "instr.perf"
      {"fabss", opcodes_fabss},
#line 752 "instr.perf"
      {"addxcc", opcodes_addxcc},
#line 794 "instr.perf"
      {"bge", opcodes_bge},
#line 806 "instr.perf"
      {"tgeu", opcodes_tgeu},
#line 718 "instr.perf"
      {"bge,a", opcodes_bge_a},
#line 916 "instr.perf"
      {"bgu", opcodes_bgu},
#line 896 "instr.perf"
      {"bgu,a", opcodes_bgu_a},
#line 840 "instr.perf"
      {"fbo", opcodes_fbo},
#line 873 "instr.perf"
      {"bgeu", opcodes_bgeu},
#line 677 "instr.perf"
      {"fbo,a", opcodes_fbo_a},
#line 796 "instr.perf"
      {"bgeu,a", opcodes_bgeu_a},
#line 835 "instr.perf"
      {"fsubd", opcodes_fsubd},
#line 888 "instr.perf"
      {"or", opcodes_or},
#line 767 "instr.perf"
      {"nop", opcodes_nop},
#line 917 "instr.perf"
      {"cmp", opcodes_cmp},
#line 856 "instr.perf"
      {"cbn", opcodes_cbn},
#line 824 "instr.perf"
      {"orcc", opcodes_orcc},
#line 697 "instr.perf"
      {"cbn,a", opcodes_cbn_a},
#line 659 "instr.perf"
      {"tn", opcodes_tn},
#line 837 "instr.perf"
      {"tne", opcodes_tne},
#line 771 "instr.perf"
      {"bn", opcodes_bn},
#line 763 "instr.perf"
      {"fbn", opcodes_fbn},
#line 709 "instr.perf"
      {"bn,a", opcodes_bn_a},
#line 663 "instr.perf"
      {"fbn,a", opcodes_fbn_a},
#line 899 "instr.perf"
      {"bne", opcodes_bne},
#line 919 "instr.perf"
      {"fbne", opcodes_fbne},
#line 859 "instr.perf"
      {"bne,a", opcodes_bne_a},
#line 764 "instr.perf"
      {"fbne,a", opcodes_fbne_a},
#line 918 "instr.perf"
      {"sub", opcodes_sub},
#line 893 "instr.perf"
      {"umul", opcodes_umul},
#line 790 "instr.perf"
      {"save", opcodes_save},
#line 807 "instr.perf"
      {"subcc", opcodes_subcc},
#line 787 "instr.perf"
      {"umulcc", opcodes_umulcc},
#line 749 "instr.perf"
      {"rd", opcodes_rd},
#line 847 "instr.perf"
      {"not", opcodes_not},
#line 803 "instr.perf"
      {"and", opcodes_and},
#line 910 "instr.perf"
      {"smul", opcodes_smul},
#line 813 "instr.perf"
      {"fadds", opcodes_fadds},
#line 681 "instr.perf"
      {"andcc", opcodes_andcc},
#line 762 "instr.perf"
      {"taddcc", opcodes_taddcc},
#line 897 "instr.perf"
      {"fbg", opcodes_fbg},
#line 872 "instr.perf"
      {"fbg,a", opcodes_fbg_a},
#line 775 "instr.perf"
      {"smulcc", opcodes_smulcc},
#line 845 "instr.perf"
      {"fbge", opcodes_fbge},
#line 909 "instr.perf"
      {"fdtos", opcodes_fdtos},
#line 729 "instr.perf"
      {"fbge,a", opcodes_fbge_a},
#line 656 "instr.perf"
      {"fbug", opcodes_fbug},
#line 667 "instr.perf"
      {"fmuls", opcodes_fmuls},
#line 901 "instr.perf"
      {"fbug,a", opcodes_fbug_a},
#line 761 "instr.perf"
      {"tl", opcodes_tl},
#line 766 "instr.perf"
      {"sll", opcodes_sll},
#line 818 "instr.perf"
      {"fblg", opcodes_fblg},
#line 669 "instr.perf"
      {"fbuge", opcodes_fbuge},
#line 748 "instr.perf"
      {"fblg,a", opcodes_fblg_a},
#line 902 "instr.perf"
      {"fbuge,a", opcodes_fbuge_a},
#line 841 "instr.perf"
      {"tle", opcodes_tle},
#line 783 "instr.perf"
      {"bl", opcodes_bl},
#line 820 "instr.perf"
      {"tlu", opcodes_tlu},
#line 702 "instr.perf"
      {"bl,a", opcodes_bl_a},
#line 877 "instr.perf"
      {"ble", opcodes_ble},
#line 846 "instr.perf"
      {"tleu", opcodes_tleu},
#line 666 "instr.perf"
      {"ble,a", opcodes_ble_a},
#line 664 "instr.perf"
      {"blu", opcodes_blu},
#line 732 "instr.perf"
      {"blu,a", opcodes_blu_a},
#line 725 "instr.perf"
      {"cb0", opcodes_cb0},
#line 765 "instr.perf"
      {"bleu", opcodes_bleu},
#line 770 "instr.perf"
      {"cb0,a", opcodes_cb0_a},
#line 831 "instr.perf"
      {"bleu,a", opcodes_bleu_a},
#line 908 "instr.perf"
      {"ret", opcodes_ret},
#line 737 "instr.perf"
      {"cb02", opcodes_cb02},
#line 751 "instr.perf"
      {"faddd", opcodes_faddd},
#line 703 "instr.perf"
      {"cb02,a", opcodes_cb02_a},
#line 743 "instr.perf"
      {"tnz", opcodes_tnz},
#line 786 "instr.perf"
      {"bnz", opcodes_bnz},
#line 747 "instr.perf"
      {"retl", opcodes_retl},
#line 695 "instr.perf"
      {"bnz,a", opcodes_bnz_a},
#line 655 "instr.perf"
      {"fmuld", opcodes_fmuld},
#line 883 "instr.perf"
      {"subx", opcodes_subx},
#line 827 "instr.perf"
      {"fsubq", opcodes_fsubq},
#line 812 "instr.perf"
      {"xor", opcodes_xor},
#line 795 "instr.perf"
      {"stbar", opcodes_stbar},
#line 741 "instr.perf"
      {"subxcc", opcodes_subxcc},
#line 688 "instr.perf"
      {"cb01", opcodes_cb01},
#line 882 "instr.perf"
      {"xorcc", opcodes_xorcc},
#line 839 "instr.perf"
      {"cb01,a", opcodes_cb01_a},
#line 892 "instr.perf"
      {"rett", opcodes_rett},
#line 858 "instr.perf"
      {"cb012", opcodes_cb012},
#line 692 "instr.perf"
      {"cb012,a", opcodes_cb012_a},
#line 689 "instr.perf"
      {"jmp", opcodes_jmp},
#line 683 "instr.perf"
      {"tsubcctv", opcodes_tsubcctv},
#line 900 "instr.perf"
      {"fstod", opcodes_fstod},
#line 905 "instr.perf"
      {"cb03", opcodes_cb03},
#line 757 "instr.perf"
      {"fcmps", opcodes_fcmps},
#line 707 "instr.perf"
      {"cb03,a", opcodes_cb03_a},
#line 673 "instr.perf"
      {"jmpl", opcodes_jmpl},
#line 685 "instr.perf"
      {"cb023", opcodes_cb023},
#line 861 "instr.perf"
      {"fcmpes", opcodes_fcmpes},
#line 740 "instr.perf"
      {"cb023,a", opcodes_cb023_a},
#line 890 "instr.perf"
      {"sllx", opcodes_sllx},
#line 696 "instr.perf"
      {"mov", opcodes_mov},
#line 780 "instr.perf"
      {"bclr", opcodes_bclr},
#line 769 "instr.perf"
      {"cpop2", opcodes_cpop2},
#line 694 "instr.perf"
      {"lduw", opcodes_lduw},
#line 676 "instr.perf"
      {"lduwa", opcodes_lduwa},
#line 716 "instr.perf"
      {"wr", opcodes_wr},
#line 816 "instr.perf"
      {"fsmuld", opcodes_fsmuld},
#line 791 "instr.perf"
      {"cb013", opcodes_cb013},
#line 711 "instr.perf"
      {"cb013,a", opcodes_cb013_a},
#line 712 "instr.perf"
      {"orn", opcodes_orn},
#line 823 "instr.perf"
      {"fcmpd", opcodes_fcmpd},
#line 668 "instr.perf"
      {"tvc", opcodes_tvc},
#line 884 "instr.perf"
      {"fitos", opcodes_fitos},
#line 665 "instr.perf"
      {"fcmped", opcodes_fcmped},
#line 907 "instr.perf"
      {"udiv", opcodes_udiv},
#line 778 "instr.perf"
      {"orncc", opcodes_orncc},
#line 914 "instr.perf"
      {"bvc", opcodes_bvc},
#line 678 "instr.perf"
      {"bvc,a", opcodes_bvc_a},
#line 862 "instr.perf"
      {"flush", opcodes_flush},
#line 698 "instr.perf"
      {"cpop1", opcodes_cpop1},
#line 894 "instr.perf"
      {"udivcc", opcodes_udivcc},
#line 866 "instr.perf"
      {"inc", opcodes_inc},
#line 753 "instr.perf"
      {"faddq", opcodes_faddq},
#line 776 "instr.perf"
      {"tvs", opcodes_tvs},
#line 782 "instr.perf"
      {"sdiv", opcodes_sdiv},
#line 878 "instr.perf"
      {"inccc", opcodes_inccc},
#line 727 "instr.perf"
      {"mulscc", opcodes_mulscc},
#line 799 "instr.perf"
      {"fdtoq", opcodes_fdtoq},
#line 746 "instr.perf"
      {"bvs", opcodes_bvs},
#line 912 "instr.perf"
      {"bvs,a", opcodes_bvs_a},
#line 654 "instr.perf"
      {"fmulq", opcodes_fmulq},
#line 855 "instr.perf"
      {"sdivcc", opcodes_sdivcc},
#line 829 "instr.perf"
      {"tpos", opcodes_tpos},
#line 720 "instr.perf"
      {"andn", opcodes_andn},
#line 731 "instr.perf"
      {"fdivs", opcodes_fdivs},
#line 691 "instr.perf"
      {"taddcctv", opcodes_taddcctv},
#line 913 "instr.perf"
      {"bpos", opcodes_bpos},
#line 779 "instr.perf"
      {"fitod", opcodes_fitod},
#line 717 "instr.perf"
      {"bpos,a", opcodes_bpos_a},
#line 853 "instr.perf"
      {"neg", opcodes_neg},
#line 774 "instr.perf"
      {"tneg", opcodes_tneg},
#line 750 "instr.perf"
      {"andncc", opcodes_andncc},
#line 887 "instr.perf"
      {"fnegs", opcodes_fnegs},
#line 680 "instr.perf"
      {"bneg", opcodes_bneg},
#line 793 "instr.perf"
      {"sethi", opcodes_sethi},
#line 848 "instr.perf"
      {"bneg,a", opcodes_bneg_a},
#line 768 "instr.perf"
      {"btog", opcodes_btog},
#line 864 "instr.perf"
      {"clr", opcodes_clr},
#line 838 "instr.perf"
      {"fstoq", opcodes_fstoq},
#line 889 "instr.perf"
      {"fdmulq", opcodes_fdmulq},
#line 671 "instr.perf"
      {"fdtoi", opcodes_fdtoi},
#line 851 "instr.perf"
      {"fdivd", opcodes_fdivd},
#line 826 "instr.perf"
      {"fqtos", opcodes_fqtos},
#line 836 "instr.perf"
      {"fstoi", opcodes_fstoi},
#line 755 "instr.perf"
      {"clrb", opcodes_clrb},
#line 885 "instr.perf"
      {"fcmpq", opcodes_fcmpq},
#line 715 "instr.perf"
      {"fcmpeq", opcodes_fcmpeq},
#line 833 "instr.perf"
      {"fqtod", opcodes_fqtod},
#line 802 "instr.perf"
      {"fmovs", opcodes_fmovs},
#line 756 "instr.perf"
      {"fsqrts", opcodes_fsqrts},
#line 657 "instr.perf"
      {"fitoq", opcodes_fitoq},
#line 736 "instr.perf"
      {"clrh", opcodes_clrh},
#line 662 "instr.perf"
      {"fdivq", opcodes_fdivq},
#line 739 "instr.perf"
      {"fsqrtd", opcodes_fsqrtd},
#line 870 "instr.perf"
      {"xnor", opcodes_xnor},
#line 734 "instr.perf"
      {"xnorcc", opcodes_xnorcc},
#line 830 "instr.perf"
      {"fqtoi", opcodes_fqtoi},
#line 903 "instr.perf"
      {"fsqrtq", opcodes_fsqrtq},
#line 687 "instr.perf"
      {"restore", opcodes_restore}
    };

  static short lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,   0,   1,   2,   3,  -1,  -1,   4,  -1,
        5,  -1,   6,   7,  -1,  -1,  -1,   8,   9,  10,
       11,  -1,  -1,  12,  -1,  13,  -1,  -1,  14,  -1,
       15,  -1,  -1,  16,  17,  18,  19,  20,  21,  22,
       -1,  23,  -1,  -1,  24,  -1,  25,  -1,  -1,  -1,
       26,  27,  28,  29,  -1,  30,  -1,  31,  32,  33,
       -1,  34,  35,  36,  -1,  -1,  -1,  37,  -1,  38,
       -1,  -1,  39,  40,  41,  42,  -1,  -1,  43,  44,
       -1,  45,  -1,  -1,  46,  47,  -1,  -1,  48,  49,
       50,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  51,  52,
       53,  -1,  54,  55,  56,  -1,  -1,  -1,  57,  58,
       59,  -1,  60,  -1,  61,  -1,  -1,  -1,  -1,  -1,
       62,  -1,  -1,  -1,  63,  -1,  64,  -1,  65,  66,
       67,  -1,  68,  -1,  69,  70,  -1,  -1,  71,  -1,
       -1,  -1,  -1,  72,  -1,  -1,  -1,  -1,  73,  74,
       75,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  76,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  77,  78,
       79,  80,  81,  -1,  82,  83,  84,  -1,  85,  -1,
       86,  -1,  -1,  -1,  87,  -1,  -1,  -1,  88,  89,
       -1,  -1,  90,  -1,  91,  -1,  -1,  -1,  92,  93,
       94,  -1,  95,  96,  97,  98,  99,  -1, 100, 101,
      102,  -1,  -1, 103,  -1, 104,  -1,  -1, 105, 106,
      107, 108,  -1,  -1,  -1, 109,  -1, 110, 111,  -1,
       -1,  -1,  -1, 112,  -1,  -1,  -1,  -1, 113, 114,
      115,  -1, 116,  -1,  -1,  -1,  -1,  -1, 117,  -1,
       -1,  -1, 118, 119, 120, 121,  -1,  -1, 122, 123,
      124, 125,  -1, 126, 127,  -1,  -1,  -1,  -1, 128,
       -1,  -1,  -1,  -1,  -1, 129, 130, 131, 132,  -1,
       -1,  -1,  -1, 133,  -1,  -1,  -1,  -1,  -1, 134,
      135,  -1,  -1,  -1,  -1, 136, 137,  -1, 138,  -1,
      139, 140,  -1,  -1, 141, 142, 143,  -1,  -1, 144,
      145, 146, 147, 148, 149, 150, 151, 152, 153,  -1,
       -1,  -1, 154, 155, 156,  -1,  -1,  -1, 157, 158,
      159,  -1,  -1, 160,  -1, 161,  -1,  -1, 162, 163,
      164, 165,  -1, 166, 167, 168, 169,  -1, 170,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 171, 172,
      173,  -1,  -1,  -1,  -1, 174,  -1,  -1,  -1, 175,
      176,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 177,  -1,
      178, 179,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 180,
      181, 182,  -1,  -1, 183, 184,  -1, 185, 186,  -1,
       -1,  -1,  -1, 187,  -1, 188,  -1,  -1,  -1, 189,
      190, 191,  -1,  -1, 192, 193, 194, 195,  -1, 196,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 197, 198,
      199,  -1, 200, 201,  -1, 202,  -1,  -1,  -1,  -1,
       -1, 203,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      204,  -1, 205, 206,  -1, 207, 208,  -1,  -1,  -1,
      209, 210,  -1,  -1, 211, 212, 213,  -1, 214,  -1,
      215,  -1,  -1,  -1,  -1, 216, 217,  -1, 218,  -1,
      219,  -1,  -1,  -1,  -1,  -1, 220,  -1,  -1, 221,
      222, 223,  -1,  -1,  -1, 224, 225,  -1, 226,  -1,
      227, 228,  -1,  -1, 229,  -1,  -1,  -1,  -1, 230,
      231,  -1,  -1, 232, 233, 234, 235,  -1, 236, 237,
       -1, 238,  -1,  -1,  -1, 239,  -1,  -1,  -1, 240,
      241, 242,  -1,  -1, 243,  -1,  -1,  -1, 244,  -1,
      245, 246,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      247,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 248,  -1,  -1,  -1,  -1,
      249,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 250,  -1,  -1,  -1, 251,
      252,  -1,  -1,  -1,  -1,  -1, 253,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 254,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      255,  -1,  -1,  -1,  -1,  -1, 256,  -1,  -1,  -1,
      257,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 258,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      259, 260,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 261,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 262,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 263,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 264, 265
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
#line 920 "instr.perf"

