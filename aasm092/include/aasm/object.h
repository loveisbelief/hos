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

#ifndef OBJECT_H_
# define OBJECT_H_

#include "modules.h"
#include "scope.h"
#include "arch_id.h"

struct				out_object_s;

struct				expr_s;
struct				section_s;
struct				proc_s;
struct				macro_s;

# define OBJECT_BASEADDR	0x1		/* base address has been defined */
# define OBJECT_DEBUG		0x2		/* add debug information to object */

struct				object_s
{
  /* module used for object operations */

  unsigned int			flag;

  const char			*srcfilename;
  const char			*name;
  const char			*copyright;
  const char			*description;

  struct arch_s			arch;

  struct section_s		*section_first;
  struct section_s		*section_last;

  unsigned int			section_count;
  struct scope_s		global;

  unsigned int			address;	/* object base address */

  /* used during object building */
  struct scope_s		*cur_scope;
  struct section_s		*cur_section;
  struct proc_s			*cur_proc;
  struct macro_s		*cur_macro;

  struct module_scope_s		msc;		/* objcet modules scope */

  struct out_object_s		*out_obj;
};

void		object_check		(struct object_s	*obj,
					 const char		*filename);

void		object_init		(struct object_s	*obj);

int		object_set_name		(struct object_s	*obj,
					 const char		*str);

int		object_set_desc		(struct object_s	*obj,
					 const char		*str);

int		object_set_copyright	(struct object_s	*obj,
					 const char		*str);

int		object_set_base		(struct object_s	*obj,
					 unsigned int			base);

void		object_assemble		(struct object_s	*obj);

int		object_reloc		(struct object_s	*obj);

void		object_free		(struct object_s	*obj);


#endif

