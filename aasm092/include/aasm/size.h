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

#ifndef SIZE_H_
# define SIZE_H_

# define SIZE_FLAG_BYTE		0x1 /* 1 byte */
# define SIZE_FLAG_WORD		0x2 /* 2 bytes */
# define SIZE_FLAG_DWORD	0x4 /* 4 bytes */
# define SIZE_FLAG_FWORD	0x8 /* 6 bytes */
# define SIZE_FLAG_QWORD	0x10 /* 8 bytes */
# define SIZE_FLAG_TWORD	0x20 /* 10 bytes */
# define SIZE_FLAG_OWORD	0x40 /* 16 bytes */

# define SIZE_MASK		0x7f
# define SIZE_UNDEF		SIZE_MASK

# define AASM_MAX_VAL_SIZE	16 /* maximum number/value size in byte handled by AASM */


# define SIZE_COMBINE_MAX(a, b)	(((a) & (b)) ? ((a) & (b)) : ((a) > (b) ? (a) : (b)))
/* 
  \ a            SIZE_A          SIZE_B          SIZE_UNDEF
  b\
SIZE_A           SIZE_A   max(SIZE_A, SIZE_B)    SIZE_A

SIZE_B    max(SIZE_A, SIZE_B)    SIZE_B          SIZE_B

SIZE_UNDEF       SIZE_A          SIZE_B          SIZE_UNDEF

*/

extern const char		*size_name[];

extern const unsigned int	size_flag2bytes[];

#endif

