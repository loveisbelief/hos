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
#include <stdlib.h>

#include "aasm/object.h"
#include "aasm/section.h"
#include "aasm/macro.h"
#include "aasm/proc.h"
#include "aasm/symbol.h"
#include "aasm/modules.h"
#include "aasm/memory.h"
#include "aasm/reloc.h"

/*
** section
*/

struct section_s	*
section_find		(struct object_s		*obj,
			 const char			*name)
{
  struct section_s	*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    if (!strcmp(name, sec->name))
      break;

  if (!sec)
    sec = section_new(obj, name);

  return (sec);
}

int
section_define		(struct object_s		*obj,
			 struct section_s		*section,
			 unsigned int			type,
			 const struct error_locat_s	*location)
{
  if (section->flag & SECTION_DEFINED)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR, "section name `%s' already used at %s", location);
      error_submsg(err, section->name);
      error_sublocation(err, &section->def_location);

      return (-1);
    }

  section->flag = type | SECTION_DEFINED;

  memcpy(&section->def_location, get_location(), sizeof (struct error_locat_s));

  obj->cur_section = section;
  obj->cur_scope = &section->scope;
  section->scope.section = section;

  return (0);
}

void
section_end_current	(struct object_s		*obj,
			 const struct error_locat_s	*location)
{
  if (obj->cur_proc)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR,
		  "function `%s' not ended at end of `%s' section",
		  &obj->cur_proc->sym->def_location);

      error_submsg(err, obj->cur_proc->sym->name);
      error_submsg(err, obj->cur_section->name);

      proc_end_current(obj, location);
    }

  if (obj->cur_macro)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR,
		  "macro `%s' not ended at end of `%s' section",
		  &obj->cur_macro->sym->def_location);

      error_submsg(err, obj->cur_macro->sym->name);
      error_submsg(err, obj->cur_section->name);

      macro_end_current(obj, location);
    }

  scope_end(obj->cur_scope, location);

  obj->cur_scope = obj->cur_scope->parent;
  obj->cur_section = NULL;
}

int
section_set_base	(struct section_s		*sec,
			 unsigned int			base)
{

  if (sec->flag & SECTION_BASEADDR)
    {
      error(ERRTYP_ERROR, "section base address already defined", 0);
      return (-1);
    }

  sec->flag |= SECTION_BASEADDR;
  sec->address = base;

  return (0);
}

int
section_set_align	(struct section_s		*sec,
			 unsigned int			align)
{

  if (sec->flag & SECTION_ALIGN)
    {
      error(ERRTYP_ERROR, "section address alignment already defined", 0);
      return (-1);
    }

  sec->flag |= SECTION_ALIGN;
  sec->align = align;

  return (0);
}

void
section_check		(struct section_s		*sec)
{
  struct proc_s		*proc;

  if (!(sec->flag & SECTION_DEFINED))
    {
      error_submsg(error(ERRTYP_ERROR,
			 "use of undefined section `%s' (reported only once)",
			 &sec->use_location), sec->name);
      return;
    }

  for (proc = sec->proc_first; proc; proc = proc->next)
    {
      scope_syms_check(&proc->scope);
      scope_instrs_check(&proc->scope, sec);
    }

  scope_syms_check(&sec->scope);
  scope_instrs_check(&sec->scope, sec);
}

void
section_assemble	(struct section_s		*sec,
			 struct object_s		*obj)
{
  struct proc_s		*proc;

  scope_assemble(&sec->scope, sec, obj);

  for (proc = sec->proc_first; proc; proc = proc->next)
    scope_assemble(&proc->scope, sec, obj);
}

void
section_address_reduce	(struct section_s		*sec)
{
  sec->size = scope_address(&sec->scope, sec, 0);
}

void
section_write		(struct section_s		*sec,
			 aasm_u8_t			*buf)
{
  scope_write(&sec->scope, buf);
}

/*
** Memory management for section
*/

static struct mem_s	section_pool_g;

static const struct mem_desc_s	section_mem_desc =
  {
    "section",
    NULL
  };

void			section_init (void)
{
  mem_init (&section_pool_g, sizeof (struct section_s), &section_mem_desc);
}

void			section_clean (void)
{
  mem_clean (&section_pool_g);
}

/*
** section
*/

struct section_s	*section_new(struct object_s	*obj,
				     const char		*name)
{
  struct section_s	*section;

  section = mem_pop (&section_pool_g);
  memset (section, 0, sizeof (struct section_s));

  scope_init(&section->scope,
	     &obj->global,
	     SECTION_SYM_HASHSIZE);

  if (obj->section_first)
    obj->section_last->next = section;
  else
    obj->section_first = section;

  obj->section_last = section;
  obj->section_count++;

  section->name = xstrdup(name);

  memcpy(&section->use_location, get_location(),
  sizeof (struct error_locat_s));

  return (section);
}

void			section_free(struct section_s	*sec)
{
  scope_free (&sec->scope);
  procs_free (sec);
  relocs_free (sec);

  dup_free ((void*) sec->name);
  mem_push (sec, &section_pool_g);
}

