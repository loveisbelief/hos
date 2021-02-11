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
#include "aasm/symbol.h"
#include "aasm/macro.h"
#include "aasm/object.h"

#include "scan.h"
#include "parse.h"
#include "directive.h"

void		directive_macro(const char		*param,
			       struct object_s		*obj)
{
  char				name[MAX_LINE_LEN];
  const char			*param_list;
  struct symbol_s		*sym;
  struct macro_s		*macro;
  unsigned int			param_count = 0;

  if (!scan_store(param, &param_list, "#%i#", name))
    {
      error(ERRTYP_ERROR, "invalid use of .macro directive", 0);
      return;
    }

  if (obj->cur_macro)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "previous macro `%s' not ended",
			 0), obj->cur_macro->sym->name);
      macro_end_current(obj, in_parser_get_location());
    }

  /* create symbol */
  sym = symbol_creat(name, obj->cur_scope);

  /* creat macro */
  macro = macro_new(obj->cur_scope, sym);

  /* define symbol value */
  symbol_define(sym, macro, SYMBOL_TYPE_MACRO, obj->cur_scope, in_parser_get_location());

  /* set current macro */
  macro_set_current(obj, macro, in_parser_get_location());

  while (1)
    {
      struct symbol_s	*param_sym;
      char		scan_buffer[MAX_LINE_LEN + 1];
      const char	*end, *param_name;

      if (!*(param_list = scan_skip(param_list, automata_spacecomment)))
	break;

      if (!*(param_name = scan_get_end(param_list, automata_idf, &end, scan_buffer)))
	{
	  error(ERRTYP_ERROR, "invalid use of .macro directive", 0);
	  break;
	}
      
      param_sym = symbol_creat(param_name, obj->cur_scope);

      symbol_define(param_sym, NULL, SYMBOL_TYPE_PARAM, obj->cur_scope, in_parser_get_location());
      param_sym->val.number = param_count;

      param_count++;

      if (param_count >= MAX_OPERAND_COUNT)
	{
	  error(ERRTYP_ERROR, "parse: too many parameters", 0);
	  break;
	}

      param_list = end;

    }

  macro->param_count = param_count;

}

void		directive_macro_end(const char		*param,
				  struct object_s	*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".endm: extra character on line", 0);
      return;
    }  

  if (!obj->cur_macro)
    {
      error(ERRTYP_ERROR, "no macro to end", 0);
      return;
    }

  macro_end_current(obj, in_parser_get_location());
}

