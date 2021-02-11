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
#include <unistd.h>
#include <time.h>

#include "aasm/disp.h"
#include "aasm/error.h"
#include "aasm/object.h"
#include "aasm/section.h"
#include "aasm/reloc.h"
#include "aasm/info.h"
#include "aasm/memory.h"
#include "aasm/alloc.h"

#include "coff.h"

static const struct mem_desc_s	symbol_mem_desc =
  {
    "coff_symbol",
    NULL
  };

static const struct mem_desc_s	section_mem_desc =
  {
    "coff_section",
    NULL
  };

struct out_object_s	*
out_coff_newobject	(struct object_s	*obj)
{
  struct out_object_s	*out_obj;

  /* creat out object on first call */
  out_obj = xzalloc(sizeof (struct out_object_s));

  /* init out_section & out_symbol alloc pools */
  mem_init(&out_obj->sec_pool, sizeof (struct out_section_s), &section_mem_desc);
  mem_init(&out_obj->sym_pool, sizeof (struct out_symbol_s), &symbol_mem_desc);

  return (out_obj);
}

void
out_coff_clean		(struct object_s	*obj)
{
  mem_clean(&obj->out_obj->sec_pool);
  mem_clean(&obj->out_obj->sym_pool);

  free(obj->out_obj);
}

static int
coff_init_header	(struct object_s	*obj,
			 FILE			*file)
{
  struct out_object_s	*out_obj = obj->out_obj;
  struct coff_header_s	hdr;
  aasm_u32_t		flags = COFF_HDRFLG_NO_LINE;

  memset(&hdr, 0, sizeof (struct coff_header_s));

  out_obj->arch->write_16(&hdr.magic, out_obj->arch->magic);
  out_obj->arch->write_16(&hdr.sec_cnt, out_obj->section_count);

  /* Remove time from header. This is usefull to
     keep object file checksum constant, for testing
     purpose */
  if (!(out_obj->flag & COFF_NOTIME)) 
    out_obj->arch->write_32(&hdr.time_stamp, (aasm_u32_t)time(NULL));

  out_obj->arch->write_32(&hdr.symtab_pos, out_obj->symtab_pos);
  out_obj->arch->write_32(&hdr.sym_cnt, out_obj->sym_count);
  hdr.opthdr_size = 0;

  if (out_obj->reloc_count == 0)
    flags |= COFF_HDRFLG_NO_RELOC;

  if (out_obj->sym_unresolved == 0)
    flags |= COFF_HDRFLG_RESOLVED;

  if (out_obj->sym_local == 0)
    flags |= COFF_HDRFLG_NO_LOCAL;

  out_obj->arch->write_16(&hdr.flags, flags);

  if (fwrite(&hdr, SIZEOF_COFF_HEADER_S, 1, file) != 1)
    return (RETVAL_ERR_IO);

  return (0);
}

static int
coff_write		(struct object_s	*obj,
			 FILE			*file,
			 const char		*filename)
{
  struct out_object_s	*out_obj = obj->out_obj;

  /* get architecture specific coff information */
  if (!(obj->out_obj->arch = coff_get_arch(&obj->arch)))
    {
      error(ERRTYP_ERROR, "coff: unknown target architecture", 0);
      return (-1);
    }

  if (!(out_obj->flag & COFF_VARIANT_MASK))
    {
      error(ERRTYP_WARN, "coff: coff variant not specified, assuming `gnu' variant", 0);
      out_obj->flag |= COFF_VARIANT_GNU;
    }

  if (obj->flag & OBJECT_BASEADDR)
    error(ERRTYP_WARN, "coff: object base address ignored", NULL);

  /* update section back links */
  {
    struct section_s	*sec;

    for (sec = obj->section_first; sec; sec = sec->next)
      sec->out_sec->sec = sec;
  }

  /* skip file header and section table */
  if (fseek(file, (signed)(SIZEOF_COFF_HEADER_S
			   + (SIZEOF_COFF_SECHDR_S
			      * out_obj->section_count)),
	    SEEK_SET))
    return (-1);

  /* write symbol table */
  switch (coff_write_symtable(obj, file))
    {
    case (RETVAL_ERR_IO):
      goto coff_write_io_err;
    case (RETVAL_ERR_MISC):
      return (-1);
    }

  /* write relocation tables */
  switch (coff_write_relocs(obj, file))
    {
    case (RETVAL_ERR_IO):
      goto coff_write_io_err;
    case (RETVAL_ERR_MISC):
      return (-1);
    }

  /* write object sections data */
  if (coff_write_section(obj, file))
    goto coff_write_io_err;

  /* write file header */
  if (fseek(file, 0, SEEK_SET)
      || coff_init_header(obj, file))
    goto coff_write_io_err;

  /* write section table */
  if (coff_write_sechdr(obj, file))
    goto coff_write_io_err;

  return (0);

 coff_write_io_err:
  error_submsg(error(ERRTYP_ERROR,
		     "coff: I/O output error, can't write `%s'",
		     NULL), filename);
  return (-1);
}

int
out_coff_write		(struct object_s	*obj)
{
  char		filename[512];
  FILE		*file;

  /* set filename */
  if (obj->out_obj->flag & COFF_VARIANT_MS)
    sprintf(filename, "%s.obj", obj->name);
  else
    sprintf(filename, "%s.o", obj->name);

  disp_info(stdout, "writing", filename);

  if ((file = fopen(filename, "w+b")))
    {
      if (!coff_write(obj, file, filename))
	{
	  fclose(file);
	  return (0);
	}
      fclose(file);
      unlink(filename);
    }

  return (-1);
}

