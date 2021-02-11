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

#include "aasm/num.h"

int			num_mul(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  struct num_value_s	a_, b_;
  unsigned int		n, m;
  unsigned int		sign = 0;
  int			carry = 0;

  num_init(res);

  memcpy(&a_, a, sizeof (struct num_value_s));
  memcpy(&b_, b, sizeof (struct num_value_s));

  if ((a_.flag & NUM_SIGNED) && (a_.val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      sign ^= 1;
      if (num_neg(&a_))
	return (-1);
    }

  if ((b_.flag & NUM_SIGNED) && (b_.val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      sign ^= 1;
      if (num_neg(&b_))
	return (-1);
    }

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    if (b_.val[n])
      {
	for (m = 0; m + n < AASM_MAX_VAL_SIZE; m++)
	  {
	    carry += a_.val[m] * b_.val[n] + res->val[m + n];
	    res->val[m + n] = carry & 0xff;
	    carry >>= 8;
	  }

	if (carry)
	  return (1);

	for (; m < AASM_MAX_VAL_SIZE; m++)
	  if (a_.val[m] && b_.val[n])
	    return (1);
      }

  if (sign)
    {
      if (num_neg(res))
	return (-1);
    }
  else
    res->flag &= ~NUM_SIGNED;

  return (0);
}

int			num_mul_uint(struct num_value_s		*num,
				     unsigned int		x)
{
  unsigned int		n;
  unsigned int		v = 0;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    {
      v += num->val[n] * x;
      num->val[n] = v & 0xff;
      v >>= 8;
    }

  return (v);
}

