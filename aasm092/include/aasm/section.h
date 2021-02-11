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

#ifndef SECTION_H_
# define SECTION_H_

#include "scope.h"
#include "error.h"
#include "types.h"

struct				out_section_s;

struct				reloc_s;
struct				proc_s;
struct				object_s;

# define SECTION_TYPE_CODE	0x1
# define SECTION_TYPE_DATA	0x2
# define SECTION_TYPE_BSS	0x4

# define SECTION_TYPE_READONLY	0x8

# define SECTION_TYPE_MASK	(SECTION_TYPE_CODE | SECTION_TYPE_DATA\
				| SECTION_TYPE_BSS | SECTION_TYPE_READONLY)

# define SECTION_DEFINED	0x100
# define SECTION_BINARY		0x200		/* binary form present */
# define SECTION_SOURCE		0x400		/* source form present */

# define SECTION_BASEADDR	0x1000		/* base address has been defined */
# define SECTION_ALIGN		0x2000

struct				section_s
{
  unsigned int			flag;
  unsigned int			asm_flag;	/* asm module specific flag */

  const char			*name;
  struct error_locat_s		def_location;
  struct error_locat_s		use_location;

  struct section_s		*next;

  struct out_section_s		*out_sec;	/* may be used by out module to handle specific data */

  /* if SECTION_BINARY */
  unsigned int			address;	/* section base address */
  unsigned int			size;		/* section size */
  unsigned int			align;		/* section align */
  struct reloc_s		*reloc;		/* relocations list */

  /* if SECTION_BINARY or SECTION_SOURCE */
  struct scope_s		scope;		/* section global scope */
  struct proc_s			*proc_first;	/* procedures */
  struct proc_s			*proc_last;	/* procedures */
};

struct section_s	*
section_new		(struct object_s		*obj,
			 const char			*name);

struct section_s	*
section_find		(struct object_s		*obj,
			 const char			*name);

int
section_define		(struct object_s		*obj,
			 struct section_s		*section,
			 unsigned int			type,
			 const struct error_locat_s	*location);

int
section_set_base	(struct section_s		*sec,
			 unsigned int			base);

int
section_set_align	(struct section_s		*sec,
			 unsigned int			align);

void
section_end_current	(struct object_s		*obj,
			 const struct error_locat_s	*location);

void
section_check		(struct section_s		*sec);

void
section_assemble	(struct section_s		*sec,
			 struct object_s		*obj);

void
section_address_reduce	(struct section_s		*sec);

void
section_write		(struct section_s		*sec,
			 aasm_u8_t			*buf);

void
section_free		(struct section_s		*sec);

void
section_clean		(void);

void
section_init		(void);

#endif

