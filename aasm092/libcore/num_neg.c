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

int			num_neg(struct num_value_s		*num)
{
  unsigned int		n;
  int			x = 0;
  aasm_u8_t		old = num->val[AASM_MAX_VAL_SIZE - 1];

  if ((old & 0x80) && !(num->flag & NUM_SIGNED))
    return (-1);

  /* negate number */
  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    {
      x = -num->val[n] + x;
      num->val[n] = x & 0xff;
      x >>= 8;
    }

  /* set signed flag */
  num->flag |= NUM_SIGNED;

  /* return non 0 if sign bit didn't change */
  return (old & num->val[AASM_MAX_VAL_SIZE - 1] & 0x80);
}

