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

#ifndef RAWBIN_H_
# define RAWBIN_H_

#include <stdio.h>

out_write_t	out_intelhex_write;
out_directive_t	out_intelhex_directive;

# define INTELHEX_MAX_SIZE	0x10000

# define INTELHEX_LINESIZE	16

# define HEX_TYPE_DATA		0x00
# define HEX_TYPE_EOF		0x01
# define HEX_TYPE_SEG		0x02
# define HEX_TYPE_LINEAR	0x04

int		intelhex_write_sym(struct object_s	*obj,
				 FILE			*file);

int		intelhex_write(struct object_s		*obj,
			     FILE			*file);

void		intelhex_reloc(struct object_s		*obj);

#endif

