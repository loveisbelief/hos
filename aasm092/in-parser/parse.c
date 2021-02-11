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

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/disp.h"
#include "aasm/alloc.h"
#include "aasm/modules.h"
#include "aasm/object.h"
#include "aasm/expr.h"

#include "build_expr.h"
#include "parse.h"
#include "scan.h"
#include "directive.h"

static unsigned int		parse_include_depth_g = 0;
static FILE			*parse_file_g[MAX_INCLUDE_DEPTH];
static struct error_locat_s	parse_location_g[MAX_INCLUDE_DEPTH];

unsigned int			parse_ifdef_depth_g = 0;
unsigned int			parse_ifdef_g[MAX_IFDEF_DEPTH];

unsigned int			parser_state_g;

int		parse_open(const char	*filename)
{
  FILE		*file;
  
  if (!parse_include_depth_g && !strcmp(filename, "-"))
    {
      parse_location_g[0].filename = xstrdup("stdin");
      parse_location_g[0].pos.line = 0;
      file = stdin;
    }
  else
    {
      if (parse_include_depth_g >= MAX_INCLUDE_DEPTH)
	{
	  struct error_s	*err;
	  
	  err = error(ERRTYP_ERROR, 
		      "can't open `%s' max include depth reached", 0);
	  error_submsg(err, filename);
	  return (-1);
	}

      parse_location_g[parse_include_depth_g].filename = xstrdup(filename);
      parse_location_g[parse_include_depth_g].pos.line = 0;

      if (!(file = fopen(filename, "rb")))
	{
	  struct error_s	*err;
	  
	  err = error(ERRTYP_ERROR, "can't open `%s', I/O error", 0);
	  error_submsg(err, filename);
	  return (-1);
	}
    }

  parse_file_g[parse_include_depth_g] = file;
  parse_include_depth_g++;
  parse_ifdef_g[parse_ifdef_depth_g = 0] = 1;
  parser_state_g = PARSER_ST_MAIN;
  
  return (0);
}

static int		parse_close(void)
{

  if ((parse_include_depth_g == 1)
      && parse_ifdef_depth_g)
    error(ERRTYP_ERROR, "missing .endif at end of file", 0);

  parse_include_depth_g--;
  fclose(parse_file_g[parse_include_depth_g]);
  /* free(parse_location_g[parse_include_depth_g].filename);
     name used in error location, can't be free'd */

  return (parse_include_depth_g);
}

static const char	*parse_readline(void)
{
  static char		buffer[MAX_LINE_LEN];
  char			*ret;

  ret = fgets(buffer, MAX_LINE_LEN, parse_file_g[parse_include_depth_g - 1]);
  parse_location_g[parse_include_depth_g - 1].pos.line++;
  return (ret);
}

/*
** module exported functions
*/

int			in_parser_open(const char	*filename)
{
  return (parse_open(filename));
}

void			in_parser_close(void)
{
  while (parse_include_depth_g)
    fclose(parse_file_g[--parse_include_depth_g]);
}

struct error_locat_s	*in_parser_get_location(void)
{
  static struct error_locat_s	empty = 
    {
      "",
      {0}
    };

  if (!parse_include_depth_g)
    return (dup_location(&empty));

  return (dup_location(&parse_location_g[parse_include_depth_g - 1]));
}

int		parse_operands(const char		*param,
			       struct expr_s		**operands,
			       struct object_s		*obj,
			       struct error_locat_s	*location)
{
  char			scan_buffer[MAX_LINE_LEN + 1];
  const char		*oper_str;
  int			count = 0;
  struct expr_s		*e;

  oper_str = scan_get(param, automata_operand, scan_buffer);

  /* no operand */
  if (!*(oper_str = scan_skip(oper_str, automata_spacecomment)))
    return (0);

  if (!(e = build_expr(oper_str, obj, location)))
    return (-1);

  if ((count = expr_split_params(e, operands,
				 MAX_OPERAND_COUNT)) < 0)
    {
      error(ERRTYP_ERROR, "parse: too many operands", 0);
      return (-1);
    }

  return (count);
}

static void		parse_macro_line(struct object_s	*obj,
					 const char		*name,
					 const char		*param)
{
  char			scan_buffer[MAX_LINE_LEN + 1];
  struct instr_s	*instr;

  if (!*name)
    {
      error(ERRTYP_ERROR, "invalid use of macro", 0);
      return;
    }

  if (!obj->cur_section && !obj->cur_macro)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "no section/macro defined before use of `%s' macro", 0),
		   scan_get(name, automata_word, scan_buffer));
    }

  {
    struct error_locat_s	*location = dup_location(&parse_location_g[parse_include_depth_g - 1]);
    struct symbol_s		*sym;
    struct expr_s		*operands[MAX_OPERAND_COUNT];
    int				op_count;

    /* parse and build operands */
    if ((op_count = parse_operands(param, operands, obj, location)) < 0)
      return;

    sym = symbol_use(name, obj->cur_scope, -1);

    instr = instr_new(obj->cur_section, obj->cur_scope, NULL, 0,
		      (unsigned)op_count);

    instr_set_source(instr, location, INSTR_TYPE_MACRO);
    instr->val.macro = sym;

    /* set operands */
    memcpy(instr->operands, operands, op_count * sizeof(struct expr_s *));
  }
}

static void		parse_instruction_line(struct object_s	*obj,
					       const char	*line,
					       unsigned int	len,
					       const char	*param)
{
  char			scan_buffer[MAX_LINE_LEN + 1];
  struct instr_perf_s	*iperf;
  struct instr_s	*instr;

  if (!obj->cur_section && !obj->cur_macro)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "no section/macro defined before `%s' instruction", 0),
		   scan_get(line, automata_word, scan_buffer));
    }

  /* get instruction gperf struct */
  if (!(iperf = obj->msc.cl.nm.asm_->call.asm_.get_instr_perf(line, len)))
    {
      error_submsg(error(ERRTYP_ERROR,
			 "invalid instruction `%s'", 0),
		   scan_get(line, automata_word, scan_buffer));
      return;
    }

  /* instruction operands */
  {
    struct error_locat_s	*location = dup_location(&parse_location_g[parse_include_depth_g - 1]);
    struct expr_s		*operands[MAX_OPERAND_COUNT];
    int				op_count;

    /* parse and build operands */
    if ((op_count = parse_operands(param, operands, obj, location)) < 0)
      return;

    /* create new instruction node */
    instr = instr_new(obj->cur_section, obj->cur_scope,
		      iperf->info, obj->msc.cl.nm.asm_->call.asm_.max_binary_size(iperf->info),
		      (unsigned)op_count);

    /* define new instruction location 
       and set instruction source state flag */
    instr_set_source(instr, location, INSTR_TYPE_OPCODE);

    /* set operands */
    memcpy(instr->operands, operands, op_count * sizeof(struct expr_s *));
  }
}

static void
parse_new_label		(const char		*name,
			 struct object_s	*obj)
{
  struct symbol_s		*sym;

  if (*scan_skip(name, automata_idf))
    {
      error_submsg(error(ERRTYP_ERROR,
			 "invalid label name `%s'", 0), name);
      return;
    }

  if (!obj->cur_section)
    {
      error_submsg(error(ERRTYP_ERROR,
			 "no section defined before `%s' label",
			 0), name);
      return;
    }

  /* create symbol */
  sym = symbol_creat(name, obj->cur_scope);

  /* link last instr to symbol label */
  if (obj->cur_scope->last_instr.prev->label)
    symbol_sibling_add(obj->cur_scope->last_instr.prev->label, sym);
  else
    obj->cur_scope->last_instr.prev->label = sym;

  if (!symbol_define(sym, obj->cur_scope->last_instr.prev,
		     SYMBOL_TYPE_LABEL, obj->cur_scope,
		     in_parser_get_location()))
    {
      /* call output module symbol creat function */
      if (obj->msc.cl.nm.out->call.out.newsymbol)
	sym->out_sym = obj->msc.cl.nm.out->call.out.newsymbol(obj, obj->cur_scope, sym);
    }
}

int		in_parser_process(struct object_s	*obj)
{
  const char				*line;

  disp_info(stdout, "parsing", parse_location_g[0].filename);
  obj->srcfilename = xstrdup(parse_location_g[0].filename);

  do
    {
      while ((line = parse_readline()))
	{
	  /* jump here if label on the same line has been processed */
	parser_line_resume:

	  /* check max error count limit 
	  if (error_count_g >= MAX_ERROR_COUNT)
	    {
	      error(ERRTYP_ERROR, "too many parse errors, aborting...", 0);
	      goto parser_exit;
	    }
*/
	  /* skip spaces */
	  line = scan_skip(line, automata_spacecomment);

	  /* ignore empty or comment lines */
	  if (!*line || (*line == COMMENT_CHAR))
	    continue;

	  /* directive line */
	  switch (*line)
	    {
	      char			scan_buffer[MAX_LINE_LEN + 1];
	      struct directive_s	*dir;
	      unsigned int		len;
	      const char		*param;

	    case (DIRECTIV_1ST_CHAR):

	      /* directive line */

	      /* get directive name len and params string */
	      len = scan_get_len(line, automata_word);
	      param = scan_skip(line + len, automata_spacecomment);

	      /* get directive */
	      if ((dir = in_directive_set(line + 1, len - 1)))
		{
		  /* ignore directive if parser is in the wrong state */
		  if (dir->state_mask & parser_state_g)
		    {
		      /* lock asm module from here if needed by directive */
		      if ((dir->state_mask & PARSER_NEED_ASMMOD)
			  && !(obj->msc.lock_flag & MODULE_LOCK(MODULE_CLASS_ASM)))
			{
			  if (module_lock(&obj->msc, MODULE_CLASS_ASM))
			    goto parser_exit;
			}

		      /* lock output module from here if needed by directive */
		      if ((dir->state_mask & PARSER_NEED_OUTMOD)
			  && !(obj->msc.lock_flag & MODULE_LOCK(MODULE_CLASS_OUT)))
			{
			  if (module_lock(&obj->msc, MODULE_CLASS_OUT))
			    goto parser_exit;

			  /* call output module symbol creat function */
			  if (obj->msc.cl.nm.out->call.out.newobject)
			    obj->out_obj = obj->msc.cl.nm.out->call.out.newobject(obj);
			}

		      /* call directive function */
		      dir->fct(param, obj);
		    }
		}
	      else
		error_submsg(error(ERRTYP_ERROR,
				   "unknown directive `%s'", 0),
			     scan_get(line, automata_word, scan_buffer));
	      break;

	    case (USE_MACRO_CHAR):

	      /* macro line */

	      scan_get_end(line + 1, automata_idf, &param, scan_buffer);

	      if (parser_state_g & PARSER_ST_MAIN)
		parse_macro_line(obj, scan_buffer, param);
	      break;

	    default:

	      /* instruction line */

	      /* get instruction name len and params string */
	      len = scan_get_len(line, automata_word);
	      param = scan_skip(line + len, automata_spacecomment);

	      if (!(parser_state_g & PARSER_ST_MAIN))
		break;

	      /* recognize `label:' form */
	      if ((len > 1) && (line[len - 1] == STD_LABEL_CHAR))
		{
		  char		label_name[MAX_LINE_LEN + 1];

		  /* extract label name */
		  memcpy(label_name, line, len - 1);
		  label_name[len - 1] = 0;

		  parse_new_label(label_name, obj);

		  /* continue line processing */
		  line += len;
		  goto parser_line_resume;
		}

	      if (module_lock(&obj->msc, MODULE_CLASS_ASM))
		goto parser_exit;

	      parse_instruction_line(obj, line, len, param);
	      break;

	    }

	  /* used as loop break label */
	  continue;
	parser_exit:
	  break;

	}
    }
  while (parse_close());

  return (0);
}

