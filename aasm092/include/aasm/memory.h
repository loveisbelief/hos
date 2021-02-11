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

#ifndef		MEMORY_H_
# define	MEMORY_H_

# include "alloc.h"
# include "memstruct.h"

# define MEM_REALLOC_SIZE	1024

#ifdef DEBUG
void		mem_disp_data (void* ptr, struct mem_s *memory);
#endif

void		mem_push (void*		ptr,
			  struct mem_s	*memory);

void		*mem_pop(struct mem_s	*memory);

void		mem_clean (struct mem_s	*memory);

void		mem_init (struct mem_s	*memory,
			  unsigned int	size,
			  const struct mem_desc_s	*desc);

void		mem_resize_push (void*			ptr,
				 struct resize_mem_s	*memory);

void		*mem_resize_pop (struct resize_mem_s	*memory,
				 unsigned int		size);

void		mem_resize_clean (struct resize_mem_s	*memory);

void		mem_resize_init (struct resize_mem_s	*memory,
				 unsigned int		size);

#endif

