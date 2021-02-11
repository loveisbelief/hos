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

#ifndef SCOPE_H_
# define SCOPE_H_

#include "instr.h"
#include "types.h"

struct				symbol_s;
struct				scope_s;
struct				comment_s;
struct				section_s;
struct				object_s;

# define MACRO_SYM_HASHSIZE	3	/* prime number */
# define PROC_SYM_HASHSIZE	11	/* prime number */
# define SECTION_SYM_HASHSIZE	97	/* prime number */
# define GLOBAL_SYM_HASHSIZE	383	/* prime number */
# define META_SYM_HASHSIZE	29	/* prime number */

struct				scope_s
{
  unsigned int			symbol_hash_size; /* hash table size */
  struct symbol_s		**symbols; /* symbol head pointers hash table */
  unsigned int			symbol_count;

  struct instr_s		first_instr;
  struct instr_s		last_instr;

  struct scope_s		*parent; /* previous scope link */

  struct scope_s		*child;
  struct scope_s		*next;

  struct comment_s		*comments;
  struct section_s		*section; /* != NULL if non global scope */
};

extern struct scope_s          meta_scope_g;

void		scope_init		(struct scope_s			*scope,
					 struct scope_s			*parent,
					 unsigned int			hash_size);

void		scope_end		(struct scope_s			*scope,
					 const struct error_locat_s	*location);

void		scope_syms_check	(struct scope_s			*scope);

void		scope_instrs_check	(struct scope_s			*scope,
					 struct section_s		*sec);

void		scope_assemble		(struct scope_s			*scope,
					 struct section_s		*sec,
					 struct object_s		*obj);

unsigned int	scope_address		(struct scope_s			*scope,
					 struct section_s		*sec,
					 unsigned int			base);

void		scope_write		(struct scope_s			*scope,
					 aasm_u8_t			*buf);

void		scope_free		(struct scope_s			*scope);

#endif

