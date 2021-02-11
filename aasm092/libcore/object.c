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

#include "aasm/object.h"
#include "aasm/error.h"
#include "aasm/alloc.h"
#include "aasm/section.h"
#include "aasm/macro.h"
#include "aasm/symbol.h"

void			object_init(struct object_s	*obj)
{
  memset(obj, 0, sizeof (struct object_s));

  scope_init(obj->cur_scope = &obj->global,
	     &meta_scope_g,
	     GLOBAL_SYM_HASHSIZE);

  obj->msc.parent = &mod_gl_scope;
}

int			object_set_name(struct object_s		*obj,
					const char		*str)
{
  if (obj->name)
    {
      error(ERRTYP_ERROR, "object name already defined", 0);
      return (-1);
    }

  obj->name = xstrdup(str);

  return (0);
}

int			object_set_desc(struct object_s		*obj,
					const char		*str)
{
  if (obj->description)
    {
      error(ERRTYP_ERROR, "object description already defined", 0);
      return (-1);
    }

  obj->description = xstrdup(str);

  return (0);
}

int			object_set_copyright(struct object_s	*obj,
					     const char		*str)
{
  if (obj->copyright)
    {
      error(ERRTYP_ERROR, "object copyright already defined", 0);
      return (-1);
    }

  obj->copyright = xstrdup(str);

  return (0);
}

int			object_set_base(struct object_s	*obj,
					unsigned int		base)
{
  if (obj->flag & OBJECT_BASEADDR)
    {
      error(ERRTYP_ERROR, "object base address already defined", 0);
      return (-1);
    }

  obj->flag |= OBJECT_BASEADDR;
  obj->address = base;

  return (0);
}

void			object_check(struct object_s	*obj,
				     const char		*filename)
{
  struct section_s	*sec;

  /* check for empty object */
  if (!obj->section_first)
    {
      error(ERRTYP_ERROR, "empty object (no section)", 0);
      return;
    }

  /* check for non ended section */
  if (obj->cur_section)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "section `%s' not ended at end of file",
			 &obj->cur_section->def_location), obj->cur_section->name);
      section_end_current(obj, get_location());
    }

  /* check for non ended macro */
  if (obj->cur_macro)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "macro `%s' not at ended of file",
			 &obj->cur_macro->sym->def_location),
		   obj->cur_macro->sym->name);

      macro_end_current(obj, get_location());
    }

  /* check all object sections */
  for (sec = obj->section_first; sec; sec = sec->next)
    section_check(sec);

  /* check global scope */
  scope_syms_check(&obj->global);

  /* set object source filename */
  obj->srcfilename = xstrdup(filename);

  /* set default object name */
  if (!obj->name)
    {
      char		*end;
      char		tmp[512];
      
      strncpy(tmp, filename, 512);
      if ((end = strrchr(tmp, '.')) && (end > tmp) && (end[-1] != '/'))
	*end = 0;
      
      obj->name = xstrdup(tmp);
    }

  /* set default object description */
  if (!obj->description)
    obj->description = xstrdup("none");

  /* set default object copyright */
  if (!obj->copyright)
    obj->copyright = xstrdup("none");
}

void			object_assemble(struct object_s		*obj)
{
  struct section_s	*sec;

  /* process all sections */
  for (sec = obj->section_first; sec; sec = sec->next)
    section_assemble(sec, obj);

  obj->arch.id = obj->msc.cl.nm.asm_->info->arch_id;
}

int			object_reloc(struct object_s	*obj)
{
  struct section_s	*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      section_address_reduce(sec);
      sec->flag |= SECTION_BINARY;
    }
  /*
  for (sec = obj->section_first; sec; sec = sec->next)
    reloc_reduce(sec);
  */
  return (0);
}

void			object_free(struct object_s	*obj)
{
  struct section_s	*sec, *tmp;

  sec = obj->section_first;
  while (sec)
    {
      tmp = sec->next;
      section_free(sec);
      sec = tmp;
    }

  scope_free(&obj->global);
  
  if (obj->srcfilename)
    dup_free ((void*) obj->srcfilename);
  if (obj->name)
    dup_free ((void*) obj->name);
  if (obj->copyright)
    dup_free ((void*) obj->copyright);
  if (obj->description)
    dup_free ((void*) obj->description);
}
