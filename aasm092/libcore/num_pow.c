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

static int	mul(const aasm_u8_t	*a,
		    const aasm_u8_t	*b,
		    aasm_u8_t		*res)
{
  unsigned int          n, m;
  int                   carry = 0;
  
  memset(res, 0, AASM_MAX_VAL_SIZE);
  
  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    if (b[n])
      {
	for (m = 0; m + n < AASM_MAX_VAL_SIZE; m++)
	  {
	    carry += a[m] * b[n] + res[m + n];
	    res[m + n] = carry & 0xff;
	    carry >>= 8;
	  }
	
	if (carry)
	  return (1);

	for (; m < AASM_MAX_VAL_SIZE; m++)
	  if (a[m] && b[n])
	    return (1);
      }

  return (0);
}

int			num_pow(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  struct num_value_s	a_;
  unsigned int		sign = 0;
  aasm_u8_t		temp[AASM_MAX_VAL_SIZE];
  unsigned int		i, j;

  num_init(res);

  if ((b->flag & NUM_SIGNED) && (b->val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    return (0);

  memcpy(&a_, a, sizeof (struct num_value_s));

  if ((a_.flag & NUM_SIGNED) && (a_.val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      sign = b->val[0] & 1;
      if (num_neg(&a_))
	return (-1);
    }

  memset(res->val, 0, AASM_MAX_VAL_SIZE);
  res->val[0] = 1;

  memcpy(temp, a_.val, AASM_MAX_VAL_SIZE);

  for (i = 0; i < AASM_MAX_VAL_SIZE; i++)
    for (j = 0; j < 8; j++)
      {
	aasm_u8_t	temp2[AASM_MAX_VAL_SIZE];
	aasm_u8_t	temp3[AASM_MAX_VAL_SIZE];

	if (mul(temp, temp, temp2))
	  goto loop_end;

	if (b->val[i] & (1 << j))
	  {
	    if (mul(temp, res->val, temp3))
	      return (-1);

	    memcpy(res->val, temp3, AASM_MAX_VAL_SIZE);
	  }

	memcpy(temp, temp2, AASM_MAX_VAL_SIZE);
      }

 loop_end:

  for (; i < AASM_MAX_VAL_SIZE; i++)
    for (; j < 8; j++)
      if (b->val[i] & (1 << j))
	return (-1);

  if (sign)
    {
      if (num_neg(res))
	return (-1);
    }
  else
    res->flag &= ~NUM_SIGNED;

  return (0);
}

