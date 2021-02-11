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

static int		expr_eval_b_separ(struct expr_s	**node)
{
  struct expr_s		*old = *node;

  error(ERRTYP_WARN, "expression parts before `,' are discarded", old->location);
  /* keep right part */
  *node = old->right;
  expr_free(old->left);
  expr_node_free(old);

  return (0);
}

const struct operator_s	oper_b_separ =
{
  &expr_eval_b_separ,
  OPER_BINARY | OPER_RIGHT_ASSOCIAT,
  1,
  NULL,
  ","
};

const struct operator_s	oper_b_cparen =
{
  NULL,
  OPER_BRACE_CLOSE,
  600,
  NULL,
  ")"
};

const struct operator_s	oper_u_oparen =
{
  NULL,
  OPER_BRACE_OPEN,
  600,
  &oper_b_cparen,
  "("
};

