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
**    Copyright Cedric Bail, 2002
**    contact : cedric.bail@epita.fr
*/

#ifndef		REFCOUNT_H_
# define	REFCOUNT_H_

# include "memory.h"

struct  refcount_s { struct mem_s mem; };

void	refcount_init (struct refcount_s	*memory,
		       unsigned int		size,
		       const struct mem_desc_s	*desc);

void	refcount_clean (struct refcount_s	*memory);

void	*refcount_pop (struct refcount_s	*memory);

void	refcount_push (void			*ptr,
		       struct refcount_s	*memory);

void	*refcount_inc (void			*ptr);

#endif
