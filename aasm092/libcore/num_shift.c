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

int			num_shl(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  int			ret = 0;
  unsigned int		n;
  unsigned int		x;

  if (num_get_uint(b, &x) || (x >= AASM_MAX_VAL_SIZE * 8))
    {
      ret = -1;
      memset(res->val, 0, AASM_MAX_VAL_SIZE);
    }    
  else
    {
      if (!x)
	memcpy(res->val, a->val, AASM_MAX_VAL_SIZE);
      else
	{
	  unsigned int	byte_shift = x >> 3;

	  memset(res->val, 0, AASM_MAX_VAL_SIZE);

	  for (n = 0; n < AASM_MAX_VAL_SIZE - byte_shift; n++)
	    res->val[n + byte_shift]
	      = (a->val[n] << (x & 0x7))
	      | (n ? (a->val[n - 1] >> (8 - (x & 0x7))) : 0);

	  for (n = AASM_MAX_VAL_SIZE - byte_shift; n < AASM_MAX_VAL_SIZE; n++)
	    if ((a->val[n] << (x & 0x7)) | ((a->val[n - 1] >> (8 - (x & 0x7)))))
	      {
		ret = -1;
		break;
	      }
	}
    }

  res->flag = a->flag | b->flag;

  return (ret);
}

int			num_shr(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res)
{
  int			ret = 0;
  unsigned int		n;
  unsigned int		x;

  if (num_get_uint(b, &x) || (x >= AASM_MAX_VAL_SIZE * 8))
    {
      ret = -1;
      memset(res->val, 0, AASM_MAX_VAL_SIZE);
    }
  else
    {
      if (!x)
	memcpy(res->val, a->val, AASM_MAX_VAL_SIZE);
      else
	{
	  unsigned int	byte_shift = x >> 3;

	  memset(res->val, 0, AASM_MAX_VAL_SIZE);

	  for (n = byte_shift; n < AASM_MAX_VAL_SIZE; n++)
	    res->val[n - byte_shift]
	      = (a->val[n] >> (x & 0x7))
	      | (n < AASM_MAX_VAL_SIZE - 1 ? (a->val[n + 1] << (8 - (x & 0x7))) : 0);

	  for (n = 0; n < byte_shift; n++)
	    if ((a->val[n] >> (x & 0x7)) | ((a->val[n + 1] << (8 - (x & 0x7))) & 0xff))
	      {
		ret = -1;
		break;
	      }
	}
    }

  res->flag = a->flag | b->flag;

  return (ret);
}

