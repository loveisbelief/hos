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

#include "aasm/memory.h"

static struct resize_mem_s string_pool_g;

void			string_clean (void)
{
  mem_resize_clean (&string_pool_g);
}
void			string_init (void)
{
  mem_resize_init (&string_pool_g, 0);
}

char		*xstrdup(const char	*str)
{
  char		*res;
  size_t	len;

  if (!str)
    return (NULL);

  len = strlen(str);
  res = mem_resize_pop (&string_pool_g, len + 1);
  memcpy(res, str, len + 1);
  return (res);
}

void		dup_free (char* str)
{
  mem_resize_push (str, &string_pool_g);
}
