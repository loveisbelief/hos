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

#include <stdio.h>
#include <string.h>

#include "aasm/num.h"
#include "aasm/error.h"

void
num_init		(struct num_value_s		*num)
{
  memset(num, 0, sizeof (struct num_value_s));
}

void
num_init_uint		(struct num_value_s		*num,
			 unsigned int			x)
{
  unsigned int		n;

  num->flag = 0;

  for (n = sizeof (unsigned int); n < AASM_MAX_VAL_SIZE; n++)
    num->val[n] = 0;

  for (n = 0; n < sizeof (unsigned int); n++)
    {
      num->val[n] = x & 0xff;
      x >>= 8;
    }   
}

void
num_init_int		(struct num_value_s		*num,
			 int				x)
{
  unsigned int		n;

  num->flag = NUM_SIGNED;

  if (x >= 0)
    for (n = sizeof (int); n < AASM_MAX_VAL_SIZE; n++)
      num->val[n] = 0;
  else
    for (n = sizeof (int); n < AASM_MAX_VAL_SIZE; n++)
      num->val[n] = 0xff;
    
  for (n = 0; n < sizeof (int); n++)
    {
      num->val[n] = x & 0xff;
      x >>= 8;
    }
}

#ifdef DEBUG

void
num_disp		(const struct num_value_s	*num)
{
  int			n;

  for (n = AASM_MAX_VAL_SIZE - 1; (n > 0) && !num->val[n]; n--)
    ;

  fputs("value=", stderr);
  for (; n >= 0; n--)
    fprintf(stderr, "%02x", num->val[n]);

  fprintf(stderr, ", len=%i, sign=%c",
	  num_get_byte_size(num), 
	  num->flag & NUM_SIGNED ? 's' : 'u');
}

#endif

void
num_fout		(const struct num_value_s	*num_,
			 FILE				*out)
{
  int				n;
  const struct num_value_s	*num = num_;
  struct num_value_s		numneg;

  if ((num->flag & NUM_SIGNED)
      && (num->val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      num = memcpy(&numneg, num, sizeof (struct num_value_s));
      if (!num_neg(&numneg))
	fputc('-', out);
    }

  fprintf(out, "0x");

  /* skip zero */
  for (n = AASM_MAX_VAL_SIZE - 1; (n > 0) && !num->val[n]; n--);

  /* display single digit */
  if ((n >= 0) && !(num->val[n] & 0xf0))
    {
      fprintf(out, "%1x", num->val[n]);
      n--;
    }

  /* display remaining digits */
  for (; n >= 0; n--)
    fprintf(out, "%02x", num->val[n]);
}

int
num_get_uint		(const struct num_value_s	*num,
			 unsigned int			*val)
{
  unsigned int		res = 0;
  int			n;

  for (n = sizeof (unsigned int) - 1; n >= 0; n--)
    {
      res <<= 8;
      res |= num->val[n];
    }

  for (n = sizeof (unsigned int); n < AASM_MAX_VAL_SIZE; n++)
    if (num->val[n])
      return (-1);

  *val = res;
  return (0);
}

int
num_get_int		(const struct num_value_s	*num,
			 int				*val)
{
  int			res = 0;
  int			n;

  for (n = sizeof (unsigned int) - 1; n >= 0; n--)
    {
      res <<= 8;
      res |= num->val[n];
    }

  if ((num->flag & NUM_SIGNED) && (num->val[AASM_MAX_VAL_SIZE - 1] & 0x80))
    {
      for (n = sizeof (unsigned int); n < AASM_MAX_VAL_SIZE; n++)
	if (num->val[n] != 0xff)
	  return (-1);
    }
  else
    {
      for (n = sizeof (unsigned int); n < AASM_MAX_VAL_SIZE; n++)
	if (num->val[n])
	  return (-1);
    }

  *val = res;
  return (0);
}

unsigned int
num_is_mask		(const struct num_value_s	*num)
{
  unsigned int		i;
  int			tmp;
  static const int	scan_mask[0x100] =
    {
       0,  1, -1,  2, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1,  4,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  6,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  7,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  8,
    };

  for (i = 0; i < AASM_MAX_VAL_SIZE - 1; i++)
    if (num->val[i] != 0xff)
      break;

  if ((tmp = scan_mask[num->val[i]]) < 0)
    return (0);

  return (i * 8 + tmp);
}

int
num_get_bit_size	(const struct num_value_s	*num,
			 unsigned int			unsigned_target)
{
  unsigned int	i, size;

  if (num->flag & NUM_SIGNED)
  {
    if (num->val[AASM_MAX_VAL_SIZE - 1] & 0x80)
      {
	static const unsigned int	scan_set[0x100] =
	  {
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8,
	  };

	/* negativ number can't fit in unsigned location */
	if (unsigned_target)
	  return (-1);

	/* signed negativ number size */
	for (i = AASM_MAX_VAL_SIZE - 1;
	     (i > 0) && (num->val[i] == 0xff);
	     i--)
	  ;
	size = (i + 1) * 8;
	size -= scan_set[num->val[i]];
	return (size + 1);
      }
  }
  {
    static const unsigned int	scan_clear[0x100] =
      {
	8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      };

    /* signed/unsigned positiv number */
    for (i = AASM_MAX_VAL_SIZE - 1;
	 (i > 0) && (num->val[i] == 0);
	 i--)
      ;
    size = (i + 1) * 8;
    size -= scan_clear[num->val[i]];
    if (!unsigned_target)
      size += 1;
    return (size);
  }
}

unsigned int
num_get_byte_size		(const struct num_value_s	*num)
{
  int			i;

  if (num->flag & NUM_SIGNED)
    {
      if (num->val[AASM_MAX_VAL_SIZE - 1] & 0x80)
	{
	  /* signed negativ number */
	  for (i = AASM_MAX_VAL_SIZE - 1; i >= 0; i--)
	    if (num->val[i] != 0xff)
	      break;
	  if ((i >= 0) && (num->val[i] & 0x80))
	    i--;
	  return (i + 2);
	}
      else
	{
	  /* signed positiv number */
	  for (i = AASM_MAX_VAL_SIZE - 1; i > 0; i--)
	    if (num->val[i])
	      break;
	  if (num->val[i] & 0x80)
	    i++;
	}
    }
  else
    {
      /* unsigned number */
      for (i = AASM_MAX_VAL_SIZE - 1; i > 0; i--)
	if (num->val[i])
	  break;
    }

  return (i + 1);
}

int
num_write		(const struct num_value_s	*num,
			 unsigned int			byte_size,
			 aasm_u8_t			*dest,
			 const unsigned int		*permut,
			 const struct error_locat_s	*location)
{
  unsigned int		num_size = num_get_byte_size(num);
  unsigned int		i;

  if (num_size > byte_size)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR, "%s value do not fit in %s destination", location);
      error_submsg(err, num->flag & NUM_SIGNED ? "signed" : "unsigned");
      error_submsg(err, size_name[byte_size]);

      return (-1);
    }

  for (i = 0; i < byte_size; i++)
    dest[permut[i]] = num->val[i];

  return (0);
}

