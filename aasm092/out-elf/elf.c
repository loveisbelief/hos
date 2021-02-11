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

#include "config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "aasm/error.h"
#include "aasm/object.h"
#include "aasm/reloc.h"
#include "aasm/info.h"
#include "aasm/memory.h"
#include "aasm/alloc.h"

#include "elf.h"

static const struct mem_desc_s	symbol_mem_desc =
  {
    "elf_symbol",
    NULL
  };

static const struct mem_desc_s	section_mem_desc =
  {
    "elf_section",
    NULL
  };

static void
elf_init_header		(struct out_object_s	*elf_obj,
			 struct object_s	*obj,
			 struct arch_s		*arch)
{
  struct out_section_s	*comment;

  memset(&elf_obj->hdr, 0, sizeof (Elf_Ehdr));

  elf_obj->hdr.e_ident[EI_DATA] = elf_obj->arch->e_ident;
  elf_obj->arch->write_Half(&elf_obj->hdr.e_machine, elf_obj->arch->e_machine);

  /* set header informations */
  memcpy(elf_obj->hdr.e_ident, ELFMAG, SELFMAG);
#ifdef ELF_64BITS
  elf_obj->hdr.e_ident[EI_CLASS] = ELFCLASS64;
#else
  elf_obj->hdr.e_ident[EI_CLASS] = ELFCLASS32;
#endif
  elf_obj->hdr.e_ident[EI_VERSION] = EV_CURRENT;
  elf_obj->hdr.e_ident[EI_OSABI] = ELFOSABI_NONE;

  elf_obj->hdr.e_phoff = 0;
  elf_obj->hdr.e_flags = 0;
  elf_obj->hdr.e_phnum = 0;		/* no program header in object file */
  elf_obj->hdr.e_shnum = 0;		/* sections are added later */

  /* add section name section */
  elf_obj->section_str = elf_section_add(elf_obj, SHT_STRTAB, SHF_STRINGS);

  /* add empty string to section name section */
  elf_section_alloc(elf_obj->section_str, (aasm_u8_t*)"", 1);

  /* add comment section */
  comment = elf_section_add(elf_obj, SHT_PROGBITS, 0);

  elf_section_name(elf_obj, elf_obj->section_str, ".shstrtab");
  elf_section_name(elf_obj, comment, ".comment");

  {
    char		buf[1024];

    sprintf(buf, "Assembled with: " AASM_PROJECT_NAME " " PACKAGE_VERSION ", " AASM_PROJECT_DESC ". Object: %s. Copyright: %s. Description: %s",
	    obj->name, obj->copyright, obj->description);
    elf_section_str(comment, buf);
  }

  elf_obj->arch->write_Half(&elf_obj->hdr.e_type, ET_REL);
  elf_obj->arch->write_Word(&elf_obj->hdr.e_version, EV_CURRENT);
  elf_obj->arch->write_Off(&elf_obj->hdr.e_shoff, sizeof (Elf_Ehdr));	/* section table begin after file header */
  elf_obj->arch->write_Half(&elf_obj->hdr.e_ehsize, sizeof (Elf_Ehdr));
  elf_obj->arch->write_Half(&elf_obj->hdr.e_phentsize, sizeof (Elf32_Phdr));
  elf_obj->arch->write_Half(&elf_obj->hdr.e_shentsize, sizeof (Elf_Shdr));
  elf_obj->arch->write_Half(&elf_obj->hdr.e_shstrndx, elf_obj->section_str->index);;
}

struct out_object_s	*
out_elf_newobject	(struct object_s		*obj)
{
  struct out_object_s	*out_obj;

  /* creat out object on first call */
  out_obj = xzalloc(sizeof (struct out_object_s));

  /* init out_section & out_symbol alloc pools */
  mem_init(&out_obj->sec_pool, sizeof (struct out_section_s), &section_mem_desc);
  mem_init(&out_obj->sym_pool, sizeof (struct out_symbol_s), &symbol_mem_desc);

  out_obj->section_count = 1;

  return (out_obj);
}

void
out_elf_clean		(struct object_s		*obj)
{
  struct out_section_s		*elf_sec;

  for (elf_sec = obj->out_obj->section_first;
       elf_sec; elf_sec = elf_sec->next)
    {
      if (elf_sec->data)
	free(elf_sec->data);
    }

  mem_clean(&obj->out_obj->sec_pool);
  mem_clean(&obj->out_obj->sym_pool);

  free(obj->out_obj);
}

int
elf_write		(struct object_s		*obj,
			 FILE				*file)
{
  struct out_section_s		*elf_sec;

  /* get architecture specific elf information */
  if (!(obj->out_obj->arch = elf_get_arch(&obj->arch)))
    {
      error(ERRTYP_ERROR, "elf: unknown target architecture", 0);
      return (1);
    }

  /* set elf32.header */
  elf_init_header(obj->out_obj, obj, &obj->arch);
  elf_init_obj_sections(obj, obj->out_obj);
  elf_add_symbols(obj, obj->out_obj);
  elf_add_reloc(obj, obj->out_obj);

  if (error_count_g)
    return (1);

  /* check object base adress */
  if (obj->flag & OBJECT_BASEADDR)
    error(ERRTYP_WARN, "elf: object base address ignored", NULL);

  elf_write_obj_section(obj);

  /* skip header and section table */
  if (fseek(file, (signed)(sizeof (Elf_Ehdr)
			   + obj->out_obj->section_count
			   * sizeof (Elf_Shdr)), SEEK_SET))
    return (-1);

  elf_sec = obj->out_obj->section_first;

  /* write all section data */
  while (elf_sec)
    {
      elf_sec->offset = ftell(file);

      if (elf_sec->size && elf_sec->data)
	if (fwrite(elf_sec->data, elf_sec->size, 1, file) != 1)
	  return (-1);

      elf_sec = elf_sec->next;
    }

  /* write header */
  obj->out_obj->arch->write_Half(&obj->out_obj->hdr.e_shnum,
				 obj->out_obj->section_count);

  if (fseek(file, 0, SEEK_SET)
      || (fwrite(&obj->out_obj->hdr, sizeof (Elf_Ehdr), 1, file) != 1))
    return (-1);
 
  {
    Elf_Shdr	null_section;

    /* write null section */

    memset(&null_section, 0, sizeof(Elf_Shdr));
    if (fwrite(&null_section, sizeof (Elf_Shdr), 1, file) != 1)
      return (-1);

    /* write section table */

    elf_sec = obj->out_obj->section_first;
    while (elf_sec)
      {
	elf_prepare_section(obj->out_obj, elf_sec);
	
	if (fwrite(&elf_sec->entry, sizeof (Elf_Shdr), 1, file) != 1)
	  return (-1);

	elf_sec = elf_sec->next;
      }
  }

  return (0);
}

