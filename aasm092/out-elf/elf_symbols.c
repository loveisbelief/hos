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

#include "aasm/symbol.h"
#include "aasm/scope.h"
#include "aasm/section.h"
#include "aasm/proc.h"
#include "aasm/object.h"
#include "aasm/memory.h"
#include "aasm/alloc.h"

#include "elf.h"

static void
elf_sym_scope_lc	(struct scope_s		*scope,
			 struct out_object_s	*elf_obj,
			 struct out_section_s	*symtab,
			 struct out_section_s	*symstr)
{
  unsigned int		i;

  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s	*sym;

      for (sym = scope->symbols[i]; sym; sym = sym->next)
	{
	  unsigned int	value;
	  unsigned int	size;
	  unsigned int	sindex;
	  Elf_Sym	elf_sym;

	  if (!sym->out_sym)
	    continue;

	  if (sym->flag & (SYMBOL_EXPORT | SYMBOL_TYPE_EXTERN))
	    continue;

	  switch (sym->flag & SYMBOL_TYPE_MASK)
	    {
	    case (SYMBOL_TYPE_LABEL):
	      elf_sym.st_info = STT_OBJECT | (STB_LOCAL << 4);
	      value = sym->val.instr->next->offset;
	      size = 0;
	      sindex = sym->section->out_sec->index;
	      break;

	    case (SYMBOL_TYPE_PROC):
	      elf_sym.st_info = STT_FUNC | (STB_LOCAL << 4);
	      value = sym->val.proc->scope.first_instr.offset;
	      size = sym->val.proc->instr->len;
	      sindex = sym->section->out_sec->index;
	      break;

	    default:
	      continue;
	    }

	  elf_obj->arch->write_Word(&elf_sym.st_name,
				    elf_section_str(symstr, sym->name));
	  elf_obj->arch->write_Addr(&elf_sym.st_value, value);
	  elf_obj->arch->write_Xword(&elf_sym.st_size, size);
	  elf_obj->arch->write_Section(&elf_sym.st_shndx, sindex);

	  elf_sym.st_other = 0;
	  sym->out_sym->elf_id = ++elf_obj->symbol_count;

	  elf_section_alloc(symtab, (aasm_u8_t*)&elf_sym, sizeof (Elf_Sym));
	}
    }
}

static void
elf_sym_scope_gl	(struct symbol_s	*htable[],
			 struct out_object_s	*elf_obj,
			 struct out_section_s	*symtab,
			 struct out_section_s	*symstr)
{
  unsigned int		i;

  for (i = 0; i < ELF32_SYMBOL_HASH_SIZE; i++)
    {
      struct symbol_s	*sym;

      for (sym = htable[i]; sym; sym = sym->out_sym->next)
	{
	  unsigned int	value;
	  unsigned int	size;
	  unsigned int	sindex;
	  Elf_Sym	elf_sym;

	  switch (sym->flag & SYMBOL_TYPE_MASK)
	    {
	    case (SYMBOL_TYPE_LABEL):
	      elf_sym.st_info = STT_OBJECT | (STB_GLOBAL << 4);
	      value = sym->val.instr->next->offset;
	      size = 0;
	      sindex = sym->section->out_sec->index;
	      break;

	    case (SYMBOL_TYPE_PROC):
	      elf_sym.st_info = STT_FUNC | (STB_GLOBAL << 4);
	      value = sym->val.proc->scope.first_instr.offset;
	      size = sym->val.proc->instr->len;
	      sindex = sym->section->out_sec->index;
	      break;

	    case (SYMBOL_TYPE_EXTERN):
	      elf_sym.st_info = STT_NOTYPE | (STB_GLOBAL << 4);
	      value = 0;
	      size = 0;
	      sindex = 0;
	      break;

	    default:
	      continue;
	    }

	  elf_obj->arch->write_Word(&elf_sym.st_name,
				    elf_section_str(symstr, sym->name));
	  elf_obj->arch->write_Addr(&elf_sym.st_value, value);
	  elf_obj->arch->write_Xword(&elf_sym.st_size, size);
	  elf_obj->arch->write_Section(&elf_sym.st_shndx, sindex);

	  elf_sym.st_other = 0;
	  sym->out_sym->elf_id = ++elf_obj->symbol_count;

	  elf_section_alloc(symtab, (aasm_u8_t*)&elf_sym, sizeof (Elf_Sym));
	}
    }
}

static void
elf_sym_section		(struct object_s	*obj,
			 struct out_object_s	*elf_obj,
			 struct out_section_s	*symtab)
{
  struct section_s	*sec;

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      Elf_Sym	elf_sym;

      memset (&elf_sym, 0, sizeof (Elf_Sym));

      elf_sym.st_info = STT_SECTION | (STB_LOCAL << 4);
      elf_obj->arch->write_Section(&elf_sym.st_shndx, sec->out_sec->index);
      sec->out_sec->symbol_index = ++elf_obj->symbol_count;

      elf_section_alloc(symtab, (aasm_u8_t*)&elf_sym, sizeof (Elf_Sym));
    }
}

static void
elf_check_sym_scope	(struct scope_s		*scope,
			 struct symbol_s	*htable[])
{
  unsigned int	i;

  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s		*sym = scope->symbols[i];

      while (sym)
	{
	  struct symbol_s	*tmp;

	  if (!sym->out_sym)
	    goto next_sym;

	  if (!(sym->flag & (SYMBOL_EXPORT | SYMBOL_TYPE_EXTERN)))
	    goto next_sym;

	  /* look for symbol with same name */
	  for (tmp = htable[sym->hash % ELF32_SYMBOL_HASH_SIZE];
	       tmp; tmp = tmp->out_sym->next)
	    {
	      if ((sym->hash == tmp->hash)
		  && !strcmp(tmp->name, sym->name))
		{
		  struct error_s	*err;

		  /* allow multiple extern symbol, but keep it once */
		  if (sym->flag & tmp->flag & SYMBOL_TYPE_EXTERN)
		    sym->out_sym = tmp->out_sym; /* same named symbols share same out_sym */
		  else
		    {
		      err = error(ERRTYP_ERROR, "elf: exported symbol name `%s' already in use at %s", &sym->def_location);
		      error_submsg(err, sym->name);
		      error_sublocation(err, &tmp->def_location);
		    }

		  goto next_sym;
		}
	    }

	  sym->out_sym->next = htable[sym->hash % ELF32_SYMBOL_HASH_SIZE];
	  htable[sym->hash % ELF32_SYMBOL_HASH_SIZE] = sym;

	next_sym:
	  sym = sym->next;
	}
    }
}

void
elf_add_symbols		(struct object_s	*obj,
			 struct out_object_s	*elf_obj)
{
  struct section_s	*sec;
  struct out_section_s	*symtab;
  struct out_section_s	*symstr;
  Elf_Sym		empty_sym;

  /* creat symbols table section */
  elf_obj->section_sym = symtab = elf_section_add(elf_obj, SHT_SYMTAB, 0);
  elf_section_name(elf_obj, symtab, ".symtab");

  /* creat symbols name section */
  symstr = elf_section_add(elf_obj, SHT_STRTAB, SHF_STRINGS);
  elf_section_name(elf_obj, symstr, ".strtab");

  /* update symbol table info */
  elf_obj->arch->write_Word(&symtab->entry.sh_link, symstr->index);
  elf_obj->arch->write_Xword(&symtab->entry.sh_entsize, sizeof (Elf_Sym));

  /* add empty symbol to table */
  memset(&empty_sym, 0, sizeof (Elf_Sym));
  elf_section_alloc(symtab, (aasm_u8_t*)&empty_sym, sizeof (Elf_Sym));
  elf_section_alloc(symstr, (aasm_u8_t*)"", 1);

  /* add section special symbols */
  elf_sym_section(obj, elf_obj, symtab);

  /* walk thru object symbols */

  elf_sym_scope_lc(&obj->global, elf_obj, symtab, symstr);

  for (sec = obj->section_first; sec; sec = sec->next)
    {
      struct proc_s	*proc;

      /* local symbols first */
      elf_sym_scope_lc(&sec->scope, elf_obj, symtab, symstr);

      for (proc = sec->proc_first; proc; proc = proc->next)
	elf_sym_scope_lc(&proc->scope, elf_obj, symtab, symstr);

    }

  /* set index of first global symbol */
  elf_obj->arch->write_Word(&symtab->entry.sh_info, elf_obj->symbol_count + 1);

  {
    struct symbol_s	*htable[ELF32_SYMBOL_HASH_SIZE];

    memset(htable, 0, ELF32_SYMBOL_HASH_SIZE * sizeof (struct symbol_s *));

    elf_check_sym_scope(&obj->global, htable);

    for (sec = obj->section_first; sec; sec = sec->next)
      {
	struct proc_s	*proc;
      
	/* global symbols next */
	elf_check_sym_scope(&sec->scope, htable);

	for (proc = sec->proc_first; proc; proc = proc->next)
	  elf_check_sym_scope(&proc->scope, htable);

      }

    elf_sym_scope_gl(htable, elf_obj, symtab, symstr);
  }
}

struct out_symbol_s	*
out_elf_newsymbol	(struct object_s		*obj,
			 struct scope_s			*sc,
			 struct symbol_s		*sym)
{
  if (sym->flag & (SYMBOL_TYPE_LABEL | SYMBOL_TYPE_PROC | SYMBOL_TYPE_EXTERN))
    {
      struct out_symbol_s	*elf_sym = mem_pop(&obj->out_obj->sym_pool);

      memset(elf_sym, 0, sizeof (struct out_symbol_s));
      return (elf_sym);
    }

  return (NULL);
}

