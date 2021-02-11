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

#include <string.h>

#include "aasm/error.h"
#include "aasm/proc.h"
#include "aasm/object.h"
#include "aasm/symbol.h"
#include "aasm/memory.h"
#include "aasm/section.h"
#include "aasm/scope.h"
#include "aasm/macro.h"


/*
** should be moved in parser
*/

void			proc_set_current(struct object_s		*obj,
					 struct proc_s			*proc,
					 const struct error_locat_s	*location)
{
  obj->cur_proc = proc;
  obj->cur_scope = &proc->scope;
  proc->scope.section = obj->cur_section;
}

void			proc_end_current(struct object_s		*obj,
					 const struct error_locat_s	*location)
{
  if (obj->cur_macro)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR,
		  "macro `%s' not ended at end of `%s' function",
		  &obj->cur_macro->sym->def_location);

      error_submsg(err, obj->cur_macro->sym->name);
      error_submsg(err, obj->cur_proc->sym->name);

      macro_end_current(obj, location);
    }

  scope_end(obj->cur_scope, location);

  obj->cur_scope = obj->cur_scope->parent;
  obj->cur_proc = NULL;
}

/* Memory management for proc */

static struct mem_s proc_pool_g;

static const struct mem_desc_s	proc_mem_desc =
  {
    "proc",
    NULL
  };

void
proc_init (void)
{
  mem_init (&proc_pool_g, sizeof (struct proc_s), &proc_mem_desc);
}

void
proc_clean (void)
{
  mem_clean (&proc_pool_g);
}

struct proc_s		*proc_new (struct section_s	*section,
				   struct symbol_s	*sym)
{
  struct proc_s		*proc = mem_pop (&proc_pool_g);
  memset (proc, 0, sizeof (struct proc_s));

  proc->sym = sym;
  scope_init(&proc->scope, &section->scope, PROC_SYM_HASHSIZE);

  if (section->proc_first)
    section->proc_last->next = proc;
  else
    section->proc_first = proc;

  section->proc_last = proc;

  return (proc);
}

void			procs_free (struct section_s	*sec)
{
  struct proc_s		*proc, *next;

  proc = sec->proc_first;
  while (proc)
    {
      next = proc->next;
      scope_free (&proc->scope);
      mem_push (proc, &proc_pool_g);
      proc = next;
    }
  sec->proc_first = NULL;
  sec->proc_last = NULL;
}
