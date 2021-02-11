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

#ifndef ASM_H_
# define ASM_H_

#include "aasm/types.h"
#include "aasm/mod_asm_import.h"

/*** regsiter descriptor struct ***/

struct				register_s
{
  const char			*name;		/* register name */
  unsigned int			num;

  unsigned int			size;		/* register size flag */
  unsigned int			type;
  unsigned short int		flag;
  unsigned short int		other;
};

/*** opcode descriptor struct ***/

typedef aasm_u32_t		opcode_bin_t;

# define ASM_INSTR_PARAMS	struct instr_s		*instr,\
				struct section_s	*sec

typedef void			(asm_instr_t)(ASM_INSTR_PARAMS);

struct				instr_info_s
{
  const char			*name;		/* instruction name */
  unsigned int			id;

  aasm_u32_t			op_type;
  aasm_u16_t			flags;
  aasm_u16_t			reg_other;	/* register id if op_type is OP_REGOTHER */
  opcode_bin_t			fix;		/* fixed binary data */

  asm_instr_t			*asm_instr;
};

/*
** asm_reloc.c
*/

struct			instr_s;
struct			expr_s;
struct			section_s;

opcode_bin_t
asm_set_simm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr);

opcode_bin_t
asm_set_rimm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr);

opcode_bin_t
asm_set_imm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr);

/*** exported fonction prototypes ***/

asm_get_instr_perf_t	asm_sparc_get_instr_perf;
asm_get_reg_t		asm_sparc_get_reg;
asm_process_t		asm_sparc_process;
asm_reg_size_t		asm_sparc_reg_size;
asm_reg_name_t		asm_sparc_reg_name;
asm_instr_name_t	asm_sparc_instr_name;
asm_max_binary_size_t	asm_sparc_max_binary_size;
asm_instr_help_t	asm_sparc_instr_help;
asm_directive_t		asm_sparc_directive;
asm_endian_permut_t	asm_sparc_endian_permut;

#endif

