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

static int		expr_eval_b_mul(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  /* multiply two number */
  if (old->right->flag & old->left->flag & EXPR_TYPE_NUM)
    {
      struct num_value_s	res;

      old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
      if (num_mul(&old->right->val.num,
		  &old->left->val.num,
		  &res))
	error(ERRTYP_ERROR, "multiplication overflow", old->location);
      *node = old->right;
      memcpy(&old->right->val.num, &res, sizeof (struct num_value_s));
      expr_node_free(old->left);
      expr_node_free(old);

      return (0);
    }

  if (old->left->flag & EXPR_TYPE_NUM)
    {
      /* reduce 1 * A = A */
      if (num_cmp_uint(&old->left->val.num, 1))
	{
	  old->right->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
	  *node = old->right;
	  expr_free(old->left);
	  expr_node_free(old);

	  return (0);
	}

      /* reduce 0 * A = 0 */
      if (num_cmp_uint(&old->left->val.num, 0))
	{
	  old->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
	  old->flag ^= EXPR_TYPE_NUM | EXPR_TYPE_OPERATION;
	  num_init(&old->val.num);
	  expr_free(old->right);
	  expr_free(old->left);

	  return (0);
	}
    }

  if (old->right->flag & EXPR_TYPE_NUM)
    {
      /* reduce A * 1 = A */
      if (num_cmp_uint(&old->right->val.num, 1))
	{
	  old->left->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
	  *node = old->left;
	  expr_free(old->right);
	  expr_node_free(old);

	  return (0);
	}
      
      /* reduce A * 0 = A */
      if (num_cmp_uint(&old->right->val.num, 0))
	{
	  old->size = SIZE_COMBINE_MAX(old->right->size, old->left->size);
	  old->flag ^= EXPR_TYPE_NUM | EXPR_TYPE_OPERATION;
	  num_init(&old->val.num);
	  expr_free(old->right);
	  expr_free(old->left);

	  return (0);
	}
    }

  return (0);
}

const struct operator_s	oper_b_mul =
{
  &expr_eval_b_mul,
  OPER_BINARY | OPER_COMMUT,
  200,
  NULL,
  "*"
};

