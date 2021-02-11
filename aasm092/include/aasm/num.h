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

#ifndef NUM_H_
# define NUM_H_

#include <stdio.h>

#include "types.h"
#include "size.h"

struct				error_locat_s;

struct				num_value_s
{
  unsigned int			flag;
  aasm_u8_t			val[AASM_MAX_VAL_SIZE];
};

# define NUM_SIGNED		0x80 /* signed value */

int			num_and(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_or(const struct num_value_s		*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_xor(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_shl(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_shr(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_not(struct num_value_s		*a);

void			num_init(struct num_value_s		*num);

void			num_init_uint(struct num_value_s	*num,
				      unsigned int		x);

void			num_init_int(struct num_value_s		*num,
				     int			x);

int			num_cmp(const struct num_value_s	*a,
				const struct num_value_s	*b);

int			num_cmp_uint(const struct num_value_s	*a,
				     unsigned int		b);

int			num_add(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_sub(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_mul(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_div(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_mod(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_pow(const struct num_value_s	*a,
				const struct num_value_s	*b,
				struct num_value_s		*res);

int			num_neg(struct num_value_s		*num);

int			num_add_uint(struct num_value_s		*num,
				     unsigned int		x);

int			num_mul_uint(struct num_value_s		*num,
				     unsigned int		x);

#ifdef DEBUG
void			num_disp(const struct num_value_s	*num);
#endif

void			num_fout(const struct num_value_s	*num,
				 FILE				*out);

int			num_get_uint(const struct num_value_s	*num,
				     unsigned int		*val);

int			num_get_int(const struct num_value_s	*num,
				    int				*val);

unsigned int
num_is_mask		(const struct num_value_s	*num);

int
num_get_bit_size	(const struct num_value_s	*num,
			 unsigned int			unsigned_target);

unsigned int
num_get_byte_size	(const struct num_value_s	*num);

int			num_write(const struct num_value_s	*num,
				  unsigned int			byte_size,
				  aasm_u8_t			*dest,
				  const unsigned int		*permut,
				  const struct error_locat_s	*location);

#endif

