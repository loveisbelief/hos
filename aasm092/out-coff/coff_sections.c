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
#include <stdlib.h>

#include "aasm/types.h"
#include "aasm/memory.h"
#include "aasm/object.h"
#include "aasm/alloc.h"
#include "aasm/section.h"
#include "aasm/error.h"

#include "coff.h"

int
coff_write_sechdr	(struct object_s	*obj,
			 FILE			*file)
{
  struct out_object_s	*out_obj = obj->out_obj; 

  {
    struct out_section_s	*out_sec;
    aasm_u32_t			sec_addr = 0;

    /* respect out_section order */
    for (out_sec = out_obj->section_first; out_sec; out_sec = out_sec->next)
      {
	struct section_s	*sec = out_sec->sec;
	unsigned int		flags = 0;
	struct coff_sechdr_s	entry;

	/* set section name */

	memset(&entry, 0, sizeof (struct coff_sechdr_s));

	if (strlen(sec->name) > COFF_SECNAME_SIZE)
	  error_submsg(error(ERRTYP_WARN, "coff: `%s' section name truncated",
			     &sec->def_location), sec->name);
	strncpy(entry.name, sec->name, COFF_SECNAME_SIZE);

	/* set section size */

	out_obj->arch->write_32(&entry.size, sec->size);

	/* set section address */

	if (sec->align > 1)	/* align address if needed */
	  sec_addr = (((sec_addr - 1) / sec->align) + 1) * sec->align;

	/* entry.vt_addr = 0; */
	if (out_obj->flag & COFF_VARIANT_GNU) /* virtual size = 0 for ms coff */
	  out_obj->arch->write_32(&entry.ph_addr, sec_addr);

	sec_addr += sec->size;

	/* set section other info */

	out_obj->arch->write_32(&entry.data_pos, out_sec->data_pos);

	out_obj->arch->write_32(&entry.reloc_pos, out_sec->reloc_pos);
	out_obj->arch->write_16(&entry.reloc_cnt, out_sec->reloc_count);

	/* line_pos */
	entry.line_cnt = entry.line_pos = 0;

	/* set section flags */
	switch (sec->flag & SECTION_TYPE_MASK)
	  {
	  case (SECTION_TYPE_CODE):
	  case (SECTION_TYPE_CODE | SECTION_TYPE_READONLY):
	    flags = COFF_SECFLG_TYPECODE | COFF_SECFLG_EXEC;
	    break;

	  case (SECTION_TYPE_DATA | SECTION_TYPE_READONLY):
	    flags = COFF_SECFLG_TYPEDATA | COFF_SECFLG_READ;
	    break;

	  case (SECTION_TYPE_DATA):
	    flags = COFF_SECFLG_TYPEDATA | COFF_SECFLG_READ | COFF_SECFLG_WRITE;
	    break;

	  case (SECTION_TYPE_BSS):
	  case (SECTION_TYPE_BSS | SECTION_TYPE_READONLY):
	    flags = COFF_SECFLG_TYPEBSS | COFF_SECFLG_READ | COFF_SECFLG_WRITE;
	    break;
	  }

	out_obj->arch->write_32(&entry.flags, flags);

	if (fwrite(&entry, SIZEOF_COFF_SECHDR_S, 1, file) != 1)
	  return (RETVAL_ERR_IO);
      }
  }

  return (0);
}

static struct out_section_s	*
coff_section_new	(struct out_object_s	*out_obj)
{
  struct out_section_s	*out_section = mem_pop(&out_obj->sec_pool);

  memset(out_section, 0, sizeof (struct out_section_s));

  /* insert section to list */
  if (out_obj->section_first)
    out_obj->section_last->next = out_section;
  else
    out_obj->section_first = out_section;

  out_obj->section_last = out_section;

  out_section->index = ++out_obj->section_count;

  return (out_section);
}

struct out_section_s	*
out_coff_newsection	(struct object_s		*obj,
			 struct section_s		*sec)
{
  /* creat coff section */
  return (coff_section_new(obj->out_obj));
}

int
coff_write_section	(struct object_s		*obj,
			 FILE				*file)
{
  struct section_s	*sec;
  aasm_u8_t		*data = NULL;
  unsigned int		alloc_size = 0;
  int			retval = 0;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      if (sec->flag & (SECTION_TYPE_CODE | SECTION_TYPE_DATA))
	{
	  struct out_section_s	*out_sec = sec->out_sec;

	  if (sec->size)
	    {
	      alloc_size = alloc_size > sec->size
		? alloc_size : sec->size;

	      data = xrealloc(data, alloc_size);

	      memset(data, 0, sec->size);
	      section_write(sec, data);
	      out_sec->data_pos = ftell(file);

	      if (fwrite(data, sec->size, 1, file) != 1)
		{
		  retval = RETVAL_ERR_IO;
		  break;
		}
	    }
	}
    }

  if (data)
    free(data);

  return (retval);
}

