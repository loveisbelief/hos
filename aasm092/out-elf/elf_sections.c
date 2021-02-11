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
#include <stdio.h>

#include "aasm/types.h"
#include "aasm/memory.h"

#include "elf.h"

struct out_section_s	*
elf_section_new	(struct out_object_s	*elf_obj)
{
  struct out_section_s	*elf_section = mem_pop(&elf_obj->sec_pool);

  memset(elf_section, 0, sizeof (struct out_section_s));

  /* insert section to list */
  if (elf_obj->section_first)
    elf_obj->section_last->next = elf_section;
  else
    elf_obj->section_first = elf_section;

  elf_obj->section_last = elf_section;

  /* set section index */
  elf_section->index = elf_obj->section_count++;

  return (elf_section);
}

struct out_section_s	*
elf_section_add		(struct out_object_s	*elf_obj,
			 unsigned int		type,
			 unsigned int		flags)
{
  struct out_section_s	*elf_section = elf_section_new(elf_obj);

  /* set type and flags  */
  elf_obj->arch->write_Word(&elf_section->entry.sh_type, type);
  elf_obj->arch->write_Xword(&elf_section->entry.sh_flags, flags);

  return (elf_section);
}

aasm_u8_t		*
elf_section_alloc	(struct out_section_s	*elf_sec,
			 const aasm_u8_t	*data,
			 unsigned int		len)
{
  if (!elf_sec->data)
    elf_sec->data = xmalloc(elf_sec->alloc_size =
			      len + ELF32_REALLOC_SIZE);
  else
    {
      if (len > (elf_sec->alloc_size - elf_sec->size))
	elf_sec->data = xrealloc(elf_sec->data,
				   elf_sec->alloc_size =
				   elf_sec->size
				   + len + ELF32_REALLOC_SIZE);
    }

  if (data)
    memcpy(elf_sec->data + elf_sec->size, data, len);

  elf_sec->size += len;

  return (elf_sec->data);
}

void
elf_section_name	(struct out_object_s	*elf_obj,
			 struct out_section_s	*elf_sec,
			 const char		*name)
{
  /* set string name index */
  elf_sec->name = elf_obj->section_str->size;

  /* add name string to .shstrtab section data */
  elf_section_alloc(elf_obj->section_str,
		     (aasm_u8_t*)name, strlen(name) + 1);
}

unsigned int
elf_section_str(struct out_section_s		*elf_sec,
		  const char			*str)
{
  unsigned int	index = elf_sec->size;

  /* add string to section data */
  elf_section_alloc(elf_sec, (aasm_u8_t*)str, strlen(str) + 1);
  
  return (index);
}

void
elf_prepare_section(struct out_object_s	*elf_obj,
		      struct out_section_s	*elf_sec)
{
  elf_obj->arch->write_Word(&elf_sec->entry.sh_name, elf_sec->name);
  elf_obj->arch->write_Xword(&elf_sec->entry.sh_size, elf_sec->size);
  elf_obj->arch->write_Off(&elf_sec->entry.sh_offset, elf_sec->offset);
}

