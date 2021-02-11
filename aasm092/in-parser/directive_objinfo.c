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
#include <stdlib.h>

#include "aasm/error.h"
#include "aasm/object.h"
#include "aasm/expr.h"

#include "scan.h"
#include "directive.h"
#include "build_expr.h"
#include "parse.h"

/*
** object informations
*/

void		directive_objname(const char		*param,
				 struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%w#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .obj_name directive", 0);
      return;
    }
  
  object_set_name(obj, str);
  return;
}

void		directive_objdesc(const char		*param,
				 struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%l#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .obj_desc directive", 0);
      return;
    }

  object_set_desc(obj, str);
  return;
}

void		directive_objcopyright(const char	*param,
				      struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%l#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .obj_copyright directive", 0);
      return;
    }

  object_set_copyright(obj, str);
  return;
}

void		directive_objbase(const char		*param,
				 struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  struct expr_s		*expr;
  const char		*end;

  if (!scan_store(param, &end, "#%o#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .obj_base directive", 0);
      return;
    }

  if ((expr = build_expr(str, obj, in_parser_get_location())))
    {
      unsigned int	val;

      if (expr_reduce(&expr))
	goto directive_objbase_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &val))
	{
	  error(ERRTYP_ERROR, "can't deduce base value from expression", 0);
	  goto directive_objbase_freeexpr;
	}

      object_set_base(obj, val);

    directive_objbase_freeexpr:
      expr_free(expr);
    }
}

void		directive_debug(const char		*param,
			       struct object_s		*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".debug: extra character on line", 0);
      return;
    }  

  if (obj->flag & OBJECT_DEBUG)
    error(ERRTYP_WARN_MORE, "object debug flag already set", 0);

  obj->flag |= OBJECT_DEBUG;

  return;
}

