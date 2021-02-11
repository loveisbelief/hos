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

#ifndef RELOC_H_
# define RELOC_H_

struct				expr_s;
struct				instr_s;
struct				section_s;
struct				object_s;

#define RELOC_SIZE_MASK		0xffff

#define RELOC_TYPE_RELATIV	0x010000

#define RELOC_ADDEND		0x020000	/* relocation addend */
#define RELOC_ADDEND_VALUE	0x040000	/* relocation value is valid */
#define RELOC_ADDEND_DIRECT_OUT	0x080000	/* out module support direct addend in instruction */

#define RELOC_WARN_SIZE		0x200000	/* add warning if reloc size is larger than needed */

#define RELOC_LITTLE_ENDIAN	0x400000	/* may be used on endian-less architectures */
#define RELOC_SIGNED		0x800000


#define RELOC_REDUCE_PARAMS	struct reloc_s		*reloc,\
				struct section_s	*sec,\
				const struct object_s	*obj

struct				reloc_s
{
  unsigned int			flag;	/* relocation size and flag */
  unsigned int			bits_size;
  int				bits_shift;

  struct expr_s			*expr_;	/* direct relocation value */
  struct expr_s			**expr; /* indirect relocation value (ptr to direct if unused) */
  int				addend;

  struct instr_s		*instr;	/* pointer to relocated instruction */
  unsigned int			offset;	/* relocation offset in instruction buffer */

  int				(*reduce)(RELOC_REDUCE_PARAMS);

  struct reloc_s		*next;
};

struct reloc_s	*reloc_add		(struct section_s		*sec);
void		relocs_free		(struct section_s		*sec);

void		reloc_free		(struct reloc_s			*rel);

void		reloc_clean		(void);
void		reloc_init		(void);

#endif

