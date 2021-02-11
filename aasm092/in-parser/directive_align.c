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
#include "aasm/instr.h"
#include "aasm/object.h"
#include "aasm/expr.h"

#include "directive.h"
#include "scan.h"
#include "keyword.h"
#include "build_expr.h"
#include "parse.h"

void		directive_align(const char		*param,
			       struct object_s		*obj)
{
  const char			*end;
  char				str[2][MAX_LINE_LEN];
  struct expr_s			*expr;
  const struct keyword_s	*kw;
  static const struct keyword_s	keyword[3] =
    {
      {"offset", NULL, INSTR_ALIGN_OFFSET},
      {"boundary", NULL, INSTR_ALIGN_BOUNDARY},
      {0, 0, 0}
    };

  if (((scan_store(param, &end, "#%w#%o#", str[0], str[1]) != 2) || *end)
      || !(kw = keyword_find(keyword, str[0])))
    {
      error(ERRTYP_ERROR, "invalid use of .align directive", 0);
      return;
    }

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .align directive", 0);
      return;
    }
  
  if (obj->cur_scope->last_instr.prev->flag & (INSTR_ALIGN_OFFSET | INSTR_ALIGN_BOUNDARY))
    {
      error(ERRTYP_ERROR, "instruction alignment already set", 0);
      return;
    }

  if ((expr = build_expr(str[1], obj, in_parser_get_location())))
    {
      unsigned int		val;

      if (expr_reduce(&expr))
	goto directive_align_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &val))
	{
	  error(ERRTYP_ERROR, "can't deduce align value from expression", 0);
	  goto directive_align_freeexpr;
	}

      obj->cur_scope->last_instr.prev->align = val;
      obj->cur_scope->last_instr.prev->flag |= kw->data_int;

    directive_align_freeexpr:
      expr_free(expr);
    }
}

