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

#include "aasm/error.h"
#include "aasm/reloc.h"
#include "aasm/section.h"
#include "aasm/object.h"
#include "aasm/expr.h"
#include "aasm/symbol.h"

#include "coff.h"

static void
coff_reloc_oper_add	(struct reloc_s		*rel)
{
  struct expr_s		*expr = *rel->expr;
  int			old_addend = rel->addend;
  int			value;

  if ((expr->flag & EXPR_TYPE_OPERATION)
      && (expr->val.op == &oper_b_add))
    {
      if ((expr->right->flag & EXPR_TYPE_NUM)
	  && !num_get_int(&expr->right->val.num, &value))
	{
	  *rel->expr = expr->left;
	  expr_node_free(expr->right);

	  goto reloc_oper_add_done;
	}

      if ((expr->left->flag & EXPR_TYPE_NUM)
	  && !num_get_int(&expr->left->val.num, &value))
	{
	  *rel->expr = expr->right;
	  expr_node_free(expr->left);

	reloc_oper_add_done:
	  rel->addend += value;
	  expr_node_free(expr);
	  rel->flag |= RELOC_ADDEND | RELOC_ADDEND_VALUE;

	  /* check signed addition overflow */
	  if (((value ^ old_addend) > 0)
	      && ((value ^ rel->addend) < 0))
	    error(ERRTYP_WARN, "coff: relocation addend overflow", expr->location);
	}
    }
}

int
coff_write_relocs	(struct object_s		*obj,
			 FILE				*file)
{
  struct section_s	*sec;
  struct out_object_s	*out_obj = obj->out_obj;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      struct reloc_s	*rel;
      unsigned int	reloc_count = 0;

      sec->out_sec->reloc_pos = ftell(file);

      for (rel = sec->reloc; rel; rel = rel->next)
	{
	  struct expr_s		*expr;
	  unsigned int		coff_reltype;
	  struct coff_reloc_s	entry;

	  /* reduce expression */
	  expr_reduce(rel->expr);

	  /* try to reduce (A + n) to A relocation with addend */
	  coff_reloc_oper_add(rel);

	  /* get relocation type and adjust addend value
	     depending on format variant */
	  coff_reltype = out_obj->arch->set_addend(out_obj, rel);

	  /* coff allow direct addend in instruction */
	  if (rel->flag & RELOC_ADDEND)
	    rel->flag |= RELOC_ADDEND_DIRECT_OUT;

	  /* try to reduce relocation */
	  if (rel->reduce(rel, sec, obj))
	    continue;

	  expr = *rel->expr;	/* shortcut */

	  /* check for remaining addends */
	  if ((rel->flag & RELOC_ADDEND) && (rel->flag & RELOC_ADDEND_VALUE))
	    {
	      error(ERRTYP_ERROR,
		    "coff: unable to handle explicit addends for relocations",
		    expr->location);
	      continue;
	    }

	  /* write relocation address and type */
	  out_obj->arch->write_32(&entry.address, rel->instr->offset + rel->offset);
	  out_obj->arch->write_16(&entry.type, coff_reltype);

	  /* check relocation expr type and set symbol index */
	  switch (expr->flag & EXPR_TYPE_MASK)
	    {
	    case (EXPR_TYPE_SECTION):
	      out_obj->arch->write_32(&entry.symbol, expr->val.sec->out_sec->symbol_id);
	      break;

	    case (EXPR_TYPE_SYMBOL):

	      /* symbol have to be exportable */
	      if (!(expr->val.sym->out_sym))
		{
		  error(ERRTYP_ERROR, "coff: relocation on non exportable symbol type", expr->location);
		  continue;
		}

	      out_obj->arch->write_32(&entry.symbol, expr->val.sym->out_sym->out_id);
	      break;

	    default:
	      error(ERRTYP_ERROR, "coff: can not reduce relocation to section, symbol or value", expr->location);
	      continue;
	    }

	  /* write entry to file */
	  if (fwrite(&entry, COFF_RELOC_S_SIZE, 1, file) != 1)
	    return (RETVAL_ERR_IO);

	  reloc_count++;
	}

      sec->out_sec->reloc_count = reloc_count;
      out_obj->reloc_count += reloc_count;
    }
  
  if (error_count_g)
    return (RETVAL_ERR_MISC);

  return (0);
}

