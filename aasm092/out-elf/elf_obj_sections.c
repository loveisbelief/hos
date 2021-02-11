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

#include "aasm/error.h"
#include "aasm/section.h"
#include "aasm/object.h"
#include "aasm/alloc.h"

#include "elf.h"

struct out_section_s	*
out_elf_newsection	(struct object_s		*obj,
			 struct section_s		*sec)
{
  /* creat elf section */
  return (elf_section_new(obj->out_obj));
}

void
elf_init_obj_sections	(struct object_s		*obj,
			 struct out_object_s		*elf_obj)
{
  struct section_s		*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {

      elf_obj->arch->write_Word(&sec->out_sec->entry.sh_type, (unsigned)
				((sec->flag & SECTION_TYPE_BSS)
				 ? SHT_NOBITS : SHT_PROGBITS));

      elf_obj->arch->write_Xword(&sec->out_sec->entry.sh_flags, (unsigned)
				(SHF_ALLOC
				 | (sec->flag & SECTION_TYPE_READONLY ? 0 : SHF_WRITE)
				 | (sec->flag & SECTION_TYPE_CODE ? SHF_EXECINSTR : 0)));

      elf_section_name(elf_obj, sec->out_sec, sec->name);

      /* check section base adress */
      if (sec->flag & SECTION_BASEADDR)
	error_submsg(error(ERRTYP_WARN,
			   "elf: `%s' section base address ignored",
			   &sec->def_location), sec->name);

      if (sec->flag & SECTION_ALIGN)
	{
	  if (sec->align && (sec->align & (sec->align - 1)))
	    error_submsg(error(ERRTYP_ERROR, "elf: alignment for `%s' section have to be a power of 2",
			       &sec->def_location), sec->name);

	  elf_obj->arch->write_Word(&sec->out_sec->entry.sh_addralign, sec->align);
	}
      else
	error_submsg(error(ERRTYP_WARN_OPT, "elf: no alignment defined for `%s' section",
			   &sec->def_location), sec->name);
    }
}

void	elf_write_obj_section(struct object_s		*obj)
{
  struct section_s		*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      if (sec->flag & (SECTION_TYPE_CODE | SECTION_TYPE_DATA))
	{
	  aasm_u8_t		*data = xzalloc(sec->size);

	  sec->out_sec->data = data;
	  section_write(sec, data);
	}
      sec->out_sec->size = sec->size;
    }
}

