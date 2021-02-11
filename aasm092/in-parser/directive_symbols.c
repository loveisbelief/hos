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

#include "aasm/error.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/object.h"
#include "aasm/proc.h"

#include "scan.h"
#include "parse.h"
#include "directive.h"
#include "keyword.h"
#include "build_expr.h"

void		directive_define(const char		*param,
				struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  char			expr_str[MAX_LINE_LEN];
  struct expr_s		*expr;
  struct symbol_s	*sym;

  if (scan_store(param, NULL, "#%i#%l", sym_name, expr_str) != 2)
    {
      error(ERRTYP_ERROR, "invalid use of .define directive", 0);
      return;
    }

  sym = symbol_creat(sym_name, obj->cur_scope);

  if (!(expr = build_expr(expr_str, obj, &sym->def_location)))
    return;

  symbol_define(sym, expr, SYMBOL_TYPE_EXPR, obj->cur_scope, in_parser_get_location());

  return;
}

void		directive_label(const char		*param,
			       struct object_s		*obj)
{
  int				i;
  char				str[2][MAX_LINE_LEN];
  const char			*end;
  struct symbol_s		*sym;
  unsigned int			flag = 0;
  static const struct keyword_s	keyword[6] =
    {
      {"data", NULL, SYMBOL_LABEL_DATA},
      {"loop", NULL, SYMBOL_LABEL_LOOP},
      {"call", NULL, SYMBOL_LABEL_CALL},
      {"taken", NULL, SYMBOL_LABEL_TAKEN},
      {"nottaken", NULL, SYMBOL_LABEL_NOTTAKEN},
      {0, 0, 0}
    };

  i = scan_store(param, &end, "#%i#%w#", str[0], str[1]);
  if (!i)
    {
      error(ERRTYP_ERROR, "invalid label name", 0);
      return;
    }

  if (*end)
    {
      error(ERRTYP_ERROR, ".label directive with extra character on line", 0);
      return;
    }

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .label directive", 0);
      return;
    }

#ifdef LABEL_ALIAS
  /* check previous label at same point */
  if (obj->cur_scope->last_instr.prev->label)
    {
      if (i > 1)
	error_submsg(error(ERRTYP_WARN,
			   "label `%s' already defined at this point, new qualifier ignored.", 0),
		     obj->cur_scope->last_instr.prev->sym->name);

      symbol_alias(str[0], obj->cur_scope->last_instr.prev->label, obj->cur_scope);

      return;
    }
#endif

  if (i > 1)      /* label qualifier present */
    {
      const struct keyword_s	*kw;

      if (!(kw = keyword_find(keyword, str[1])))
	{
	  error_submsg(error(ERRTYP_ERROR, "invalid label qualifier `%s'", 0), str[1]);
	  return;
	}

      if ((kw->data_int & SYMBOL_LABEL_CODE_MASK)
	  && !(obj->cur_section->flag & SECTION_TYPE_CODE))
	error_submsg(error(ERRTYP_WARN_MORE,
			   "`%s' label qualifier in non code section",
			   0), str[1]);

      if ((kw->data_int & SYMBOL_LABEL_DATA)
	  && !(obj->cur_section->flag & (SECTION_TYPE_DATA | SECTION_TYPE_BSS)))
	error_submsg(error(ERRTYP_WARN_MORE,
			   "`%s' label qualifier in non data section",
			   0), str[1]);

      flag = kw->data_int;
    }

  /* create symbol */
  sym = symbol_creat(str[0], obj->cur_scope);

  /* link last instr to symbol label */
  if (obj->cur_scope->last_instr.prev->label)
    symbol_sibling_add(obj->cur_scope->last_instr.prev->label, sym);
  else
    obj->cur_scope->last_instr.prev->label = sym;

  if (!symbol_define(sym, obj->cur_scope->last_instr.prev,
		     SYMBOL_TYPE_LABEL | flag, obj->cur_scope,
		     in_parser_get_location()))
    {
      /* call output module symbol creat function */
      if (obj->msc.cl.nm.out->call.out.newsymbol)
	sym->out_sym = obj->msc.cl.nm.out->call.out.newsymbol(obj, obj->cur_scope, sym);
    }

  return;
}

void		directive_extern(const char		*param,
				struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  const char		*end;
  struct symbol_s	*sym;

  if ((scan_store(param, &end, "#%i#", sym_name) != 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .extern directive", 0);
      return;
    }

  sym = symbol_creat(sym_name, obj->cur_scope);
  if (!symbol_define(sym, NULL, SYMBOL_TYPE_EXTERN, obj->cur_scope, in_parser_get_location()))
    {
      /* call output module symbol creat function */
      if (obj->msc.cl.nm.out->call.out.newsymbol)
	sym->out_sym = obj->msc.cl.nm.out->call.out.newsymbol(obj, obj->cur_scope, sym);
    }
}

void		directive_export(const char		*param,
				struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  const char		*end;
  struct symbol_s	*sym;

  if ((scan_store(param, &end, "#%i#", sym_name) != 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .export directive", 0);
      return;
    }

  sym = symbol_use(sym_name, obj->cur_scope, -1);

  sym->flag |= SYMBOL_EXPORT;
}

