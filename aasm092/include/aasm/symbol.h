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

#ifndef SYMBOL_H_
# define SYMBOL_H_

/*
** symbol defs
*/

# define SYMBOL_TYPE_LABEL	0x1
# define SYMBOL_TYPE_PROC	0x2
# define SYMBOL_TYPE_EXPR	0x4
# define SYMBOL_TYPE_EXTERN	0x8
# define SYMBOL_TYPE_MACRO	0x10
# define SYMBOL_TYPE_PARAM	0x20
# define SYMBOL_TYPE_ALIAS	0x40

# define SYMBOL_TYPE_MASK	(SYMBOL_TYPE_LABEL | SYMBOL_TYPE_PROC\
				| SYMBOL_TYPE_EXPR | SYMBOL_TYPE_EXTERN\
				| SYMBOL_TYPE_MACRO | SYMBOL_TYPE_PARAM | SYMBOL_TYPE_ALIAS)

# define SYMBOL_TYPE_EXPORTMASK	(SYMBOL_TYPE_LABEL | SYMBOL_TYPE_PROC | SYMBOL_TYPE_EXTERN) /* exportable symbol types */

# define SYMBOL_TYPE_REDEFMASK	(SYMBOL_TYPE_EXTERN)

# define SYMBOL_USED		0x100 /* symbol has already been defined, */
# define SYMBOL_DEFINED		0x200 /* symbol may be used before being defined */
# define SYMBOL_STAT		(SYMBOL_USED | SYMBOL_DEFINED)

# define SYMBOL_RECURS		0x800 /* recursiv use of symbol for expr/macro detection */

# define SYMBOL_EXPORT		0x4000

# define SYMBOL_LABEL_TAKEN	0x8000
# define SYMBOL_LABEL_NOTTAKEN	0x10000
# define SYMBOL_LABEL_LOOP	0x20000
# define SYMBOL_LABEL_CALL	0x40000

# define SYMBOL_LABEL_DATA	0x80000

# define SYMBOL_LABEL_CODE_MASK	(SYMBOL_LABEL_TAKEN | SYMBOL_LABEL_NOTTAKEN\
				| SYMBOL_LABEL_LOOP | SYMBOL_LABEL_CALL)

# define SYMBOL_LABEL_MASK	(SYMBOL_LABEL_CODE_MASK\
				| SYMBOL_LABEL_DATA)

# define SYMBOL_LABEL_SHIFT	15

#include "error.h"

struct				out_symbol_s;

struct				object_s;
struct				section_s;
struct				instr_s;
struct				proc_s;
struct				expr_s;
struct				macro_s;
struct				scope_s;

struct				symbol_s
{
  unsigned int			flag;
  unsigned int			hash;
  const char			*name;

  struct section_s		*section;
  struct symbol_s		*next; /* link to next symbol in current scope */
  struct symbol_s		*sibling; /* link to symbol sibling (ring list) */

  /* bellow defined if SYMBOL_DEFINED */

  struct error_locat_s		def_location;

  union
  {
    struct instr_s		*instr;	/* used if SYMBOL_LABEL */
    struct proc_s		*proc; /* used if SYMBOL_PROCEDURE */
    struct expr_s		*expr; /* used if SYMBOL_EXPR */
    struct macro_s		*macro;	/* used if SYMBOL_MACRO */
    unsigned int		number;	/* used if SYMBOL_PARAM */
    struct symbol_s		*link; /* used if SYMBOL_ALIAS */
    void			*_void;
  }				val;

  /* bellow defined if SYMBOL_USED */

  struct error_locat_s		use_location;	/* first use location */

  struct out_symbol_s		*out_sym;	/* may be used by out module to handle specific data */
};

struct symbol_s		*
symbol_creat		(const char		*name,
			 struct scope_s		*scope);

int
symbol_define		(struct symbol_s		*sym,
			 void				*val,
			 unsigned int			type,
			 struct scope_s			*scope,
			 const struct error_locat_s	*loc);

struct symbol_s		*
symbol_new		(const char			*name,
			 unsigned int			hash,
			 struct scope_s			*scope);

void
symbol_alias		(const char			*alias_name,
			 struct symbol_s		*sym,
			 struct scope_s			*scope);

void
symbol_sibling_add	(struct symbol_s		*grp_sym,
			 struct symbol_s		*sym);

struct symbol_s		*
symbol_use		(const char			*name,
			 struct scope_s			*scope,
			 int				depth);

struct symbol_s		*
symbol_isdef		(const char			*name,
			 struct scope_s			*scope);

void
symbol_free		(struct symbol_s		*sym);

int
symbol_cmp		(const struct symbol_s		*a,
			 const struct symbol_s		*b);

void
symbol_clean		(void);

void
symbol_init		(void);

#endif

