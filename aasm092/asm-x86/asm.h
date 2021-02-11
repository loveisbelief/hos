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

#ifndef ASM_H_
# define ASM_H_

#include "aasm/types.h"
#include "aasm/mod_asm_import.h"

struct				instr_s;
struct				section_s;
struct				expr_s;

/*** regsiter descriptor struct ***/

struct				register_s
{
  char				name[8];
  unsigned int			num;		/* register number used in opcodes */
  unsigned int			r_type;
  unsigned int			r_size;
  unsigned int			id;		/* register id */
  unsigned int			overlap;	/* register overlap mask */
  unsigned int			mem;		/* used as memory table index */
};

/*** opcode descriptor struct ***/

#define X86_MAXOP_CNT		4

struct				instr_info_s
{
  const char			*name;
  const char			*desc;

  unsigned int			id;

  unsigned int			i_size[X86_MAXOP_CNT];
  unsigned int			i_type[X86_MAXOP_CNT];
  unsigned int			i_flag;

  aasm_u8_t			fix[8];

  /* function used to generate bin opcode */
  void				(*asm_instr)(struct instr_s		*instr,
					     struct section_s		*sec,
					     unsigned int		*op_flag,
					     unsigned int		*op_size);
};

/*** asm section flag ***/

# define ASM_SECTION_CODE16	0x1
# define ASM_SECTION_CODE32	0x2
# define ASM_SECTION_WARNING	0x4 /* warning message already displayed */

/*** bin opcode generation functions ***/

# define ASM_I_PARAMS		struct instr_s			*instr,\
				struct section_s		*sec,\
				unsigned int			*op_flag,\
				unsigned int			*op_size

void		asm_i_2rm		(ASM_I_PARAMS);
void		asm_i_1rm1im		(ASM_I_PARAMS);
void		asm_i_1im		(ASM_I_PARAMS);
void		asm_i_2im		(ASM_I_PARAMS);
void		asm_i_inout		(ASM_I_PARAMS);
void		asm_i_1r_short		(ASM_I_PARAMS);
void		asm_i_1rm		(ASM_I_PARAMS);
void		asm_i_void		(ASM_I_PARAMS);

unsigned int	asm_x86_set_mem	(struct instr_s		*instr,
				 unsigned int		offset,
				 struct expr_s		*expr_,
				 struct section_s	*sec);


void		asm_x86_set_factor(struct instr_s	*instr,
				   unsigned int		offset,
				   struct section_s	*sec,
				   struct expr_s	*expr);

unsigned int	asm_x86_set_imm_iexpr(struct instr_s	*instr,
				      unsigned int	offset,
				      unsigned int	size,
				      struct section_s	*sec,
				      struct expr_s	**expr);

unsigned int	asm_x86_set_imm_riexpr(struct instr_s	*instr,
				       unsigned int	offset,
				       unsigned int	size,
				       struct section_s	*sec,
				       struct expr_s	**expr);

unsigned int	asm_x86_set_imm_expr(struct instr_s	*instr,
				     unsigned int	offset,
				     unsigned int	size,
				     struct section_s	*sec,
				     struct expr_s	*expr);

asm_get_instr_perf_t	asm_x86_get_instr_perf;
asm_get_reg_t		asm_x86_get_reg;
asm_process_t		asm_x86_process;
asm_reg_size_t		asm_x86_reg_size;
asm_reg_name_t		asm_x86_reg_name;
asm_instr_name_t	asm_x86_instr_name;
asm_max_binary_size_t	asm_x86_max_binary_size;
asm_instr_help_t	asm_x86_instr_help;
asm_directive_t		asm_x86_directive;
asm_endian_permut_t	asm_x86_endian_permut;

#endif

