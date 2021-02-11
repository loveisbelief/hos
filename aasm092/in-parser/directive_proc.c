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
#include "aasm/proc.h"
#include "aasm/object.h"
#include "aasm/macro.h"

#include "directive.h"
#include "scan.h"
#include "parse.h"

void		directive_proc(const char		*param,
			      struct object_s		*obj)
{
  char				str[MAX_LINE_LEN];
  const char			*end;
  struct symbol_s		*sym;
  struct proc_s			*proc;
  struct instr_s		*instr;

  if ((scan_store(param, &end, "#%i#", str) < 1) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .proc directive", 0);
      return;
    }

  if (obj->cur_proc)
    {
      error_submsg(error(ERRTYP_ERROR, "previous function `%s' not ended",
			 0), obj->cur_proc->sym->name);
      proc_end_current(obj, in_parser_get_location());
    }

  if (obj->cur_macro)
    {
      error_submsg(error(ERRTYP_ERROR, "can't define function inside `%s' macro",
			 0), obj->cur_macro->sym->name);
      return;
    }

  if (!obj->cur_section)
    {
      error_submsg(error(ERRTYP_ERROR, "no section defined before `%s' function",
			 0), str);
      return;
    }

  /* create symbol */
  sym = symbol_creat(str, obj->cur_scope);

  /* creat proc node */
  proc = proc_new(obj->cur_section, sym);

  /* define symbol value */
  if (!symbol_define(sym, proc, SYMBOL_TYPE_PROC, obj->cur_scope, in_parser_get_location()))
    {
      /* call output module symbol creat function */
      if (obj->msc.cl.nm.out->call.out.newsymbol)
	sym->out_sym = obj->msc.cl.nm.out->call.out.newsymbol(obj, obj->cur_scope, sym);
    }

  proc->instr = instr = instr_new(obj->cur_section, obj->cur_scope, NULL, 0, 0);
  instr_set_source(instr, dup_location (&sym->def_location), INSTR_TYPE_PROC);
  instr->val.proc = proc;

  /* set current proc */
  proc_set_current(obj, proc, in_parser_get_location());
}

void		directive_proc_end(const char		*param,
				  struct object_s	*obj)
{
  if (*scan_skip(param, automata_spacecomment))
    {
      error(ERRTYP_ERROR, ".endp: extra character on line", 0);
      return;
    }  

  if (!obj->cur_proc)
    {
      error(ERRTYP_ERROR, "no function to end", 0);
      return;
    }

  proc_end_current(obj, in_parser_get_location());
}

