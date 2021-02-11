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
**    Copyright Alexandre Becoulet, 2002
**    contact : alexandre.becoulet@free.fr
*/

#ifndef INSTR_H_
# define INSTR_H_

# include "types.h"

/*
** instruction
*/

# define INSTR_TYPE_OPCODE	0x1 /* real cpu instruction */
# define INSTR_TYPE_DATA	0x2 /* data inserted using directive */
# define INSTR_TYPE_MACRO	0x4
# define INSTR_TYPE_UNINIT	0x8 /* uninitialized data */
# define INSTR_TYPE_PROC	0x10

# define INSTR_TYPE_MASK	(INSTR_TYPE_OPCODE | INSTR_TYPE_DATA\
				| INSTR_TYPE_MACRO | INSTR_TYPE_UNINIT\
				| INSTR_TYPE_PROC)

# define INSTR_ASM		0x20 /* binary form present */
# define INSTR_ADDR		0x40 /* address present */
# define INSTR_SOURCE		0x80 /* source form present */

# define INSTR_ALIGN_OFFSET	0x100
# define INSTR_ALIGN_BOUNDARY	0x200

# define INSTR_ALIGN_MASK	(INSTR_ALIGN_OFFSET | INSTR_ALIGN_BOUNDARY)

# define INSTR_MACRO_DONE	0x1000 /* macro replacement done */

struct				instr_info_s;
struct				expr_s;
struct				error_locat_s;
struct				comment_s;
struct				symbol_s;
struct				section_s;
struct				scope_s;
struct				proc_s;

struct				instr_s
{
  unsigned int			bin_size;
  unsigned int			flag;
  unsigned int			asm_flag; /* asm module specific flag copyed from section asm_flag */
  unsigned int			align;	/* next instruction alignment */

  struct instr_s		*next;
  struct instr_s		*prev;

  struct symbol_s		*label;   /* pointer to next label symbol */

  unsigned int			op_count;

  /* present if INSTR_ADDR */
  unsigned int			offset;	/* opcode offset in section buffer, once assembled */

  /* present if INSTR_SOURCE */
  const struct instr_info_s	*info;	/* pointer to module opcode const data */
  struct expr_s			**operands;
  struct error_locat_s		*location;

  /* present if INSTR_ASM */
  unsigned int			len;	/* opcode len */

  /* comment for DEBUG only */
  struct comment_s		*comments;

  union
  {
    struct symbol_s		*macro;
    struct proc_s		*proc;
    aasm_u8_t			*bin;
  }				val;
};

struct				instr_perf_s
{
  const char			*name;
  const struct instr_info_s	*info;
};

struct instr_s		*instr_new(struct section_s		*sec,
				   struct scope_s		*scope,
				   const struct instr_info_s	*info,
				   unsigned int			bin_size,
				   unsigned int			op_count);

struct instr_s		*instr_dup_src(struct instr_s		*instr,
				       struct instr_s		*prev,
				       struct error_locat_s	*location);

void			instrs_free (struct scope_s		*scp);

void			instr_set_source(struct instr_s		*instr,
					 struct error_locat_s	*location,
					 unsigned int		flag);


void			instr_set_asm(struct instr_s		*instr,
				      unsigned int		len);

int			instr_help(char				*name);

void			instr_clean (void);
void			instr_init (void);

#endif


