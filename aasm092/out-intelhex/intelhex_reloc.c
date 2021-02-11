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

#include <stdlib.h>

#include "aasm/expr.h"
#include "aasm/reloc.h"
#include "aasm/symbol.h"
#include "aasm/proc.h"
#include "aasm/section.h"
#include "aasm/object.h"

#include "intelhex.h"

static int		intelhex_reloc_reduce(struct reloc_s	*rel,
					      struct expr_s	*expr,
					      struct object_s	*obj)
{
  switch (expr->flag & EXPR_TYPE_MASK)
    {

    case (EXPR_TYPE_SYMBOL):
      {
	struct instr_s	*lbl;
	struct symbol_s	*sym = expr->val.sym;
	
	switch (expr->val.sym->flag & SYMBOL_TYPE_MASK)
	  {
	  case (SYMBOL_TYPE_LABEL):
	    lbl = sym->val.instr;
	    break;
	    
	  case (SYMBOL_TYPE_PROC):
	    lbl = &sym->val.proc->scope.first_instr;
	    break;
	    
	  case (SYMBOL_TYPE_EXTERN):
	    error_submsg(error(ERRTYP_ERROR,
			       "intelhex: can't reduce `%s' external symbol relocation",
			       (*rel->expr)->location), sym->name);
	    return (0);
	    
	  default:
	    error(ERRTYP_ERROR, "intelhex: can't reduce symbol relocation to value",
		  (*rel->expr)->location);
	    return (0);
	  }

	if ((rel->flag & RELOC_ADDEND) && (rel->flag & RELOC_ADDEND_VALUE))
	  {
	    num_init_uint(&expr->val.num, sym->section->address
			  + lbl->next->offset
			  + rel->addend);
	    rel->flag ^= RELOC_ADDEND_VALUE;
	  }
	else
	  num_init_uint(&expr->val.num, sym->section->address
			+ lbl->next->offset);

	expr->flag ^= EXPR_TYPE_SYMBOL | EXPR_TYPE_NUM;
      }
      return (-1);		/* reduce done */

    case (EXPR_TYPE_OPERATION):
      return (((expr->val.op->flag & OPER_BINARY)
	       && intelhex_reloc_reduce(rel, expr->left, obj))
	      | intelhex_reloc_reduce(rel, expr->right, obj));

    default:
      return (0);
    }
}

void			intelhex_reloc(struct object_s		*obj)
{
  struct section_s		*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      struct reloc_s		**rel;
      struct reloc_s		*r;

      for (rel = &sec->reloc; (r = *rel); )
	{
	  /* reduce expression */
	  expr_reduce(r->expr);

	  if (r->reduce(r, sec, obj))
	    {
	      *rel = r->next;
	      reloc_free(r);
	      continue;
	    }

	  rel = &r->next;

	  /* reduce label and proc symbols to num */
	  if (!intelhex_reloc_reduce(r, *r->expr, obj))
	    error(ERRTYP_ERROR, "intelhex: can't reduce relocation",
		  (*r->expr)->location);
	}
    }

  if (error_count_g)
    return;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      struct reloc_s		*r;

      for (r = sec->reloc; r; r = r->next)
	{
	  /* reduce expression retry */
	  expr_reduce(r->expr);

	  if (r->reduce(r, sec, obj))
	    continue;

	  error(ERRTYP_ERROR, "intelhex: can't reduce relocation",
		(*r->expr)->location);
	}
    }
}

