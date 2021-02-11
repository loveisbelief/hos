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
#include "aasm/expr.h"
#include "aasm/section.h"
#include "aasm/object.h"

#include "parse.h"
#include "scan.h"
#include "directive.h"
#include "keyword.h"
#include "build_expr.h"

void		directive_section(const char		*param,
				 struct object_s	*obj)
{
  int				i;
  char				str[2][MAX_LINE_LEN];
  const char			*end;
  const struct keyword_s	*kw;
  static const struct keyword_s	keyword[6] =
    {
      {"code", ".text", SECTION_TYPE_CODE | SECTION_TYPE_READONLY},
      {"data", ".data", SECTION_TYPE_DATA},
      {"rodata", ".rodata", SECTION_TYPE_DATA | SECTION_TYPE_READONLY},
      {"wrcode", ".wrtext", SECTION_TYPE_CODE},
      {"bss", ".bss", SECTION_TYPE_BSS},
      {0, 0, 0}
    };

  i = scan_store(param, &end, "#%w#%S#", str[0], str[1]);
  if (*end || !i)
    {
      error(ERRTYP_ERROR, "invalid use of .section directive", 0);
      return;
    }

  if (obj->cur_section)
    {
      error_submsg(error(ERRTYP_ERROR, "previous section `%s' not ended",
			 0), obj->cur_section->name);
      section_end_current(obj, in_parser_get_location());
    }

  if ((kw = keyword_find(keyword, str[0])))
    {
      struct section_s		*sec;

      if (i == 1)
	{
	  sec = section_find(obj, kw->data_str);
	  error_submsg(error(ERRTYP_WARN_MORE, "implicit use of `%s' as section name", 0), kw->data_str);
	}
      else
	sec = section_find(obj, str[1]);

      if (section_define(obj, sec, kw->data_int, in_parser_get_location()))
	return;

      /* call output module new section function if available */
      if (obj->msc.cl.nm.out->call.out.newsection)
	sec->out_sec = obj->msc.cl.nm.out->call.out.newsection(obj, sec);
    }
  else
    error_submsg(error(ERRTYP_ERROR,
		       ".section: invalid section type `%s'", 0), str[0]);

  return;
}

void		directive_sectionbase(const char		*param,
				     struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  struct expr_s		*expr;
  const char		*end;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined here", 0);
      return;
    }

  if (!scan_store(param, &end, "#%o#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .section_base directive", 0);
      return;
    }

  if ((expr = build_expr(str, obj, in_parser_get_location())))
    {
      unsigned int	val;

      if (expr_reduce(&expr))
	goto directive_sectionbase_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &val))
	{
	  error(ERRTYP_ERROR, "can't deduce base value from expression", 0);
	  goto directive_sectionbase_freeexpr;
	}

      section_set_base(obj->cur_section, val);

    directive_sectionbase_freeexpr:
      expr_free(expr);
    }
}

void		directive_sectionalign(const char	*param,
				      struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  struct expr_s		*expr;
  const char		*end;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined here", 0);
      return;
    }

  if (!scan_store(param, &end, "#%o#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .section_align directive", 0);
      return;
    }

  if ((expr = build_expr(str, obj, in_parser_get_location())))
    {
      unsigned int	val;

      if (expr_reduce(&expr))
	goto directive_sectionalign_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &val))
	{
	  error(ERRTYP_ERROR, "can't deduce align value from expression", 0);
	  goto directive_sectionalign_freeexpr;
	}

      if ((val < 2) || (val & (val - 1)))
	error(ERRTYP_WARN_OPT, "section alignment isn't a power of 2", 0);

      section_set_align(obj->cur_section, val);

    directive_sectionalign_freeexpr:
      expr_free(expr);
    }
}

void		directive_section_end(const char		*param,
				     struct object_s	*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".ends: extra character on line", 0);
      return;
    }  

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section to end", 0);
      return;
    }

  section_end_current(obj, in_parser_get_location());

  return;
}
