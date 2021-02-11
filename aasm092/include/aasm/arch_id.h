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

#ifndef ARCH_ID_H_
# define ARCH_ID_H_

struct				arch_s
{
  unsigned short		id;
  unsigned short		sub_id;
};

# define ARCH_ID_NONE		0
# define ARCH_SID_NONE		0

/*** intel x86 ***/

# define ARCH_ID_X86		1

# define ARCH_SID_X86_8086	1
# define ARCH_SID_X86_286	2
# define ARCH_SID_X86_386	3
# define ARCH_SID_X86_486	4
# define ARCH_SID_X86_586	5
# define ARCH_SID_X86_686	6

/*** sparc ***/

# define ARCH_ID_SPARC		2

# define ARCH_SID_SPARC_V8	1
# define ARCH_SID_SPARC_V9	2

/*** Freedom CPU ***/

# define ARCH_ID_FCPU		3

/******/

#endif

