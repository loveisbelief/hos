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

#include "aasm/error.h"
#include "aasm/expr.h"

static int		expr_eval_u_neg(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  switch (old->right->flag & EXPR_TYPE_MASK)
    {
      /* remove double - */
    case (EXPR_TYPE_OPERATION):
      if (old->right->val.op == &oper_u_neg)
	{
	  *node = old->right->right;
	  expr_node_free(old->right);
	  expr_node_free(old);

	  return (0);
	}
      break;

      /* negate number */
    case (EXPR_TYPE_NUM):
      old->size = old->right->size;
      *node = old->right;
      if (num_neg(&old->right->val.num))
	error(ERRTYP_ERROR, "negation overflow", old->location);
      expr_node_free(old);

      return (0);
    }

  return (0);
}

const struct operator_s	oper_u_neg =
{
  &expr_eval_u_neg,
  OPER_UNARY,
  300,
  NULL,
  "-"
};
