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

#include <stdio.h>

#include "aasm/object.h"
#include "aasm/symbol.h"
#include "aasm/section.h"
#include "aasm/proc.h"

#include "intelhex.h"

int			intelhex_write_sym(struct object_s	*obj,
					 FILE			*file)
{
  struct section_s	*sec;
  unsigned int		i;

  fprintf(file, ";Object base address:             %08x\n\n", obj->address);

  fprintf(file, ";Section                          Address  Size\n");

  for (sec = obj->section_first; sec; sec = sec->next)
    fprintf(file, " %-32s %08x %u\n", sec->name, sec->address, sec->size);

  fprintf(file, "\n;Section  Symbol                  Address  Type\n");

  for (sec = obj->section_first; sec; sec = sec->next)
    for (i = 0; i < sec->scope.symbol_hash_size; i++)
      {
	struct symbol_s			*sym;

	for (sym = sec->scope.symbols[i]; sym; sym = sym->next)
	  {
	    const char			*type;
	    const struct instr_s	*lbl;
	
	    if (!(sym->flag & SYMBOL_EXPORT))
	      continue;

	    switch (sym->flag & SYMBOL_TYPE_MASK)
	      {
	      case (SYMBOL_TYPE_LABEL):
		type = "LABEL";
		lbl = sym->val.instr;
		break;

	      case (SYMBOL_TYPE_PROC):
		type = "PROC";
		lbl = &sym->val.proc->scope.first_instr;
		break;

	      default:
		continue;
	      }

	    fprintf(file, " %-8s %-23s %08x %s\n", sec->name, sym->name,
		    sec->address + lbl->next->offset, type);
	  }
      }

  return (0);
}

