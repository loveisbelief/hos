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

#include "aasm/num.h"

int			num_and(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  unsigned int		n;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    res->val[n] = a->val[n] & b->val[n];

  res->flag = a->flag | b->flag;

  return (0);
}

int			num_or(const struct num_value_s	*a,
			       const struct num_value_s	*b,
			       struct num_value_s	*res)
{
  unsigned int		n;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    res->val[n] = a->val[n] | b->val[n];

  res->flag = a->flag | b->flag;

  return (0);
}

int			num_xor(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  unsigned int		n;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    res->val[n] = a->val[n] ^ b->val[n];

  res->flag = a->flag | b->flag;

  return (0);
}

int			num_not(struct num_value_s		*a)
{
  unsigned int		n;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    a->val[n] = ~a->val[n];

  a->flag |= NUM_SIGNED;

  return (0);
}

