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
#include <string.h>

#include "aasm/error.h"
#include "aasm/refcount.h"

static struct refcount_s location_pool_g;

#ifdef DEBUG
static void
location_disp	(void		*data)
{
  struct error_locat_s	*err = data;

  fprintf(stderr, "file=%s, line=%u",
	  err->filename, err->pos.line);
}
#endif

static const struct mem_desc_s	location_mem_desc =
  {
    "location",
#ifdef DEBUG
    &location_disp,
#else
    NULL,
#endif
  };

void			location_init (void)
{
  refcount_init (&location_pool_g, sizeof (struct error_locat_s), &location_mem_desc);
}

void			location_clean (void)
{
  refcount_clean (&location_pool_g);
}

void			free_location (struct error_locat_s* location)
{
  refcount_push (location, &location_pool_g);
}


struct error_locat_s	*dup_location (const struct error_locat_s	*location)
{
  struct error_locat_s	*new = refcount_pop (&location_pool_g);

  memcpy(new, location, sizeof (struct error_locat_s));

  return (new);
}

struct error_locat_s	*location_newref (struct error_locat_s	*location)
{
  return refcount_inc (location);
}
