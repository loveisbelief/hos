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

int			num_cmp(const struct num_value_s	*a,
				const struct num_value_s	*b)
{
  unsigned int		n;
  
  /* not equal if signed/unsigned and MSB set */
  if (((a->flag ^ b->flag) & NUM_SIGNED)
      && ((a->val[AASM_MAX_VAL_SIZE - 1] | b->val[AASM_MAX_VAL_SIZE - 1]) & 0x80))
    return (0);

  /* compare numbers */
  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    if (a->val[n] != b->val[n])
      return (0);

  return (-1);
}

int			num_cmp_uint(const struct num_value_s	*a,
				     unsigned int		b)
{
  unsigned int		n;

  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    {
      if (a->val[n] != (b & 0xff))
	return (0);
      b >>= 8;
    }

  return (-1);
}

