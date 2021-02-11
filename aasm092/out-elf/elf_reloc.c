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

#include "aasm/error.h"
#include "aasm/reloc.h"
#include "aasm/expr.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/object.h"

#include "elf.h"

static void
elf_reloc_noaddend	(struct section_s	*sec,
			 struct out_object_s	*elf_obj)
{
  struct reloc_s	*rel;
  struct out_section_s	*reltab;
  char			name[MAX_SECNAME_LEN];
  Elf_Rel		rel_entry;

  /* creat reloc table section */
  sprintf(name, ".rel%s", sec->name);
  reltab = elf_section_add(elf_obj, SHT_REL, 0);
  elf_section_name(elf_obj, reltab, name);

  /* set links to symbol table and text/data sections */
  elf_obj->arch->write_Word(&reltab->entry.sh_link, elf_obj->section_sym->index);
  elf_obj->arch->write_Word(&reltab->entry.sh_info, sec->out_sec->index);
  elf_obj->arch->write_Xword(&reltab->entry.sh_entsize, sizeof (Elf_Rel));

  for (rel = sec->reloc; rel; rel = rel->next)
    {
      struct expr_s	*expr = *rel->expr;

      /* add relocation entry in table */
      if (expr->flag & EXPR_TYPE_SYMBOL)
	elf_obj->arch->write_Xword(&rel_entry.r_info,
				   ELF_R_INFO(expr->val.sym->out_sym->elf_id,
					      elf_obj->arch->get_rtype(rel)));
      else
	elf_obj->arch->write_Xword(&rel_entry.r_info,
				   ELF_R_INFO(expr->val.sec->out_sec->symbol_index,
					      elf_obj->arch->get_rtype(rel)));

      elf_obj->arch->write_Addr(&rel_entry.r_offset, rel->instr->offset + rel->offset);

      elf_section_alloc(reltab, (aasm_u8_t*)&rel_entry, sizeof (Elf_Rel));
    }
}

static void
elf_reloc_addend	(struct section_s	*sec,
			 struct out_object_s	*elf_obj)
{
  struct reloc_s	*rel;
  struct out_section_s	*reltab;
  char			name[MAX_SECNAME_LEN];
  Elf_Rela		rel_entry;

  /* creat reloc table section */
  sprintf(name, ".rela%s", sec->name);
  reltab = elf_section_add(elf_obj, SHT_RELA, 0);
  elf_section_name(elf_obj, reltab, name);

  /* set links to symbol table and text/data sections */
  elf_obj->arch->write_Word(&reltab->entry.sh_link, elf_obj->section_sym->index);
  elf_obj->arch->write_Word(&reltab->entry.sh_info, sec->out_sec->index);
  elf_obj->arch->write_Xword(&reltab->entry.sh_entsize, sizeof (Elf_Rela));

  for (rel = sec->reloc; rel; rel = rel->next)
    {
      struct expr_s	*expr = *rel->expr;

      /* add relocation entry in table */
      if (expr->flag & EXPR_TYPE_SYMBOL)
	elf_obj->arch->write_Xword(&rel_entry.r_info,
				   ELF_R_INFO(expr->val.sym->out_sym->elf_id,
					      elf_obj->arch->get_rtype(rel)));
      else
	elf_obj->arch->write_Xword(&rel_entry.r_info,
				   ELF_R_INFO(expr->val.sec->out_sec->symbol_index,
					      elf_obj->arch->get_rtype(rel)));

      elf_obj->arch->write_Addr(&rel_entry.r_offset, rel->instr->offset + rel->offset);

      /* set addend value if needed */
      if ((rel->flag & RELOC_ADDEND) && (rel->flag & RELOC_ADDEND_VALUE))
	elf_obj->arch->write_Sxword(&rel_entry.r_addend, rel->addend);
      else
	rel_entry.r_addend = 0;

      elf_section_alloc(reltab, (aasm_u8_t*)&rel_entry, sizeof (Elf_Rela));
    }
}

static void
elf_reloc_oper_add	(struct reloc_s		*rel)
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
	    error(ERRTYP_WARN, "elf: relocation addend overflow", expr->location);
	}
    }
}

static void
elf_add_reloc_sec	(struct section_s	*sec,
			 struct out_object_s	*elf_obj,
			 struct object_s	*obj)
{
  unsigned int			rela; /* need explicit addend reloc table ? */
  struct reloc_s		**reloc;

  /* architecture may require explicit addend */
  rela = elf_obj->arch->flag & ELF_ARCHFLG_RELA;

  for (reloc = &sec->reloc; *reloc;)
    {
      struct reloc_s		*rel = *reloc;

      /* reduce expression */
      expr_reduce(rel->expr);

      /* try to reduce (A + n) to A relocation with addend */
      elf_reloc_oper_add(rel);

      /* elf allow direct addend in instruction */
      if (!rela && (rel->flag & RELOC_ADDEND))
	rel->flag |= RELOC_ADDEND_DIRECT_OUT;

      /* try to reduce relocation */
      if (rel->reduce(rel, sec, obj))
	{
	  /* remove reduced relocation */
	  *reloc = rel->next;
	  reloc_free(rel);
	  continue;
	}

      if (!rela && (rel->flag & RELOC_ADDEND) && (rel->flag & RELOC_ADDEND_VALUE))
	error(ERRTYP_ERROR, "elf: unable to write implicit relocation addend",
	      (*rel->expr)->location);

      /* check relocation expr is a symbol */
      switch ((*rel->expr)->flag & EXPR_TYPE_MASK)
	{
	case (EXPR_TYPE_SYMBOL):
	  if (!(*rel->expr)->val.sym->out_sym)
	    error(ERRTYP_ERROR, "elf: relocation on non exportable symbol type", (*rel->expr)->location);

	case (EXPR_TYPE_SECTION):
	  break;

	default:
	  error(ERRTYP_ERROR, "elf: can not reduce relocation to section, symbol or value", (*rel->expr)->location);
	}

      reloc = &rel->next;
    }

  if (error_count_g)
    return;

  if (rela)
      /* use reloc table with addends */
      elf_reloc_addend(sec, elf_obj);
  else
      /* use reloc table without addend */
      elf_reloc_noaddend(sec, elf_obj);

  return;
}

void
elf_add_reloc		(struct object_s	*obj,
			 struct out_object_s	*elf_obj)
{
  struct section_s	*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    if (sec->reloc)
      elf_add_reloc_sec(sec, elf_obj, obj);

}

