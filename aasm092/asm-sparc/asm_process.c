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

#include "aasm/section.h"
#include "aasm/error.h"
#include "aasm/expr.h"

#include "asm.h"
#include "fields.h"

static unsigned int
asm_op_type_get		(struct expr_s		*expr)
{
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_REGISTER):
      return (expr->val.reg->type);

    case (EXPR_TYPE_OPERATION):
      if (expr->val.op == &oper_u_obrack)
	{
	  if ((expr->right->flag & EXPR_TYPE_REGISTER)
	      && (expr->right->val.reg->type & OP_RREG(0)))
	    return (OP_REGMEM(0));
	  else
	    return (OP_NONREGMEM(0));
	}

    default:
      return (OP_IMM(0));
    }
}

void
asm_sparc_process	(struct instr_s		*instr,
			 struct section_s	*sec)
{
  unsigned int			i;
  unsigned int			op_type = 0;
  const struct instr_info_s	*info;

  if ((!(sec->asm_flag & FLG_WARNING))
      && (!(instr->asm_flag & FLG_MASK)))
    {
      error_submsg(error(ERRTYP_WARN, "sparc: no architecture version declared before `%s' instruction",
			 instr->location), instr->info->name);
      sec->asm_flag |= FLG_WARNING;
    }

  /* reduce each operand expression */
  for (i = 0; i < instr->op_count; i++)
    if (expr_reduce(&instr->operands[i]))
      return;

  for (i = 0; i < instr->op_count; i++)
    op_type |= asm_op_type_get(instr->operands[i]) << (i * OP_SHIFT);

  i = 0;
  while ((info = (instr->info + i))->asm_instr)
    {
      unsigned int		j;
      unsigned int		reg_flag;

      /* check operand counts */
      if (FLG_OPCOUNT(info->flags) != instr->op_count)
	goto continue_loop;

      for ((j = 0), (reg_flag = FLG_MASK);
	   j < instr->op_count; j++)
	{
	  /* check operand type */
	  if (!(info->op_type & op_type & OP_MASK(j)))
	    goto continue_loop;

	  /* check othrt register id if needed */
	  if ((info->op_type & OP_REGOTHER(j))
	      && (info->reg_other != instr->operands[j]->val.reg->other))
	    goto continue_loop;

	  /* update register architecture flag */
	  if (instr->operands[j]->flag & EXPR_TYPE_REGISTER)
	    reg_flag &= instr->operands[j]->val.reg->flag;
	}

      /* instruction form found */

      if (instr->asm_flag & FLG_MASK)
	{
	  /* check opcode availability on declared arch version */
	  if (!(instr->asm_flag & info->flags & FLG_MASK))
	    error_submsg(error(ERRTYP_WARN, "sparc: `%s' instruction form not available on declared architecture",
			       instr->location), instr->info->name);
	  else
	    /* check register availability on declared arch version */
	    if (!(instr->asm_flag & reg_flag))
	      error(ERRTYP_WARN, "sparc: register not available on declared architecture",
		    instr->location);
	}

      instr->info += i;

      /* check delay slot if any */
      if (instr->info->flags & FLG_DELAY_SLOT)
	{
	  switch (instr->next->flag & INSTR_TYPE_MASK)
	    {
	    case (INSTR_TYPE_MACRO):
	      error_submsg(error(ERRTYP_WARN_CTRL, "sparc: use of macro in `%s' instruction delay slot",
				 instr->location), instr->info->name);
	      break;

	    case (INSTR_TYPE_OPCODE):
	      if (instr->next->info->flags & FLG_DELAY_SLOT)
		error_submsg(error(ERRTYP_WARN_CTRL, "sparc: use of branch instruction in `%s' instruction delay slot",
				   instr->location), instr->info->name);
	      break;

	    default:
	      error_submsg(error(ERRTYP_WARN_CTRL, "sparc: no instruction in `%s' instruction delay slot",
				 instr->location), instr->info->name);
	    }
	}

      instr->info->asm_instr(instr, sec);

      return;

    continue_loop:
      i++;
    }

  error_submsg(error(ERRTYP_ERROR,
		     "sparc: no matching `%s' form found",
		     instr->location), instr->info->name);
}

