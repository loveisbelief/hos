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

#ifndef MACRO_H_
# define MACRO_H_

#include "scope.h"

struct				object_s;
struct				section_s;
struct				symbol_s;

struct				macro_s
{
  unsigned int			param_count;

  struct symbol_s		*sym;
  struct scope_s		scope;
};

struct macro_s	*macro_new		(struct scope_s			*par_scope,
					 struct symbol_s		*sym);

void		macros_free		(struct section_s		*sec);

void		macro_set_current	(struct object_s		*obj,
					 struct macro_s			*macro,
					 const struct error_locat_s	*location);

void		macro_end_current	(struct object_s		*obj,
					 const struct error_locat_s	*location);

void		macro_replace		(struct object_s		*obj);

void		macros_free		(struct section_s		*sec);

void		macro_init		(void);
void		macro_clean		(void);

#endif

