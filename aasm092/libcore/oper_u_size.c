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

static int		expr_eval_u_size(struct expr_s	**node)
{
  struct expr_s		*old = *node;
  unsigned int		size;

  *node = old->right;
  if (!(size = old->right->size & old->val.op->size))
    error_submsg(error(ERRTYP_ERROR,
		       "size operator `%s' can't be used here",
		       old->location), old->val.op->token);
  else
    old->right->size = size;
  expr_node_free(old);

  return (0);
}

const struct operator_s	oper_u_size_byte =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "byte",
  SIZE_FLAG_BYTE
};

const struct operator_s	oper_u_size_word =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "word",
  SIZE_FLAG_WORD
};

const struct operator_s	oper_u_size_dword =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "dword",
  SIZE_FLAG_DWORD
};

const struct operator_s	oper_u_size_fword =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "fword",
  SIZE_FLAG_FWORD
};

const struct operator_s	oper_u_size_qword =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "qword",
  SIZE_FLAG_QWORD
};

const struct operator_s	oper_u_size_tword =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "tword",
  SIZE_FLAG_TWORD
};

const struct operator_s	oper_u_size_oword =
{
  &expr_eval_u_size,
  OPER_UNARY | OPER_SIZE_OPERATOR,
  300,
  NULL,
  "oword",
  SIZE_FLAG_OWORD
};

