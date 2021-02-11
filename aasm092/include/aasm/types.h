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

#ifndef TYPES_H_
# define TYPES_H_

typedef unsigned char				aasm_u8_t;
typedef unsigned short int			aasm_u16_t;
typedef unsigned int				aasm_u32_t;

typedef signed char				aasm_s8_t;
typedef signed short int			aasm_s16_t;
typedef signed int				aasm_s32_t;

#define HAVELONGLONG

# ifdef HAVELONGLONG
#  ifdef __GNUC__
__extension__
#  endif
typedef signed long long int			aasm_s64_t;
#  ifdef __GNUC__
__extension__
#  endif
typedef unsigned long long int			aasm_u64_t;
# else
typedef aasm_u8_t				aasm_u64_t[8];
typedef aasm_s8_t				aasm_s64_t[8];
# endif

#endif

