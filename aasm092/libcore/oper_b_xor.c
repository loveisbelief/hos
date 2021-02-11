/*
**    This file is part of AASM.
**
**    AASM is free software; you can redistribute it xor/or modify
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
**    Copyright Alexxorre Becoulet, 2002
**    contact : alexxorre.becoulet@free.fr
*/

#include <stdlib.h>

#include "aasm/expr.h"
#include "aasm/error.h"

static int		expr_eval_b_xor(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  /* xor two numbers */
  if (old->right->flag & old->left->flag & EXPR_TYPE_NUM)
    {
      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      num_xor(&old->right->val.num,
	      &old->left->val.num,
	      &old->right->val.num);
      *node = old->right;
      expr_node_free(old->left);
      expr_node_free(old);

      return (0);
    }

  return (0);
}

const struct operator_s	oper_b_xor =
{
  &expr_eval_b_xor,
  OPER_BINARY | OPER_COMMUT,
  40,
  NULL,
  "^"
};
