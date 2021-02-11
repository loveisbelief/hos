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

#include "coff.h"
#include "realloc.h"

static unsigned int
coff_check_sym_scope	(struct scope_s		*scope,
			 struct symbol_s	*htable[])
{
  unsigned int	sym_count = 0;
  unsigned int	i;

  /* walk thru symbol scope */
  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s		*sym = scope->symbols[i];

      while (sym)
	{
	  struct symbol_s	*tmp;

	  if (!sym->out_sym)
	    goto next_sym;

	  if (sym->flag & (SYMBOL_EXPORT | SYMBOL_TYPE_EXTERN))
	    {
	      /* look for symbol with same name */
	      for (tmp = htable[sym->hash % OUT_SYMBOL_HASH_SIZE];
		   tmp; tmp = tmp->out_sym->next)
		{
		  if ((tmp->flag & (SYMBOL_EXPORT | SYMBOL_TYPE_EXTERN))
		      && (sym->hash == tmp->hash)
		      && !strcmp(tmp->name, sym->name))
		    {
		      struct error_s	*err;

		      /* allow multiple extern symbol, but keep it once */
		      if (sym->flag & tmp->flag & SYMBOL_TYPE_EXTERN)
			sym->out_sym = tmp->out_sym; /* same named symbols share same out_sym */
		      else
			{
			  err = error(ERRTYP_ERROR, "coff: exported symbol name `%s' already in use at %s", &sym->def_location);
			  error_submsg(err, sym->name);
			  error_sublocation(err, &tmp->def_location);
			}

		      goto next_sym;
		    }
		}
	    }

	  /* add symbol in hash table */
	  sym->out_sym->next = htable[sym->hash % OUT_SYMBOL_HASH_SIZE];
	  htable[sym->hash % OUT_SYMBOL_HASH_SIZE] = sym;

	  /* inc symbol count in scope */
	  sym_count++;

	next_sym:
	  sym = sym->next;
	}
    }

  return (sym_count);		/* return symbol count */
}

int
coff_write_symtable		(struct object_s		*obj,
				 FILE				*file)
{
  struct symbol_s	*htable[OUT_SYMBOL_HASH_SIZE];
  unsigned int		sym_count = 0;

  /* sort and check symbols  */
  {
    struct section_s	*sec;

    /* initialize symbol hash table */
    memset(htable, 0, OUT_SYMBOL_HASH_SIZE * sizeof (struct symbol_s *));

    /* insert symbol in hash table and check duplicate names */
    sym_count += coff_check_sym_scope(&obj->global, htable);

    for (sec = obj->section_first; sec; sec = sec->next)
      {
	struct proc_s	*proc;

	/* insert symbol in hash table and check duplicate names */
	sym_count += coff_check_sym_scope(&sec->scope, htable);

	/* insert symbol in hash table and check duplicate names */
	for (proc = sec->proc_first; proc; proc = proc->next)
	  sym_count += coff_check_sym_scope(&proc->scope, htable);
      }

    /* fix total symbol count */
    obj->out_obj->sym_count = sym_count;
  }

  /* abbort if error */
  if (error_count_g)
    return (RETVAL_ERR_MISC);

  {
    unsigned int		i, j = 0;
    struct coff_sym_s		entry;
    struct out_object_s		*out_obj = obj->out_obj;
    struct out_section_s	*out_sec;

    /* memory for string table */
    REALLOC_DEF(str_table, char, OUT_REALLOC_SIZE);

    /* reserve four byte for string table size at beginning*/
    REALLOC_APPEND(str_table, "\0\0\0\0", 4);

    out_obj->symtab_pos = ftell(file);

    /* add section symbols */
    for (out_sec = out_obj->section_first; out_sec; out_sec = out_sec->next)
      {
	struct section_s	*sec = out_sec->sec;
	unsigned int		name_len = strlen(sec->name);

	memset (&entry, 0, sizeof (struct coff_sym_s));

	if (name_len <= COFF_SYM_SHRTNAM_SIZE)
	  /* write symbol short name */
	  strncpy(entry.name.short_name, sec->name, COFF_SYM_SHRTNAM_SIZE);
	else
	  /* write symbol long name */
	  {
	    entry.name.ext.is_longname = 0;
	    out_obj->arch->write_32(&entry.name.ext.str_pos, REALLOC_COUNT(str_table));
	    REALLOC_APPEND(str_table, sec->name, name_len + 1);
	  }

	/* write section id and symbol class */
	out_obj->arch->write_16(&entry.section, out_sec->index);
	entry.class = COFF_SYMCLS_STATIC;

	if (fwrite(&entry, SIZEOF_COFF_SYM_S, 1, file) != 1)
	  goto write_symtable_err_free;

	/* set symbol index and increment */
	out_sec->symbol_id = j++;
      }

    /* walk thru symbol hash table */
    for (i = 0; i < OUT_SYMBOL_HASH_SIZE; i++)
      {
	struct symbol_s	*sym;

	for (sym = htable[i]; sym; sym = sym->out_sym->next)
	  {
	    unsigned int	name_len = strlen(sym->name);
	    unsigned int	sym_offset = 0;

	    memset (&entry, 0, sizeof (struct coff_sym_s));

	    /* update unresolved symbol counter */
	    if (sym->flag & SYMBOL_TYPE_EXTERN)
	      out_obj->sym_unresolved++;

	    /* update local symbol counter */
	    if (!(sym->flag & (SYMBOL_TYPE_EXTERN | SYMBOL_EXPORT)))
	      out_obj->sym_local++;

	    if (name_len <= COFF_SYM_SHRTNAM_SIZE)
	      /* write symbol short name */
	      strncpy(entry.name.short_name, sym->name, COFF_SYM_SHRTNAM_SIZE);
	    else
	      /* write symbol long name */
	      {
		entry.name.ext.is_longname = 0;
		out_obj->arch->write_32(&entry.name.ext.str_pos, REALLOC_COUNT(str_table));
		REALLOC_APPEND(str_table, sym->name, name_len + 1);
	      }

	    entry.aux = 0; /* do not set symbol aux field */

	    switch (sym->flag & SYMBOL_TYPE_MASK)
	      {
	      case (SYMBOL_TYPE_LABEL):
		sym_offset = sym->val.instr->next->offset;
		out_obj->arch->write_16(&entry.section, sym->section->out_sec->index);
		/* already set to zero
		   entry.type = COFF_SYMTYP_NULL
		*/
		entry.class = (sym->flag & SYMBOL_EXPORT) ? COFF_SYMCLS_EXTERN : COFF_SYMCLS_STATIC;
		break;

	      case (SYMBOL_TYPE_PROC):
		sym_offset = sym->val.proc->scope.first_instr.offset;
		out_obj->arch->write_16(&entry.section, sym->section->out_sec->index);
		out_obj->arch->write_16(&entry.type, COFF_SYMTYP_FUNC);
		entry.class = (sym->flag & SYMBOL_EXPORT) ? COFF_SYMCLS_EXTERN : COFF_SYMCLS_STATIC;
		break;

	      case (SYMBOL_TYPE_EXTERN):
		entry.class = COFF_SYMCLS_EXTERN;
		/* already set to zero
		entry.section = COFF_SYMSEC_UNDEF;
		entry.type = COFF_SYMTYP_NULL;
		entry.value = 0;
		*/
		break;
	      }

	    sym->out_sym->offset = sym_offset;
	    out_obj->arch->write_32(&entry.value, sym_offset);

	    if (fwrite(&entry, SIZEOF_COFF_SYM_S, 1, file) != 1)
	      goto write_symtable_err_free;

	    /* set symbol index and increment */
	    sym->out_sym->out_id = j++;
	  }
      }

    out_obj->sym_count = j;

    out_obj->arch->write_32(REALLOC_POOL(str_table), REALLOC_COUNT(str_table));
    if (fwrite(REALLOC_POOL(str_table), REALLOC_COUNT(str_table), 1, file) != 1)
      goto write_symtable_err_free;

    REALLOC_FREE(str_table);
    return (0);

  write_symtable_err_free:

    REALLOC_FREE(str_table);
    return (RETVAL_ERR_IO);
  }
}

struct out_symbol_s	*
out_coff_newsymbol	(struct object_s		*obj,
			 struct scope_s			*sc,
			 struct symbol_s		*sym)
{
  if (sym->flag & (SYMBOL_TYPE_LABEL | SYMBOL_TYPE_PROC | SYMBOL_TYPE_EXTERN))
    {
      struct out_symbol_s	*out_sym = mem_pop(&obj->out_obj->sym_pool);

      memset(out_sym, 0, sizeof (struct out_symbol_s));
      return (out_sym);
    }

  return (NULL);
}


