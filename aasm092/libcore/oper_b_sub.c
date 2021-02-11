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

#include <stdlib.h>

#include "aasm/expr.h"
#include "aasm/error.h"
#include "aasm/instr.h"
#include "aasm/symbol.h"

static int		expr_eval_b_sub(struct expr_s	**node)
{
  struct expr_s			*old = *node;

  /* reduce label symbols to numbers */
  if (old->right->flag & old->left->flag & EXPR_TYPE_SYMBOL)
    {
      struct symbol_s		*rsym = old->right->val.sym;
      struct symbol_s		*lsym = old->left->val.sym;

      if (rsym->flag & lsym->flag & SYMBOL_TYPE_LABEL)
	{
	  struct instr_s	*rinstr = rsym->val.instr;
	  struct instr_s	*linstr = lsym->val.instr;

	  if (rinstr->flag & linstr->flag & INSTR_ADDR)
	    {
	      if (rsym->section == lsym->section)
		{
		  /* reduce label to number */
		  num_init_uint(&old->right->val.num,
				rinstr->next->offset);
		  old->right->flag ^= EXPR_TYPE_NUM | EXPR_TYPE_SYMBOL;

		  /* reduce label to number */
		  num_init_uint(&old->left->val.num,
				linstr->next->offset);
		  old->left->flag ^= EXPR_TYPE_NUM | EXPR_TYPE_SYMBOL;
		}
	      else
		error(ERRTYP_WARN, "subtraction between labels from different sections",
		      old->location);
	    }
	}
    }

  /* subtract two numbers */
  if (old->right->flag & old->left->flag & EXPR_TYPE_NUM)
    {
      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      if (num_sub(&old->left->val.num,
		  &old->right->val.num,
		  &old->right->val.num))
	error(ERRTYP_ERROR, "subtraction overflow", old->location);
      *node = old->right;
      expr_node_free(old->left);
      expr_node_free(old);
      
      return (0);
    }

  /* reduce A - -B to A + B */
  if ((old->right->flag & EXPR_TYPE_OPERATION)
      && (old->right->val.op == &oper_u_neg))
    {
      struct expr_s	*tmp;

      tmp = old->right;
      old->right = old->right->right;
      expr_node_free(tmp);
      old->val.op = &oper_b_add;
      return (old->val.op->eval(node));
    }

  /* reduce A - A to 0 */
  if (expr_cmp(old->right, old->left))
    {
      old->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      old->flag ^= EXPR_TYPE_NUM | EXPR_TYPE_OPERATION;
      num_init(&old->val.num);
      expr_free(old->right);
      expr_free(old->left);

      return (0);
    }

  if (old->right->flag & EXPR_TYPE_NUM)
    {
      if (num_cmp_uint(&old->right->val.num, 0))
	{
	  /* reduce A - 0 to A */
	  old->left->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
	  *node = old->left;
	  expr_node_free(old->right);
	  expr_node_free(old);
	  
	  return (0);
	}
      else
	{
	  /* reduce A - n to A + (-n) */
	  old->val.op = &oper_b_add;
	  if (num_neg(&old->right->val.num))
	    error(ERRTYP_ERROR, "negation overflow", old->location);
	  return (0);
	}
    }

  /* reduce 0 - A to -A */
  if ((old->left->flag & EXPR_TYPE_NUM)
      && num_cmp_uint(&old->left->val.num, 0))
    {
      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      old->val.op = &oper_u_neg;
      expr_node_free(old->left);

      return (0);
    }

  return (0);
}

const struct operator_s	oper_b_sub =
{
  &expr_eval_b_sub,
  OPER_BINARY,
  100,
  NULL,
  "-"
};

