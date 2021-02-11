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

#include <string.h>

#include "aasm/scope.h"
#include "aasm/instr.h"
#include "aasm/proc.h"
#include "aasm/error.h"
#include "aasm/symbol.h"
#include "aasm/alloc.h"
#include "aasm/expr.h"
#include "aasm/section.h"
#include "aasm/object.h"

struct scope_s		meta_scope_g;

void
scope_init		(struct scope_s			*scope,
			 struct scope_s			*parent,
			 unsigned int			hash_size)
{
  scope->symbols = xzalloc(sizeof (struct symbol_s *) * hash_size);
  scope->symbol_hash_size = hash_size;
  scope->parent = parent;
  scope->last_instr.prev = &scope->first_instr;
  scope->first_instr.next = &scope->last_instr;

  if (parent)
    {
      scope->next = scope->parent->child;
      scope->parent->child = scope;
    }
}

void
scope_end		(struct scope_s			*scope,
			 const struct error_locat_s	*location)
{
  unsigned int			i;

  if (!scope->parent)
    return;

  /* move all undefined symbols to parent scope */
  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s		**sym;
      struct symbol_s		*sym2;

      for (sym = &scope->symbols[i]; (sym2 = *sym); )
	{
	  if (!(sym2->flag & SYMBOL_DEFINED))
	    {
	      unsigned int	hash;

	      hash = sym2->hash % scope->parent->symbol_hash_size;
	      *sym = sym2->next;

	      sym2->next = scope->parent->symbols[hash];
	      scope->parent->symbols[hash] = sym2;
	    }
	  else
	    sym = &sym2->next;
	}
    }  

  scope->last_instr.location = dup_location(location);
}

void
scope_syms_check	(struct scope_s			*scope)
{
  unsigned int			i;

  /* check use of undefined symbols */
  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s		*sym;

      for (sym = scope->symbols[i]; sym; sym = sym->next)
	{
	  switch (sym->flag & SYMBOL_STAT)
	    {
	    case (SYMBOL_USED):
	      error_submsg(error(ERRTYP_ERROR,
				 "use of undefined symbol `%s' (reported only once)",
				 &sym->use_location), sym->name);
	      break;

	    case (SYMBOL_DEFINED):
	      if (!(sym->flag & (SYMBOL_EXPORT | SYMBOL_TYPE_EXPR)))
		{
		  static const char	*sym_typ_name[65] =
		    {
		      "", "label", "function", "", "expression",
		      "", "", "", "external symbol", "", "", "",
		      "", "", "", "", "macro", "", "", "", "", ""
		      "", "", "", "", "", "", "", "", "", "", ""
		      "", "", "", "", "", "", "", "", "", "", "", 
		      "", "", "", "", "parameter"
		    };
		  struct error_s	*err;

		  err = error(ERRTYP_WARN_MORE,
			      "%s `%s' defined but not used",
			      &sym->def_location);
		  
		  error_submsg(err, sym_typ_name[sym->flag & SYMBOL_TYPE_MASK]);
		  error_submsg(err, sym->name);
		}

	      break;
	    }
	}
    }
}

void
scope_instrs_check	(struct scope_s			*scope,
			 struct section_s		*sec)
{
  struct instr_s	*instr;

  switch (sec->flag & (SECTION_TYPE_CODE | SECTION_TYPE_DATA | SECTION_TYPE_BSS))
    {
    case (SECTION_TYPE_CODE):
      if (error_mask_g & ERRTYP_WARN_MORE) /* skip useless test if no warning */
	for (instr = &scope->first_instr; instr->next; instr = instr->next)
	  {
	    if (instr->flag & INSTR_TYPE_DATA)
	      error(ERRTYP_WARN_MORE, "data in code section", instr->location);
	    if (instr->flag & INSTR_TYPE_UNINIT)
	      error(ERRTYP_WARN_MORE, "uninitialized data in code section", instr->location);
	  }
      break;

    case (SECTION_TYPE_DATA):
      if (error_mask_g & ERRTYP_WARN_MORE) /* skip useless test if no warning */
	for (instr = &scope->first_instr; instr->next; instr = instr->next)
	  {
	    if (instr->flag & INSTR_TYPE_OPCODE)
	      error(ERRTYP_WARN_MORE, "cpu instruction in data section", instr->location);
	  }
      break;

    case (SECTION_TYPE_BSS):
      if (error_mask_g & ERRTYP_WARN) /* skip useless test algorithm if no warning */
	for (instr = &scope->first_instr; instr->next; instr = instr->next)
	  {
	    if (instr->flag & INSTR_TYPE_OPCODE)
	      error(ERRTYP_WARN, "instruction initialize memory in bss section", instr->location);
	    if (instr->flag & INSTR_TYPE_DATA)
	      error(ERRTYP_WARN, "data initialize memory in bss section", instr->location);
	  }
      break;
    }
}

void
scope_assemble		(struct scope_s			*scope,
			 struct section_s		*sec,
			 struct object_s		*obj)
{
  struct instr_s	*instr;

  /* assemble opcode instructions */
  for (instr = &scope->first_instr; instr->next; instr = instr->next)
    {
      if ((instr->flag & INSTR_TYPE_OPCODE)
	  && !(instr->flag & INSTR_ASM)) /* skip already assembled */
	obj->msc.cl.nm.asm_->call.asm_.process(instr, sec);
    }
}

unsigned int
scope_address		(struct scope_s			*scope,
			 struct section_s		*sec,
			 unsigned int			base)
{
  struct instr_s	*instr;
  unsigned int		offset = 0;

  for (instr = &scope->first_instr; instr; instr = instr->next)
    {
      instr->offset = base + offset;
      instr->flag |= INSTR_ADDR;

      switch (instr->flag & INSTR_TYPE_MASK)
	{
	case (INSTR_TYPE_OPCODE):
	case (INSTR_TYPE_DATA):
	case (INSTR_TYPE_UNINIT):
	  offset += instr->len;
	  break;

	case (INSTR_TYPE_PROC):
	  instr->len = scope_address(&instr->val.proc->scope,
				     sec, instr->offset);
	  offset += instr->len;
	  break;
	}

      switch (instr->flag & INSTR_ALIGN_MASK)
	{
	case (INSTR_ALIGN_OFFSET):
	  /* offset alignment */
	  if (offset > instr->align)
	    error(ERRTYP_ERROR, "alignment offset is less than current offset",
		      instr->next->location);
	  else
	    offset = instr->align;
	  break;

	case (INSTR_ALIGN_BOUNDARY):
	  /* boundary alignment */
	  if (instr->align >= 2)
	    {
	      if (offset % instr->align)
		offset += instr->align - (offset % instr->align);
	    }
	  else
	    error(ERRTYP_WARN_MORE, "alignment < 2 bytes ignored",
		  instr->next->location);

	  if ((instr->align & (instr->align - 1)))
	    error(ERRTYP_WARN_OPT, "alignment is not a power of 2",
		  instr->next->location);

	  if (!(sec->flag & SECTION_ALIGN))
	    error(ERRTYP_WARN_MORE, "alignment in non aligned section",
		  instr->next->location);
	  else
	    if (sec->align % instr->align)
	      error(ERRTYP_WARN_MORE, "alignment do not divide section alignment",
		    instr->next->location);
	  break;

	default:
	  break;
	}
    }

  return (offset);
}

void
scope_write	(struct scope_s			*scope,
		 aasm_u8_t			*buf)
{
  struct instr_s	*instr;

  for (instr = scope->first_instr.next; instr->next; instr = instr->next)
    {
      switch (instr->flag & INSTR_TYPE_MASK)
	{
	case (INSTR_TYPE_OPCODE):
	case (INSTR_TYPE_DATA):
	  memcpy(buf + instr->offset, instr->val.bin, instr->len);
	  break;

	case (INSTR_TYPE_PROC):
	  scope_write(&instr->val.proc->scope, buf);
	  break;

	default:
	  break;
	}
    }
}



