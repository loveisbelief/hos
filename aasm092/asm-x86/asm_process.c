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

#include "aasm/types.h"
#include "aasm/expr.h"
#include "aasm/instr.h"
#include "aasm/section.h"

#include "flags.h"
#include "asm.h"

static unsigned int		asm_op_type_get(struct expr_s		*expr)
{
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_REGISTER):
      return (expr->val.reg->r_type);

    case (EXPR_TYPE_OPERATION):
      if (expr->val.op == &oper_u_obrack)
	return (OP_TYPE_MEM);

    default:
      return (OP_TYPE_IM);
    }
}

void		asm_x86_process(struct instr_s		*instr,
				struct section_s	*sec)
{
  unsigned int		i;
  unsigned int		op_type[X86_MAXOP_CNT] = {0};
  unsigned int		op_size[X86_MAXOP_CNT] = {0};

  if (!(instr->asm_flag & (ASM_SECTION_CODE16 | ASM_SECTION_CODE32))
      && (instr->flag & INSTR_TYPE_OPCODE)
      && (!(sec->asm_flag & ASM_SECTION_WARNING)))
    {
      error_submsg(error(ERRTYP_WARN,
			 "x86: cpu size mode undefined before instruction, 32 bits mode assumed",
			 instr->location), instr->info->name);

      sec->asm_flag |= ASM_SECTION_WARNING;
    }

  /* check CPU extension declarartion */
  if ((instr->info->i_flag & I_ALL_EXT & instr->asm_flag)
      != (instr->info->i_flag & I_ALL_EXT))
    error_submsg(error(ERRTYP_WARN_MORE,
		       "x86: instruction `%s' is part of undeclared cpu extension",
		       instr->location), instr->info->name);

  /* check CPU instruction */
  if ((instr->info->i_flag & I_ALL_X86)
      && !(instr->info->i_flag & I_ALL_X86 & (instr->asm_flag | I_386O)))
    error_submsg(error(ERRTYP_WARN_MORE,
		       "x86: instruction `%s' is not available on declared cpu",
		       instr->location), instr->info->name);

  /* reduce each operand expression */
  for (i = 0; i < instr->op_count; i++)
    if (expr_reduce(&instr->operands[i]))
      return;

  /* check operand count limit */
  if (instr->op_count > 3)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "x86: invalid operand count for %s",
			 instr->location), instr->info->name);
      return;
    }

  /* prepare operands type and size mask */
  for (i = 0; i < instr->op_count; i++)
    {
      op_type[i] = asm_op_type_get(instr->operands[i]);
      op_size[i] = (instr->operands[i]->size & SIZE_MASK);
    }

  /* check for two memory operands */
  if (op_type[0] & op_type[1] & OP_TYPE_MEM)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "x86: two memory operands for %s",
			 instr->location), instr->info->name);
      return;
    }

  i = 0;

  while (instr->info[i].asm_instr)
    {
      /* check operand count match */
      if (instr->op_count != (instr->info[i].i_flag & I_OP_COUNT_MASK))
	goto continue_lbl;

      if (instr->op_count)
	{
	  unsigned int	j;

	  /* check operand types match */
	  for (j = 0; j < instr->op_count; j++)
	    if ((op_type[j] & instr->info[i].i_type[j]) != op_type[j])
	      goto continue_lbl;

	  /* check operand 1 & 2 size match */
	  if ((instr->info[i].i_flag & I_SELECT_SIZE_OP12) &&
	      (((op_size[0] & instr->info[i].i_size[0]) ^ op_size[0]) ||
	       ((op_size[1] & instr->info[i].i_size[1]) ^ op_size[1])))
	    goto continue_lbl;

	  /* check operand 3 size match */
	  if ((instr->info[i].i_flag & I_SELECT_SIZE_OP3) &&
	      (((op_size[2] & instr->info[i].i_size[2]) ^ op_size[2])))
	    goto continue_lbl;

	  /* deduce operand size */
	  if ((instr->op_count > 1)
	      && (instr->info[i].i_flag & I_FLAG_OP_SAME_SIZE))
	    {
	      /* deduce for same sized operands */
	      op_size[0] = op_size[1] = op_size[0] & op_size[1] & instr->info[i].i_size[0];
	    }
	  else
	    {
	      /* deduce for each operand */
	      op_size[0] &= instr->info[i].i_size[0];
	      op_size[1] &= instr->info[i].i_size[1];
	    }
	  op_size[2] &= instr->info[i].i_size[2];

	  /* check first operand size exist */
	  if (!op_size[0])
	    {
	      error_submsg(error(ERRTYP_ERROR,
				 "x86: invalid first operand size for %s",
				 instr->location), instr->info->name);
	      return;
	    }

	  /* check first operand size is uniq */
	  if ((op_size[0] - 1) & op_size[0])
	    {
	      error_submsg(error(ERRTYP_ERROR,
				 "x86: can't deduce first operand size for %s",
				 instr->location), instr->info->name);
	      return;
	    }

	  if (instr->op_count > 1)
	    {
	      /* check second operand size exist */
	      if (!op_size[1])
		{
		  error_submsg(error(ERRTYP_ERROR,
				     "x86: invalid second operand size for %s",
				     instr->location), instr->info->name);
		  return;
		}

	      /* check second operand size is uniq */
	      if ((op_size[1] - 1) & op_size[1])
		{
		  error_submsg(error(ERRTYP_ERROR,
				     "x86: can't deduce second operand size for %s",
				     instr->location), instr->info->name);
		  return;
		}

	      if ((instr->op_count > 2) && !op_size[2])
		{
		  error_submsg(error(ERRTYP_ERROR,
				     "x86: invalid third operand size for %s",
				     instr->location), instr->info->name);
		  return;
		}
	    }
	}

      instr->info += i;
      instr->info->asm_instr(instr, sec, op_type, op_size);

      return;

    continue_lbl:
      i++;
    }

  error_submsg(error(ERRTYP_ERROR,
		     "x86: no matching `%s' form found",
		     instr->location), instr->info->name);
}

