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

static int		expr_eval_b_add(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  /* add two numbers */
  if (old->right->flag & old->left->flag & EXPR_TYPE_NUM)
    {
      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      if (num_add(&old->right->val.num,
		  &old->left->val.num,
		  &old->right->val.num))
	error(ERRTYP_ERROR, "sum overflow", old->location);
      *node = old->right;
      expr_node_free(old->left);
      expr_node_free(old);

      return (0);
    }

  /* reduce A + -B to A - B */
  if ((old->right->flag & EXPR_TYPE_OPERATION)
      && (old->right->val.op == &oper_u_neg))
    {
      struct expr_s	*tmp;

      tmp = old->right;
      old->right = old->right->right;
      expr_node_free(tmp);
      old->val.op = &oper_b_sub;
      return (old->val.op->eval(node));
    }

  /* reduce 0 + A to A */
  if ((old->left->flag & EXPR_TYPE_NUM)
      && num_cmp_uint(&old->left->val.num, 0))
    {
      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      *node = old->right;
      expr_node_free(old->left);
      expr_node_free(old);

      return (0);
    }

  /* reduce A + 0 to A */
  if ((old->right->flag & EXPR_TYPE_NUM)
      && num_cmp_uint(&old->right->val.num, 0))
    {
      old->left->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      *node = old->left;
      expr_node_free(old->right);
      expr_node_free(old);
      
      return (0);
    }

  return (0);
}

const struct operator_s	oper_b_add =
{
  &expr_eval_b_add,
  OPER_BINARY | OPER_COMMUT,
  100,
  NULL,
  "+"
};
