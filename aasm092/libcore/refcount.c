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

#include <stdio.h>

#include "aasm/refcount.h"

static unsigned int	struct_size = 0;

struct internal_refcount_s
{
  int	reference;
  void*	ptr;
};

void	refcount_init (struct refcount_s	*memory,
		       unsigned int		size,
		       const struct mem_desc_s	*desc)
{
  if (!struct_size)
    {
      struct_size = sizeof (struct internal_refcount_s);
      ALIGN (struct_size);
    }

  mem_init (&(memory->mem), size + struct_size, desc);
}

void	refcount_clean (struct refcount_s	*memory)
{
  mem_clean (&(memory->mem));
}

void	*refcount_pop (struct refcount_s	*memory)
{
  struct internal_refcount_s*	nw = mem_pop (&(memory->mem));

  nw->reference = 1;
  nw->ptr = nw + 1;

  return (nw->ptr);
}

void	refcount_push (void			*ptr,
		       struct refcount_s	*memory)
{
  struct internal_refcount_s*	push = (struct internal_refcount_s*) ptr - 1;

  if (push->reference <= 0)
    {
#ifdef DEBUG
      fputs("too many push: ", stderr);
      mem_disp_data(ptr, &memory->mem);
#endif
      return;
    }

  --push->reference;

#ifndef DEBUG
  /* keep data allocated */
  mem_push (push, &(memory->mem));
#endif
}

void	*refcount_inc (void*			ptr)
{
  struct internal_refcount_s*	push = (struct internal_refcount_s*) ptr - 1;

  push->reference++;

  return ptr;
}

