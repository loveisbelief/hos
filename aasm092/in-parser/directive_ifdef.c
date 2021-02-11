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

#include "aasm/symbol.h"
#include "aasm/object.h"
#include "aasm/expr.h"

#include "directive.h"
#include "scan.h"
#include "parse.h"
#include "build_expr.h"

void		directive_if(const char			*param,
			     struct object_s		*obj)
{
  char			expr_str[MAX_LINE_LEN];
  const char		*end;
  struct expr_s		*expr;

  if (parse_ifdef_depth_g >= MAX_IFDEF_DEPTH - 1)
    {
      error(ERRTYP_ERROR, ".if: max depth reached", 0);
      return;
    }

  if ((scan_store(param, &end, "#%o#", expr_str) != 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .ifdef directive", 0);
      return;
    }

  if ((expr = build_expr(expr_str, obj, in_parser_get_location())))
    {
      if (expr_reduce(&expr) || (!(expr->flag & EXPR_TYPE_NUM)))
	{
	  error(ERRTYP_ERROR, "can't deduce bytes count value from expression", 0);
	  goto directive_if_freeexpr;
	}

      parse_ifdef_depth_g++;

      if (num_cmp_uint(&expr->val.num, 0))
	parse_ifdef_g[parse_ifdef_depth_g] = 0;
      else
	parse_ifdef_g[parse_ifdef_depth_g] =
	  parse_ifdef_g[parse_ifdef_depth_g - 1];

      parser_state_g = parse_ifdef_g[parse_ifdef_depth_g]
	? PARSER_ST_MAIN : PARSER_ST_SKIP;

    directive_if_freeexpr:
      expr_free(expr);
    }
}

void		directive_ifdef(const char		*param,
			       struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  const char		*end;

  if (parse_ifdef_depth_g >= MAX_IFDEF_DEPTH - 1)
    {
      error(ERRTYP_ERROR, ".ifdef: max depth reached", 0);
      return;
    }

  if ((scan_store(param, &end, "#%i#", sym_name) != 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .ifdef directive", 0);
      return;
    }

  parse_ifdef_depth_g++;

  if (symbol_isdef(sym_name, obj->cur_scope))
    parse_ifdef_g[parse_ifdef_depth_g] =
      parse_ifdef_g[parse_ifdef_depth_g - 1];
  else
    parse_ifdef_g[parse_ifdef_depth_g] = 0;

  parser_state_g = parse_ifdef_g[parse_ifdef_depth_g]
    ? PARSER_ST_MAIN : PARSER_ST_SKIP;

  return;
}

void		directive_ifndef(const char		*param,
				struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  const char		*end;

  if (parse_ifdef_depth_g >= MAX_IFDEF_DEPTH - 1)
    {
      error(ERRTYP_ERROR, ".ifndef: max depth reached", 0);
      return;
    }

  if ((scan_store(param, &end, "#%i#", sym_name) != 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .ifndef directive", 0);
      return;
    }

  parse_ifdef_depth_g++;

  if (!symbol_isdef(sym_name, obj->cur_scope))
    parse_ifdef_g[parse_ifdef_depth_g] =
      parse_ifdef_g[parse_ifdef_depth_g - 1];
  else
    parse_ifdef_g[parse_ifdef_depth_g] = 0;

  parser_state_g = parse_ifdef_g[parse_ifdef_depth_g]
    ? PARSER_ST_MAIN : PARSER_ST_SKIP;

  return;
}

void		directive_else(const char		*param,
			      struct object_s		*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".else: extra character on line", 0);
      return;
    }  

  if (!parse_ifdef_depth_g)
    {
      error(ERRTYP_ERROR, ".else: no previous .ifdef", 0);
      return;
    }

  parse_ifdef_g[parse_ifdef_depth_g] ^= 1;

  parser_state_g = parse_ifdef_g[parse_ifdef_depth_g]
    ? PARSER_ST_MAIN : PARSER_ST_SKIP;

  return;
}

void		directive_endif(const char		*param,
			       struct object_s		*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".endif: extra character on line", 0);
      return;
    }  

  if (!parse_ifdef_depth_g)
    {
      error(ERRTYP_ERROR, ".endif: no previous .ifdef", 0);
      return;
    }

  parse_ifdef_depth_g--;

  parser_state_g = parse_ifdef_g[parse_ifdef_depth_g]
    ? PARSER_ST_MAIN : PARSER_ST_SKIP;

  return;
}

