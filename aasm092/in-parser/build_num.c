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
#include <string.h>

#include "aasm/num.h"
#include "aasm/error.h"

#include "build_expr.h"
#include "scan.h"

static const int		build_num_val[256] =
  {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   /*    !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /  */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   /*0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?  */
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
   /*@   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O  */
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   /*P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _  */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   /*`   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o  */
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   /*p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~   .  */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
  };

static int		build_num_base(const char		*str,
				       struct num_value_s	*num,
				       unsigned int		base)
{
  unsigned int		i = 0;
  unsigned int		val_tmp = 0;
  unsigned int		base_tmp = 1;
  unsigned int		non_zero = 0;
  int			v;

  while (((v = build_num_val[(unsigned)str[i]]) >= 0) && (v < base))
    {
      i++;

      if (base_tmp > 0x800000)
	{
	  if (num_mul_uint(num, base_tmp)
	      || num_add_uint(num, val_tmp))
	    return (1);		/* overflow */
	  val_tmp = 0;
	  non_zero = base_tmp = 1;
	}
      base_tmp *= base;
      val_tmp = base * val_tmp + v;
    }

  if ((base_tmp > 1)
      && ((non_zero && num_mul_uint(num, base_tmp))
	  || num_add_uint(num, val_tmp)))
    return (1);		/* overflow */

  return (!i || str[i] ? -1 : 0); /* end of string and non empty ? */
}

static int	build_num_string(const char		*str,
				 struct num_value_s	*num)
{
  aasm_u8_t	buf[MAX_LINE_LEN];
  unsigned int	i, len;

  num_init(num);

  if (!(len = scan_string_escape((aasm_u8_t*)str, buf, '\'')))
    {
      /* empty quoted value */
      error(ERRTYP_WARN, "empty quoted value (zero)", 0);
      return (0);
    }

  if (len > AASM_MAX_VAL_SIZE)
    {
      error(ERRTYP_ERROR, "quoted value overflow", 0);
      return (-1);
    }

  /* copy buffer tail to num->val in reverse order */
  for (i = 0; i < len; i++)
    num->val[i] = buf[len - 1 - i];

  return (0);
}

int		build_expr_num(const char		*str,
			       struct num_value_s	*num)
{
  int		err;

  switch (str[0])
    {
    case ('\''):
      if (build_num_string(str + 1, num))
	return (-1);

      return (0);

    case ('0'):
      switch (str[1])
	{
	  /* 0 */
	case (0):
	  break;

	case ('x'):
	  /* 0x[0-9a-fA-F]+ hex value */
	  if ((err = build_num_base(str + 2, num, 16)))
	    {
	      if (err < 0)
		error_submsg(error(ERRTYP_ERROR,
				   "invalid hex value `%s'", 0), str);
	      else
		error_submsg(error(ERRTYP_ERROR,
				   "hex value `%s' overflow", 0), str);
	      return (-1);
	    }
	  break;
	  
	case ('b'):
	  /* 0b[01]+ bin value */
	  if ((err = build_num_base(str + 2, num, 2)))
	    {
	      if (err < 0)
		error_submsg(error(ERRTYP_ERROR,
				   "invalid binary value `%s'", 0), str);
	      else
		error_submsg(error(ERRTYP_ERROR,
				   "binary value `%s' overflow", 0), str);
	      return (-1);
	    }
	  break;

	default:
	  /* 0[0-7]* octal value */
	  if ((err = build_num_base(str + 1, num, 8)))
	    {
	      if (err < 0)
		error_submsg(error(ERRTYP_ERROR,
				   "invalid octal value `%s'", 0), str);
	      else
		error_submsg(error(ERRTYP_ERROR,
				   "octal value `%s' overflow", 0), str);
	      return (-1);
	    }
	}
      break;

    default:
      /* [1-9][0-9]* decimal value */
      if ((err = build_num_base(str, num, 10)))
	{
	  if (err < 0)
	    error_submsg(error(ERRTYP_ERROR,
			       "invalid numerical value `%s'", 0), str);
	  else
	    error_submsg(error(ERRTYP_ERROR,
			       "numerical value `%s' overflow", 0), str);
	  return (-1);
	}
    };

  return (0);
}
