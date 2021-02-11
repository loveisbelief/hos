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

#ifndef COFF_H_
# define COFF_H_

#include "aasm/types.h"
#include "aasm/arch_id.h"
#include "aasm/mod_out_import.h"
#include "aasm/memory.h"

#include "coff_bits.h"

struct					object_s;
struct					reloc_s;

/*
** Architecture specific informations and functions
** provide write function with correct endian and
** relocation ID.
**
** These functions depend on target architecture and
** have to be set on object creation or before processing
*/

struct					out_arch_s
{
  /* endian dependant write funstions */
  void					(*write_16)(void			*ptr,
						    aasm_u32_t			val);
  void					(*write_32)(void			*ptr,
						    aasm_u32_t			val);

  /* return output format relocations type from relocation and fix addend */
  unsigned int				(*set_addend)(struct out_object_s	*out_obj,
						      struct reloc_s		*rel);

  aasm_u16_t				magic;
};

struct					out_symbol_s
{
  struct symbol_s			*next;

  unsigned int				out_id;
  unsigned int				offset;
};

# define OUT_SYMBOL_HASH_SIZE		1024

/*
** Output section descriptor
*/

struct					out_section_s
{
  unsigned int				data_pos;

  unsigned int				index;
  unsigned int				symbol_id;

  struct out_section_s			*next;

  unsigned int				reloc_pos;
  unsigned int				reloc_count;

  struct section_s			*sec; /* section back link */
};

# define OUT_REALLOC_SIZE		4096

/*
** Output object descriptor
*/

#define COFF_VARIANT_UNDEF		0x00
#define COFF_VARIANT_GNU		0x01
#define COFF_VARIANT_MS			0x02
#define COFF_VARIANT_MASK		(COFF_VARIANT_GNU | COFF_VARIANT_MS)

#define COFF_NOTIME			0x100

struct					out_object_s
{
  unsigned int				flag;

  /* output sections allocation pool*/
  struct mem_s				sec_pool;

  struct out_section_s			*section_first;
  struct out_section_s			*section_last;

  struct coff_sechdr_s			*sec_table;
  unsigned int				section_count;
  unsigned int				reloc_count;

  /* output symbols allocation pool */
  struct mem_s				sym_pool;

  unsigned int				symtab_pos;

  unsigned int				sym_count; /* symbol count */
  unsigned int				sym_unresolved;	/* extern symbol count */
  unsigned int				sym_local; /* local symbol count */

  /* pointer to architecture specific structure */
  const struct out_arch_s		*arch;
};

/*
** functions declarations
*/

/* coff_reloc.c */

int
coff_write_relocs	(struct object_s	*obj,
			 FILE			*file);

/* coff_symbols.c file */

int
coff_write_symtable	(struct object_s	*obj,
			 FILE			*file);

/* coff_sections.c file */

int
coff_write_sechdr	(struct object_s	*obj,
			 FILE			*file);

int
coff_write_section	(struct object_s	*obj,
			 FILE			*file);

/* coff_arch.c file */

const struct out_arch_s	*
coff_get_arch		(struct arch_s		*arch);

/* coff_endian.c file */

void
coff_write_ulsb32	(void			*ptr,
			 aasm_u32_t		val);

void
coff_write_umsb32	(void			*ptr,
			 aasm_u32_t		val);

void
coff_write_ulsb16	(void			*ptr,
			 aasm_u32_t		val);

void
coff_write_umsb16	(void			*ptr,
			 aasm_u32_t		val);

#define RETVAL_ERR_IO	-1
#define RETVAL_ERR_MISC	-2

/*
** exported module functions declaration
*/

out_write_t		out_coff_write;
out_directive_t		out_coff_directive;
out_newobject_t		out_coff_newobject;
out_newsection_t	out_coff_newsection;
out_newsymbol_t		out_coff_newsymbol;
out_clean_t		out_coff_clean;

#endif

