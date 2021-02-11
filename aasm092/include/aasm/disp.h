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

#ifndef DISP_H_
# define DISP_H_

#include <stdio.h>

#include "types.h"

#define DISP_FLAG_COLOR		0x1
#define DISP_FLAG_QUIET		0x2

int		disp_color_set(char		**dummy);

int		disp_quiet_set(char		**dummy);

void		disp_error(FILE			*out,
			   const char		*filename,
			   unsigned int		line,
			   const char		*msg);

void		disp_warning(FILE		*out,
			     const char		*filename,
			     unsigned int	line,
			     const char		*msg);

void		disp_action(FILE		*out,
			    const char		*text);

void		disp_info(FILE			*out,
			  const char		*title,
			  const char		*text);

void		disp_action_end(FILE		*out);

void		disp_action_failed(FILE		*out);

void		disp_hex(aasm_u8_t		*buf,
			 unsigned int		len,
			 FILE			*file);

#endif

