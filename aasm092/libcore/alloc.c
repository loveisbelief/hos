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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aasm/alloc.h"

void		*xmalloc(size_t		size)
{
  void		*res;

  res = malloc(size);
  if (!res)
    {
      fprintf (stderr, ALLOC_ERR_MSG);
      exit (1);
    }
  return (res);
}

void		*xzalloc(size_t		size)
{
  void		*res;

  res = malloc(size);
  if (!res)
    {
      fprintf (stderr, ALLOC_ERR_MSG);
      exit (1);
    }
  memset(res, 0, size);
  return (res);
}

void		*xrealloc(void		*ptr,
			  size_t	size)
{
  void		*res;

  res = realloc(ptr, size);
  if (!res)
    {
      fprintf (stderr, ALLOC_ERR_MSG);
      exit (1);
    }
  return (res);
}

#undef free
#undef malloc

void		freedent(const void* ptr);
void		*mallodent(size_t size);


void		freedent(const void* ptr)
{
  free((void*)ptr);
}

void		*mallodent(size_t size)
{
  return malloc(size);
}
