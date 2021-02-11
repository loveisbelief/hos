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

#ifndef BINARY_H_
# define BINARY_H_

#include <stdio.h>

#include "aasm/mod_out_import.h"

out_write_t	out_binary_write;
out_directive_t	out_binary_directive;

int		binary_write_sym (const struct object_s	*obj,
				  FILE			*file);

int		binary_write (struct object_s	*obj,
			      FILE		*file);

void		binary_reloc (const struct object_s	*obj);

#endif

