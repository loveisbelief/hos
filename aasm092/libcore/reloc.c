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

#include <stdlib.h>
#include <string.h>

#include "aasm/memory.h"
#include "aasm/reloc.h"
#include "aasm/expr.h"
#include "aasm/section.h"

/* Memory management for relocation struct */

static struct mem_s reloc_pool_g;

static const struct mem_desc_s	reloc_mem_desc =
  {
    "relocation",
    NULL
  };

void
reloc_init (void)
{
  mem_init (&reloc_pool_g, sizeof (struct reloc_s), &reloc_mem_desc);
}

void
reloc_clean (void)
{
  mem_clean (&reloc_pool_g);
}

struct reloc_s		*reloc_add(struct section_s	*sec)
{
  struct reloc_s	*reloc = mem_pop (&reloc_pool_g);

  memset(reloc, 0, sizeof(struct reloc_s));
  reloc->expr_ = NULL;
  reloc->next = sec->reloc;
  sec->reloc = reloc;

  return (reloc);
}

void			reloc_free (struct reloc_s *rel)
{
  if (rel->expr_)
    expr_free (rel->expr_);
  mem_push (rel, &reloc_pool_g);
}

void			relocs_free(struct section_s	*sec)
{
  struct reloc_s	*rel = sec->reloc;
  struct reloc_s	*next;

  while (rel)
    {
      next = rel->next;
      if (rel->expr_)
	expr_free (rel->expr_);
      mem_push (rel, &reloc_pool_g);
      rel = next;
    }
  sec->reloc = NULL;
}
