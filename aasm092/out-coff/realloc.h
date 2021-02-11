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
**    Copyright Alexandre Becoulet, 2003
**    contact : alexandre.becoulet@free.fr
*/

#ifndef REALLOC_H_
# define REALLOC_H_

# include <stdlib.h>
# include <string.h>

/*
** define a allocation pool growing with the realloc()
** function. should be used in variable block declaration.
**
** name:	pool variable name
** type:	alloc element type
** pre_alloc:	number of preallocated elements
*/

# define REALLOC_DEF(name, type, pre_alloc)		\
struct			name				\
{							\
  type			*pool;				\
  unsigned int		size;				\
  unsigned int		alloc_size;			\
  const unsigned int	pre_size;			\
}			name = {NULL, 0, 0, pre_alloc};

/*
** append data to the pool
**
** name:	pool variable name
** data:	pointer to data
** count:	element count
*/

# define REALLOC_APPEND(name, data, count)		\
{							\
  if (name.size + (count) > name.alloc_size)		\
    {							\
      name.pool = xrealloc(name.pool,			\
			  (name.size + (count)		\
			   + name.pre_size)		\
			  * sizeof (*name.pool));	\
      name.alloc_size = name.size + (count)		\
			+ name.pre_size;		\
    }							\
  memcpy(name.pool + name.size,				\
	 data, (count) * sizeof (*name.pool));		\
  name.size += (count);					\
}

/*
** get pointer to element pool
*/

# define REALLOC_POOL(name)	(name.pool)

/*
** get element count in pool
*/

# define REALLOC_COUNT(name)	(name.size)

/*
** free and empty pool
*/

# define REALLOC_FREE(name)				\
{							\
  free(name.pool);					\
  name.pool = NULL;					\
  name.size = name.alloc_size = 0;			\
}

#endif

