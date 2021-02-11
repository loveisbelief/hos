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
#include <assert.h>

#include "aasm/symbol.h"
#include "aasm/error.h"
#include "aasm/expr.h"
#include "aasm/scope.h"
#include "aasm/object.h"
#include "aasm/memory.h"

static unsigned int
symbol_hash		(const char			*str)
{
  unsigned int	hash = 0;

  while (*str)
    hash = (unsigned)*str++ ^ (hash << 3) ^ hash;

  return (hash);
}

static struct symbol_s	*
symbol_get_prev		(const char			*name,
			 unsigned int			hash,
			 struct scope_s			*scope)
{
  struct symbol_s	*sym;

  /* find symbol in scope hash table */
  sym = scope->symbols[hash % scope->symbol_hash_size];
  while (sym)
    {
      if (sym->name		/* skip anonymous symbol */
	  && (hash == sym->hash) /* check hash */
	  && !strcmp(name, sym->name)) /* check name only if hash match */
	break;
      sym = sym->next;
    }

  /* following links if symbol is an alias */
  while (sym && (sym->flag & SYMBOL_TYPE_ALIAS))
    sym = sym->val.link;

  return (sym);
}

struct symbol_s		*
symbol_creat		(const char			*name,
			 struct scope_s			*scope)
{
  unsigned int		hash = symbol_hash(name);
  struct symbol_s	*sym;

  /* look for symbol in table */
  if (!(sym = symbol_get_prev(name, hash, scope)))
    sym = symbol_new(name, hash, scope);

  return (sym);
}

int
symbol_define		(struct symbol_s		*sym,
			 void				*val,
			 unsigned int			type,
			 struct scope_s			*scope,
			 const struct error_locat_s	*loc)
{
  struct scope_s		*up_scope;

  /* symbol already defined ? */
  if (sym->flag & SYMBOL_DEFINED)
    {
      struct error_s		*err;

      /* some symbol types may be defined more than once */
      if (sym->flag & type & SYMBOL_TYPE_REDEFMASK)
	return (1);

      err = error(ERRTYP_ERROR, "symbol `%s' already defined in current scope at %s", loc);
      error_submsg(err, sym->name);
      error_sublocation(err, &sym->def_location);

      return (1);
    }

  /* check definition in higher scopes */
  if ((error_mask_g & ERRTYP_WARN_MORE) /* skip useless test algorithm if no warning */
      && (up_scope = scope->parent))
    {
      struct symbol_s		*shadow;
      unsigned int		hash = symbol_hash(sym->name);

      /* walk thru higher scopes to find symbol with same name */
      while (!(shadow = symbol_get_prev(sym->name, hash, up_scope)) && up_scope->parent)
	up_scope = up_scope->parent;

      /* warning if same name found */
      /* but symbol if same type & redefinable */
      if (shadow && !(shadow->flag & type & SYMBOL_TYPE_REDEFMASK))
	error_submsg(error(ERRTYP_WARN_MORE, "symbol `%s' shadows higher scope definition", loc), sym->name);
    }

  /* set symbol define location */
  memcpy(&sym->def_location, loc, sizeof (struct error_locat_s));

  /* set symbol type and value */
  sym->flag |= type | SYMBOL_DEFINED;
  sym->val._void = val;
  sym->section = scope->section;

  return (0);
}

void
symbol_alias		(const char			*alias_name,
			 struct symbol_s		*sym,
			 struct scope_s			*scope)
{
  struct symbol_s	*alias;

  alias = symbol_new(alias_name, symbol_hash(alias_name), scope);

  alias->flag |= SYMBOL_TYPE_ALIAS;
  alias->val.link = sym;
}

struct symbol_s		*
symbol_use		(const char			*name,
			 struct scope_s			*scope,
			 int				depth)
{
  unsigned int		hash = symbol_hash(name);
  struct symbol_s	*sym;
  struct scope_s	*tmp_scope = scope;

  /* get symbol or creat it */
  while (!(sym = symbol_get_prev(name, hash, tmp_scope))
	 && tmp_scope->parent && depth--)
    tmp_scope = tmp_scope->parent;

  if (!sym)
    sym = symbol_new(name, hash, scope);

  /* set symbol first use location */
  if (!(sym->flag & SYMBOL_USED))
    memcpy(&sym->use_location, get_location(),
	   sizeof (struct error_locat_s));

  sym->flag |= SYMBOL_USED;

  return (sym);
}

struct symbol_s		*
symbol_isdef		(const char			*name,
			 struct scope_s			*scope)
{
  unsigned int		hash = symbol_hash(name);
  struct symbol_s	*sym;
  struct scope_s	*tmp_scope = scope;

  /* get symbol */
  while (!(sym = symbol_get_prev(name, hash, tmp_scope))
	 && tmp_scope->parent)
    tmp_scope = tmp_scope->parent;

  if (sym)
    {
      /* set symbol first use location */
      if (!(sym->flag & SYMBOL_USED))
	memcpy(&sym->use_location, get_location(),
	       sizeof (struct error_locat_s));
      
      sym->flag |= SYMBOL_USED;
    }

  return (sym);
}

void
symbol_sibling_add	(struct symbol_s		*grp_sym,
			 struct symbol_s		*sym)
{
  if (grp_sym->sibling)
    {
      sym->sibling = grp_sym->sibling;
      grp_sym->sibling = sym;
    }
  else
    {
      grp_sym->sibling = sym;
      sym->sibling = grp_sym;
    }
}

int
symbol_cmp		(const struct symbol_s		*a,
			 const struct symbol_s		*b)
{
  if (!(a->flag & b->flag & SYMBOL_DEFINED))
    return (0);

  switch (a->flag & SYMBOL_TYPE_MASK)
    {
    case (SYMBOL_TYPE_PROC):
      return ((b->flag & SYMBOL_TYPE_PROC)
	      && (a->val.proc == b->val.proc));

    case (SYMBOL_TYPE_LABEL):
      return ((b->flag & SYMBOL_TYPE_LABEL)
	      && (a->val.instr == b->val.instr));
	
    case (SYMBOL_TYPE_EXPR):
      return ((b->flag & SYMBOL_TYPE_EXPR)
	      && expr_cmp(a->val.expr, b->val.expr));

    case (SYMBOL_TYPE_EXTERN):
      return ((b->flag & SYMBOL_TYPE_EXTERN)
	      && !strcmp(a->name, b->name));
   
    }

  return (0);
}

/* Memory management for symbol */

static struct mem_s	symbol_pool_g;

static const struct mem_desc_s	symbol_mem_desc =
  {
    "symbol",
    NULL
  };

/* Initialisation and clean method */
void
symbol_init (void)
{
  mem_init (&symbol_pool_g, sizeof (struct symbol_s), &symbol_mem_desc);
}

void
symbol_clean (void)
{
  mem_clean (&symbol_pool_g);
}

struct symbol_s	*symbol_new (const char		*name,
			     unsigned int	hash,
			     struct scope_s	*scope)
{
  struct symbol_s	*new = mem_pop (&symbol_pool_g);
  memset (new, 0, sizeof(struct symbol_s));

  if (name)
    {
      /* set symbol hash value */
      new->hash = hash;
      /* set symbol name */
      new->name = xstrdup(name);
    }
  else
    new->hash = (unsigned long) new; /* random hash for anonymous symbol */

  /* add symbol to scope symbol table */
  new->next = scope->symbols[new->hash % scope->symbol_hash_size];
  scope->symbols[new->hash % scope->symbol_hash_size] = new;

  return (new);
}

void			symbol_free(struct symbol_s	*sym)
{
  if (sym->name)
    dup_free ((void*)sym->name);

  if (sym->flag & SYMBOL_DEFINED)
    {
      switch (sym->flag & SYMBOL_TYPE_MASK)
	{
	case (SYMBOL_TYPE_LABEL):
	case (SYMBOL_TYPE_PROC):
	  break;
	case (SYMBOL_TYPE_EXPR):
	  expr_free(sym->val.expr);
	  break;
	case (SYMBOL_TYPE_EXTERN):
	default:
	  break;
	}
    }
  /* Repush the symbol in the list of free symbol */
  mem_push (sym, &symbol_pool_g);
}

void			scope_free(struct scope_s	*scope)
{
  unsigned int		i;

  instrs_free (scope);

  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      struct symbol_s	*sym, *tmp;

      sym = scope->symbols[i];
      while (sym)
	{
	  tmp = sym->next;
	  symbol_free(sym);
	  sym = tmp;
	}
    }

  /* BORDEL C KOI CE FREE ??? */
  if (i)
    free (scope->symbols);
}
