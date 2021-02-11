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

#include <assert.h>
#include <string.h>

#include "aasm/macro.h"
#include "aasm/error.h"
#include "aasm/symbol.h"
#include "aasm/expr.h"
#include "aasm/object.h"
#include "aasm/proc.h"
#include "aasm/section.h"
#include "aasm/memory.h"

void			macro_set_current(struct object_s		*obj,
					  struct macro_s		*macro,
					  const struct error_locat_s	*location)
{
  obj->cur_macro = macro;
  obj->cur_scope = &macro->scope;
}

void			macro_end_current(struct object_s		*obj,
					  const struct error_locat_s	*location)
{
  scope_end(obj->cur_scope, location);

  obj->cur_scope = obj->cur_scope->parent;
  obj->cur_macro = NULL;
}

static void		macro_replace_scope(struct scope_s		*scope)
{
  struct instr_s	*macro_instr;

  for (macro_instr = scope->first_instr.next; macro_instr->next;)
    {
      if ((macro_instr->flag & INSTR_TYPE_MACRO)
	  && !(macro_instr->flag & INSTR_MACRO_DONE))
	{
	  struct symbol_s	*label, *macro_sym = macro_instr->val.macro;
	  struct macro_s	*macro = macro_sym->val.macro;
	  struct instr_s	*template_instr, *new_instr;

	  if (!(macro_sym->flag & SYMBOL_DEFINED))
	    goto next_instr;		/* skip undefined macro symbol, catched later */

	  /* check symbol type */
	  if (!(macro_sym->flag & SYMBOL_TYPE_MACRO))
	    {
	      error_submsg(error(ERRTYP_ERROR, "non macro symbol `%s' used as macro", macro_instr->location), macro_sym->name);
	      macro_instr = macro_instr->next;
	      continue;
	    }

	  /* check parameters count */
	  if (macro->param_count != macro_instr->op_count)
	    {
	      error_submsg(error(ERRTYP_ERROR, "invalid parameter count for `%s' macro", macro_instr->location), macro_sym->name);
	      macro_instr = macro_instr->next;
	      continue;
	    }

	  /* check recursive use */
	  if (macro_sym->flag & SYMBOL_RECURS)
	    {
	      error_submsg(error(ERRTYP_ERROR, "recursive use of `%s' macro", macro_instr->location), macro_sym->name);
	      macro_instr = macro_instr->next;
	      continue;
	    }

	  macro_sym->flag |= SYMBOL_RECURS;

	  /* do replace job on sub macro before processing */
	  macro_replace_scope(&macro->scope);

	  /* insert all instruction from macro scope to scope */
	  new_instr = macro_instr;
	  for (template_instr = &macro->scope.first_instr;
	       template_instr;
	       template_instr = template_instr->next)
	    {
	      if (template_instr->flag & (INSTR_TYPE_OPCODE | INSTR_TYPE_DATA))
		{
		  unsigned int	i;

		  /* duplicate template instruction */
		  new_instr = instr_dup_src(template_instr, new_instr, dup_location(macro_instr->location));

		  new_instr->asm_flag = macro_instr->asm_flag;
		  /* replace parameters with macro operands */
		  for (i = 0; i < new_instr->op_count; i++)
		    {
		      expr_reduce(&new_instr->operands[i]);
		      expr_macro_replace(&new_instr->operands[i], macro_instr->operands);
		    }
		}
	    }

	  {
	    unsigned int		i;

	    /* walk thru symbol defined inside macro */
	    for (i = 0; i < macro->scope.symbol_hash_size; i++)
	      {
		struct symbol_s		*sym;
		
		for (sym = macro->scope.symbols[i]; sym; sym = sym->next)
		  {
		    switch (sym->flag & SYMBOL_TYPE_MASK)
		      {
			/* label not supported yet */
		      case (SYMBOL_TYPE_LABEL):
		      case (SYMBOL_TYPE_PROC):
			error(ERRTYP_ERROR, "label inside macro not supported yet", &sym->def_location);
			break;

			/* extern symbols have to be declared in target scope */
		      case (SYMBOL_TYPE_EXTERN):
			error(ERRTYP_ERROR, "extern inside macro not supported yet", &sym->def_location);
			break;

			/* other symbols are ignored */
		      default:
			break;
		      }
		  }
	      }
	  }

	  /* clear recurs detection flag */
	  macro_sym->flag ^= SYMBOL_RECURS;

	  /* set macro replaced flag on instruction */
	  macro_instr->flag |= INSTR_MACRO_DONE;

	  /* move labels pointing to macro instruction 
	     to last replaced instruction in macro */
	  if ((label = new_instr->label = macro_instr->label))
	  {

	    /* FIXME assumes new_instr->label is NULL
	       list should be concatained */
	    macro_instr->label = NULL;

	    do
	      {
		label->val.instr = new_instr;
		label = label->sibling;
	      }
	    while (label && (label != new_instr->label));
	  }

	  /* go to next instruction in scope */
	  macro_instr = new_instr->next;
	  continue;
	}

    next_instr:
      macro_instr = macro_instr->next;
    }
}

void			macro_replace(struct object_s		*obj)
{
  struct section_s	*sec;

  assert (obj != NULL);

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      struct proc_s		*proc;

      for (proc = sec->proc_first; proc; proc = proc->next)
	macro_replace_scope(&proc->scope);
      
      macro_replace_scope(&sec->scope);
    }
}

/* Memory management for macro */
static struct mem_s macro_pool_g;

static const struct mem_desc_s	macro_mem_desc =
  {
    "macro",
    NULL
  };

void
macro_init (void)
{
  mem_init (&macro_pool_g, sizeof (struct macro_s), &macro_mem_desc);
}

void
macro_clean (void)
{
  mem_clean (&macro_pool_g);
}

struct macro_s		*macro_new(struct scope_s	*par_scope,
				   struct symbol_s	*sym)
{
  struct macro_s		*macro = mem_pop (&macro_pool_g);

  memset(macro, 0, sizeof(struct macro_s));
  macro->sym = sym;
  scope_init(&macro->scope, par_scope, MACRO_SYM_HASHSIZE);

  return (macro);
}
