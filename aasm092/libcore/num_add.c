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

int			num_add(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  unsigned int		sign_a = a->val[AASM_MAX_VAL_SIZE - 1] & 0x80;
  unsigned int		sign_b = b->val[AASM_MAX_VAL_SIZE - 1] & 0x80;
  unsigned int		n;
  unsigned int		carry = 0;

  /* add two numbers */
  for (n = 0; n < AASM_MAX_VAL_SIZE; n++)
    {
      carry += a->val[n] + b->val[n];
      res->val[n] = carry & 0xff;
      carry >>= 8;
    }

  /* get number type */
  if (a->flag & NUM_SIGNED)
    {
      if (b->flag & NUM_SIGNED)
	{
	  if (sign_a)
	    {
	      if (sign_b)
		{
		  if (res->val[AASM_MAX_VAL_SIZE - 1] & 0x80)
		    res->flag |= NUM_SIGNED;
		  else
		    return (-1);
		}
	      else
		res->flag |= NUM_SIGNED;
	    }
	  else
	    {
	      if (sign_b)
		res->flag |= NUM_SIGNED;
	      else
		res->flag &= ~NUM_SIGNED;
	    }
	}
      else
	{
	  if (sign_a)
	    {
	      if (carry)
		res->flag &= ~NUM_SIGNED;
	      else
		res->flag |= NUM_SIGNED;
	    }
	  else
	    {
	      if (carry)
		return (-1);
	      else
		res->flag &= ~NUM_SIGNED;
	    }
	}
    }
  else
    {
      if (b->flag & NUM_SIGNED)
	{
	  if (sign_b)
	    {
	      if (carry)
		res->flag &= ~NUM_SIGNED;
	      else
		res->flag |= NUM_SIGNED;
	    }
	  else
	    {
	      if (carry)
		return (-1);
	      else
		res->flag &= ~NUM_SIGNED;
	    }
	}
      else
	{
	  if (carry)
	    return (-1);
	  else
	    res->flag &= ~NUM_SIGNED;
	}
    }

  return (0);
}

int			num_add_uint(struct num_value_s		*num,
				     unsigned int		x)
{
  unsigned int		n;

  for (n = 0; x && (n < AASM_MAX_VAL_SIZE); n++)
    {
      x += num->val[n];
      num->val[n] = x & 0xff;
      x >>= 8;
    }   

  return (x);
}

