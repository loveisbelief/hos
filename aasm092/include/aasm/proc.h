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

#ifndef PROC_H_
# define PROC_H_

#include "scope.h"

struct				symbol_s;
struct				section_s;
struct				obkect_s;
struct				error_locat_s;
struct				instr_s;

struct				proc_s
{
  struct instr_s		*instr;

  struct symbol_s		*sym; /* pointer to procedure symbol */
  struct scope_s		scope;

  struct proc_s			*next;
};

struct proc_s	*proc_new		(struct section_s		*section,
					 struct symbol_s		*sym);

void		procs_free		(struct section_s		*sec);

void		proc_set_current	(struct object_s		*obj,
					 struct proc_s			*proc,
					 const struct error_locat_s	*location);

void		proc_end_current	(struct object_s		*obj,
					 const struct error_locat_s	*location);

int		proc_check		(struct proc_s			*proc);

void		proc_clean		(void);
void		proc_init		(void);

#endif

