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

#ifndef ELF32_H_
# define ELF32_H_

#include "aasm/types.h"
#include "aasm/arch_id.h"
#include "aasm/mod_out_import.h"
#include "aasm/memory.h"

#include "elf_bits.h"

#ifdef ELF_64BITS

typedef Elf64_Ehdr			Elf_Ehdr;
typedef Elf64_Shdr			Elf_Shdr;
typedef Elf64_Sym			Elf_Sym;
typedef Elf64_Rel			Elf_Rel;
typedef Elf64_Rela			Elf_Rela;

# define out_elf_write			out_elf64_write
# define out_elf_directive		out_elf64_directive
# define out_elf_newobject		out_elf64_newobject
# define out_elf_newsection		out_elf64_newsection
# define out_elf_newsymbol		out_elf64_newsymbol
# define out_elf_clean			out_elf64_clean

/* redefine other exported symbol names
   to avoid colision with elf32 module */

# define elf_add_symbols		elf64_add_symbols
# define elf_section_new		elf64_section_new
# define elf_section_add		elf64_section_add
# define elf_section_alloc		elf64_section_alloc
# define elf_section_name		elf64_section_name
# define elf_section_str		elf64_section_str
# define elf_prepare_section		elf64_prepare_section
# define elf_add_reloc			elf64_add_reloc
# define elf_init_obj_sections		elf64_init_obj_sections
# define elf_write_obj_section		elf64_write_obj_section
# define elf_write_ulsb32		elf64_write_ulsb32
# define elf_write_umsb32		elf64_write_umsb32
# define elf_write_slsb32		elf64_write_slsb32
# define elf_write_smsb32		elf64_write_smsb32
# define elf_write_ulsb64		elf64_write_ulsb64
# define elf_write_umsb64		elf64_write_umsb64
# define elf_write_slsb64		elf64_write_slsb64
# define elf_write_smsb64		elf64_write_smsb64
# define elf_write_ulsb16		elf64_write_ulsb16
# define elf_write_umsb16		elf64_write_umsb16
# define elf_get_arch			elf64_get_arch
# define elf_write			elf64_write

# define write_Half(a, b)		write_16u(a, (aasm_u16_t)(b))
# define write_Word(a, b)		write_32u(a, (aasm_u32_t)(b))
# define write_Xword(a, b)		write_64u(a, (aasm_u64_t)(b))
# define write_Sxword(a, b)		write_64s(a, (aasm_s64_t)(b))
# define write_Addr(a, b)		write_64u(a, (aasm_u64_t)(b))
# define write_Off(a, b)		write_64u(a, (aasm_u64_t)(b))
# define write_Section(a, b)		write_16u(a, (aasm_u16_t)(b))

# define ELF_R_INFO			ELF64_R_INFO

#else

typedef Elf32_Ehdr			Elf_Ehdr;
typedef Elf32_Shdr			Elf_Shdr;
typedef Elf32_Sym			Elf_Sym;
typedef Elf32_Rel			Elf_Rel;
typedef Elf32_Rela			Elf_Rela;

# define out_elf_write			out_elf32_write
# define out_elf_directive		out_elf32_directive
# define out_elf_newobject		out_elf32_newobject
# define out_elf_newsection		out_elf32_newsection
# define out_elf_newsymbol		out_elf32_newsymbol
# define out_elf_clean			out_elf32_clean

# define write_Half(a, b)		write_16u(a, (aasm_u16_t)(b))
# define write_Word(a, b)		write_32u(a, (aasm_u32_t)(b))
# define write_Xword(a, b)		write_32u(a, (aasm_u32_t)(b))
# define write_Sxword(a, b)		write_32s(a, (aasm_s32_t)(b))
# define write_Addr(a, b)		write_32u(a, (aasm_u32_t)(b))
# define write_Off(a, b)		write_32u(a, (aasm_u32_t)(b))
# define write_Section(a, b)		write_16u(a, (aasm_u16_t)(b))

# define ELF_R_INFO			ELF32_R_INFO

#endif

#define ELF32_REALLOC_SIZE		4096
#define MAX_SECNAME_LEN			128

struct					reloc_s;
struct					object_s;

struct					out_arch_s
{
  /* endian dependant write */
  void					(*write_16u)(void		*ptr,
						     aasm_u32_t		val);
  void					(*write_32u)(void		*ptr,
						     aasm_u32_t		val);
  void					(*write_64u)(void		*ptr,
						     aasm_u64_t		val);
  void					(*write_32s)(void		*ptr,
						     aasm_s32_t		val);
  void					(*write_64s)(void		*ptr,
						     aasm_s64_t		val);

  unsigned char				e_ident; /* endian elf.header field */
  Elf32_Half				e_machine;

  /* elf relocations type get */

  unsigned int				(*get_rtype)(struct reloc_s	*rel);

  unsigned int				flag;
};

#define ELF_ARCHFLG_NONE		0
#define ELF_ARCHFLG_RELA		0x1 /* always use rela. relocations section type */

struct					out_section_s
{
  Elf_Shdr				entry;

  aasm_u8_t				*data;
  unsigned int				alloc_size;

  unsigned int				index;

  unsigned int				symbol_index; /* special LOCAL symbol pointing to section */

  struct out_section_s			*next;

  /* fields copyed to entry.* before file write */
  unsigned int				name;
  unsigned int				offset;
  unsigned int				size;
};

struct					out_object_s
{
  struct mem_s				sec_pool;
  struct mem_s				sym_pool;

  Elf_Ehdr				hdr;

  const struct out_arch_s		*arch;

  struct out_section_s			*section_str;
  struct out_section_s			*section_sym;

  struct out_section_s			*section_first;
  struct out_section_s			*section_last;

  unsigned int				symbol_count;
  unsigned int				section_count;
};

struct					out_symbol_s
{
  unsigned int				elf_id;
  struct symbol_s			*next;
};

# define ELF32_SYMBOL_HASH_SIZE		1024

/*
** elf.c
*/

int
elf_write		(struct object_s	*obj,
			 FILE			*file);

/*
** elf_arch.c
*/

const struct out_arch_s	*
elf_get_arch		(struct arch_s		*arch);

/*
** elf_symbols.c
*/

void
elf_add_symbols		(struct object_s	*obj,
			 struct out_object_s	*elf_obj);

struct out_symbol_s	*
out_elf_newsymbol	(struct object_s	*obj,
			 struct scope_s		*sc,
			 struct symbol_s	*sym);

/*
** elf_sections.c
*/

struct out_section_s	*
elf_section_new		(struct out_object_s	*elf_obj);

struct out_section_s	*
elf_section_add		(struct out_object_s	*elf_obj,
			 unsigned int		type,
			 unsigned int		flags);

aasm_u8_t		*
elf_section_alloc	(struct out_section_s	*elf_sec,
			 const aasm_u8_t	*data,
			 unsigned int		len);

void
elf_section_name	(struct out_object_s	*elf_obj,
			 struct out_section_s	*elf_sec,
			 const char		*name);

unsigned int
elf_section_str		(struct out_section_s	*elf_sec,
			 const char		*str);

void
elf_prepare_section	(struct out_object_s	*elf_obj,
			 struct out_section_s	*elf_sec);

/*
** elf_endian.c
*/

void
elf_write_slsb64	(void			*ptr,
			 aasm_s64_t		val);

void
elf_write_smsb64	(void			*ptr,
			 aasm_s64_t		val);

void
elf_write_slsb32	(void			*ptr,
			 aasm_s32_t		val);

void
elf_write_smsb32	(void			*ptr,
			 aasm_s32_t		val);

void
elf_write_ulsb64	(void			*ptr,
			 aasm_u64_t		val);

void
elf_write_umsb64	(void			*ptr,
			 aasm_u64_t		val);

void
elf_write_ulsb32	(void			*ptr,
			 aasm_u32_t		val);

void
elf_write_umsb32	(void			*ptr,
			 aasm_u32_t		val);

void
elf_write_ulsb16	(void			*ptr,
			 aasm_u32_t		val);

void
elf_write_umsb16	(void			*ptr,
			 aasm_u32_t		val);

/*
** elf_obj_sections.c
*/

struct out_section_s	*
out_elf_newsection	(struct object_s	*obj,
			 struct section_s	*sec);

void
elf_init_obj_sections	(struct object_s	*obj,
			 struct out_object_s	*elf_obj);

void
elf_write_obj_section	(struct object_s	*obj);

/*
** elf_reloc.c
*/

void
elf_add_reloc		(struct object_s	*obj,
			 struct out_object_s	*elf_obj);

out_write_t		out_elf_write;
out_directive_t		out_elf_directive;
out_newobject_t		out_elf_newobject;
out_newsection_t	out_elf_newsection;
out_newsymbol_t		out_elf_newsymbol;
out_clean_t		out_elf_clean;

#endif

