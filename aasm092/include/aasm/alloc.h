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


#ifndef ALLOC_H_
# define ALLOC_H_

#include <stdlib.h>

# define ALLOC_ERR_MSG	"Memory allocation error, aborting.\n"

/* Macro for general alignement calcul */
#define ALIGN(Ptr) \
  {\
    Ptr--;\
    Ptr |= sizeof (void*) - 1;\
    Ptr++;\
  }

void			*xmalloc(size_t		size);
void			*xzalloc(size_t		size);
void			*xrealloc(void		*ptr,
				  size_t	size);

char			*xstrdup (const char	*str);
void			dup_free (char*		str);
void			string_clean (void);
void			string_init (void);

#endif /* !ALLOC_H_ */

