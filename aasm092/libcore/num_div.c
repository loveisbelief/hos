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

static const aasm_u8_t	bit_size[256] = 
{
  0,
  1,
  2, 2,
  3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

static int	buf_cmp(const aasm_u8_t		*a,
			const aasm_u8_t		*b)
{
  unsigned int	i = AASM_MAX_VAL_SIZE;

  while (i--)
    if (a[i] != b[i])
      return (a[i] - b[i]);
  return (0);
}

static void		mod_div(const aasm_u8_t		*a,
				const aasm_u8_t		*b,
				aasm_u8_t		*div,
				aasm_u8_t		*mod)
{
  unsigned int		b_idx = AASM_MAX_VAL_SIZE;
  unsigned int		a_idx = AASM_MAX_VAL_SIZE;

  memcpy(mod, a, AASM_MAX_VAL_SIZE);
  memset(div, 0, AASM_MAX_VAL_SIZE);

  while (b_idx--)
    if (b[b_idx])
      break;

  while (a_idx--)
    if (mod[a_idx])
      break;

  while ((a_idx > b_idx)
	 || ((a_idx == b_idx)
	     && (buf_cmp(mod, b) >= 0)))
    {
      int		bit_shift = bit_size[mod[a_idx]] - bit_size[b[b_idx]];
      unsigned int	byte_shift = a_idx - b_idx;
      unsigned int	factor;

      if (bit_shift <= 0)
	{
	  if (byte_shift)
	    {
	      factor = 0x80 >> (-bit_shift + 1);
	      byte_shift--;
	    }
	  else
	    factor = 1;
	}
      else
	factor = 1 << (bit_shift - 1);

      if (!factor)
	factor = 1;

      {
	unsigned int	i, j;
	int		carry = 0;

	for ((i = byte_shift), (j = 0);
	     (i <= a_idx);
	     i++, j++)
	  {
	    carry = mod[i] - b[j] * factor + carry;
	    mod[i] = carry & 0xff;
	    carry >>= 8;
	  }

	while (a_idx && !mod[a_idx])
	  a_idx--;

	carry = factor;
	for (i = byte_shift; carry; i++)
	  {
	    carry += div[i];
	    div[i] = carry & 0xff;
	    carry >>= 8;
	  }
      }
    }
}

int			num_div(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  struct num_value_s	tmp;
  struct num_value_s	a_, b_;
  unsigned int		sign = 0;

  num_init(res);

  if (num_cmp_uint(b, 0))
    return (-1);

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

  mod_div(a_.val, b_.val, res->val, tmp.val);

  if (sign)
    {
      if (num_neg(res))
	return (-1);
    }
  else
    res->flag &= ~NUM_SIGNED;

  return (0);
}

int			num_mod(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  struct num_value_s	tmp;
  struct num_value_s	a_, b_;
  unsigned int		sign = 0;

  num_init(res);

  if (num_cmp_uint(b, 0))
    return (-1);

  memcpy(&a_, a, sizeof (struct num_value_s));
  memcpy(&b_, b, sizeof (struct num_value_s));

  if ((a_.flag & NUM_SIGNED) && (a_.val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      sign = 1;
      if (num_neg(&a_))
	return (-1);
    }

  if ((b_.flag & NUM_SIGNED) && (b_.val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      if (num_neg(&b_))
	return (-1);
    }

  mod_div(a_.val, b_.val, tmp.val, res->val);

  if (sign)
    {
      if (num_neg(res))
	return (-1);
    }
  else
    res->flag &= ~NUM_SIGNED;

  return (0);
}

