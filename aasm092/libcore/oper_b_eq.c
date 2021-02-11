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

static int		expr_eval_b_eq(struct expr_s	**node)
{
  struct expr_s			*old = *node;

  old->size = SIZE_UNDEF;
  old->flag &= ~EXPR_TYPE_MASK;
  old->flag |= EXPR_TYPE_NUM;

  if (expr_cmp(old->right, old->left))
    num_init_uint(&old->val.num, 1);
  else
    num_init(&old->val.num);

  expr_free(old->right);
  expr_free(old->left);

  return (0);
}

const struct operator_s	oper_b_eq =
{
  &expr_eval_b_eq,
  OPER_BINARY,
  10,
  NULL,
  "="
};

