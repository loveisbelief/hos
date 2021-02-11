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
#include <stdio.h>

#include "aasm/types.h"
#include "aasm/error.h"
#include "aasm/object.h"
#include "aasm/alloc.h"
#include "aasm/section.h"

#include "binary.h"

static void		binary_set_base(struct object_s	*obj)
{
  unsigned int		flag;
  struct section_s	*sec;
  struct section_s	*min = NULL;
  struct section_s	*max = NULL;

  /* find higher and lower sections */
  for (sec = obj->section_first; sec; sec = sec->next)
    {
      if (!sec->size)
	{
	  error_submsg(error(ERRTYP_WARN,
			     "binary: zero sized `%s' section ignored",
			     &sec->def_location), sec->name);
	  continue;
	}

      if (sec->flag & SECTION_BASEADDR)
	{
	  if (!min || (min->address > sec->address))
	    min = sec;

	  if (!max || ((max->address + max->size)
		       < (sec->address + sec->size)))
	    max = sec;
	}
    }

  /* check/set object base address */
  if (obj->flag & OBJECT_BASEADDR)
    {
      if (min && (obj->address > min->address))
	{
	  error(ERRTYP_ERROR, "binary: object base address greater than lowest section base address", NULL);
	  return;
	}
    }
  else
    {
      obj->flag |= OBJECT_BASEADDR;
      if (min)
	{
	  obj->address = min->address;
	  error_submsg(error(ERRTYP_WARN,
			     "binary: object base address set to `%s' section base address",
			     NULL), min->name);
	}
      else
	{
	  obj->address = 0;
	  error(ERRTYP_WARN, "binary: object base address set to zero", NULL);
	}
    }

  /* set sections base address */
  for (sec = obj->section_first; sec; sec = sec->next)
    {
      if (!(sec->flag & SECTION_BASEADDR))
	{
	  error_submsg(error(ERRTYP_WARN,
			     "binary: no base address defined for `%s' section",
			     &sec->def_location), sec->name);

	  if (!max)
	    sec->address = obj->address;
	  else
	    sec->address = max->address + max->size;
	  max = sec;

	  sec->flag |= SECTION_BASEADDR;
	}
    }

  /* sections sort by address */
  do
    {
      struct section_s	*tmp;
      struct section_s	**prev;

      flag = 0;
      prev = &obj->section_first;

      for (sec = obj->section_first; sec && sec->next; sec = sec->next)
	{
	  if (sec->address > sec->next->address)
	    {
	      flag++;
              *prev = sec->next;
              prev = &sec->next->next;
              tmp = sec->next->next;
              sec->next->next = sec;
              sec->next = tmp;
	    }
	}
    }
  while (flag);

  /* check overlaping */
  for (sec = obj->section_first; sec && sec->next; sec = sec->next)
    if (sec->address + sec->size > sec->next->address)
      {
	struct error_s	*err;

	err = error(ERRTYP_ERROR, "binary: sections `%s' and `%s' overlap", NULL); 
	error_submsg(err, sec->name);
	error_submsg(err, sec->next->name);
      }
}

static void		binary_write_sec(const struct object_s	*obj,
					 FILE			*file)
{
  aasm_u8_t		*buf = NULL;
  unsigned int		alloc_size = 0;
  struct section_s	*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      if (sec->flag & SECTION_TYPE_BSS)
	continue;

      if (sec->size > alloc_size)
	{
	  if (buf)
	    free(buf);
	  buf = xzalloc(alloc_size = sec->size);
	}

      section_write(sec, buf);

      if (fseek(file, (signed)(sec->address - obj->address), SEEK_SET))
	goto binary_write_sec_err;
	  
      if (sec->size && (fwrite(buf, sec->size, 1, file) != 1))
	goto binary_write_sec_err;
    }

  free(buf);

  return;

 binary_write_sec_err:
  free(buf);
  error_submsg(error(ERRTYP_ERROR,
		     "output error, can't write `%s' section",
		     NULL), sec->name);
}

int			binary_write(struct object_s	*obj,
				     FILE		*file)
{
  binary_set_base(obj);

  if (error_count_g)
    return (-1);

  binary_reloc(obj);

  if (error_count_g)
    return (-1);

  binary_write_sec(obj, file);

  return (error_count_g);
}

