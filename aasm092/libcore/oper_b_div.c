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

#include <string.h>
#include <stdlib.h>

#include "aasm/expr.h"
#include "aasm/error.h"

static int		expr_eval_b_div(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  /* divide two number */
  if (old->right->flag & old->left->flag & EXPR_TYPE_NUM)
    {
      struct num_value_s	res;

      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      if (num_div(&old->left->val.num,
		  &old->right->val.num,
		  &res))
	error(ERRTYP_ERROR, "integer division error", old->location);
      *node = old->right;
      memcpy(&old->right->val.num, &res, sizeof (struct num_value_s));
      expr_node_free(old->left);
      expr_node_free(old);
    }

  return (0);
}

const struct operator_s	oper_b_div =
{
  &expr_eval_b_div,
  OPER_BINARY,
  200,
  NULL,
  "/"
};

