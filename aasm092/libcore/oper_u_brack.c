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

static int	expr_eval_u_obrack(struct expr_s	**node)
{
  return (0);
}

const struct operator_s	oper_b_cbrack =
{
  NULL,
  OPER_BRACE_CLOSE,
  600,
  NULL,
  "]"
};

const struct operator_s	oper_u_obrack =
{
  &expr_eval_u_obrack,
  OPER_BRACE_OPEN | OPER_UNARY | OPER_NO_NUM_EVAL,
  600,
  &oper_b_cbrack,
  "["
};

