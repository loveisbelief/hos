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

#include "aasm/expr.h"
#include "aasm/reloc.h"
#include "aasm/error.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/object.h"

#include "scan.h"
#include "parse.h"
#include "directive.h"
#include "build_expr.h"

void		directive_string(const char		*param,
				struct object_s		*obj)
{
  char			sym_name[MAX_LINE_LEN];
  unsigned char		str[MAX_LINE_LEN];
  unsigned int		sym_ok = 0;
  const char		*end;
  struct symbol_s	*sym;
  struct instr_s	*instr;
  unsigned int		len;

  param = scan_skip(param, automata_spacecomment);

  if (param[0] == '"')
    {
      if ((scan_store(param, &end, "%s#", str) < 1) || *end)
	goto directive_string_err;
    }
  else
    {
      if ((scan_store(param, &end, "%i#%s#", sym_name, str) < 2) || *end)
	goto directive_string_err;
      sym_ok = 1;
    }

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .string directive", 0);
      return;
    }

  /* escape string and get len */
  if (!(len = scan_string_escape(str + 1, str, '"')))
    {
      error(ERRTYP_ERROR, ".string directive with zero sized string", 0);
      return;
    }

  if (sym_ok)
    {
      /* create symbol */
      sym = symbol_creat(sym_name, obj->cur_scope);

      /* set label qualifier flag */
      sym->flag |= SYMBOL_LABEL_DATA;

      /* link last instr to symbol label */
      if (obj->cur_scope->last_instr.prev->label)
	symbol_sibling_add(obj->cur_scope->last_instr.prev->label, sym);
      else
	obj->cur_scope->last_instr.prev->label = sym;
      
      /* define symbol value */
      if (!symbol_define(sym, obj->cur_scope->last_instr.prev,
			 SYMBOL_TYPE_LABEL, obj->cur_scope, in_parser_get_location()))
	{
	  /* call output module symbol creat function */
	  if (obj->msc.cl.nm.out->call.out.newsymbol)
	    sym->out_sym = obj->msc.cl.nm.out->call.out.newsymbol(obj, obj->cur_scope, sym);
	}
    }

  /* creat instruction node */
  instr = instr_new(obj->cur_section, obj->cur_scope, NULL, len, 0);
  instr_set_source(instr, dup_location(in_parser_get_location()),
		   INSTR_TYPE_DATA);
  instr_set_asm(instr, len);

  /* set instruction binary string */
  memcpy(instr->val.bin, str, len);

  return;

 directive_string_err:
  error(ERRTYP_ERROR, "invalid use of .string directive", 0);
  return;
}

void		directive_fill(const char		*param,
			      struct object_s		*obj)
{
  const char		*end;
  char			str[2][MAX_LINE_LEN];
  unsigned int		val, len;
  struct instr_s	*instr;
  struct expr_s		*expr;

  if ((scan_store(param, &end, "#%n#%o#", str[0], str[1]) != 2) || *end)
    goto directive_fill_err;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .fill directive", 0);
      return;
    }

  if (((val = strtol(str[0], (void*)&end, 0)) > 0xff) || *end)
    goto directive_fill_err;

  if ((expr = build_expr(str[1], obj, in_parser_get_location())))
    {
      if (expr_reduce(&expr))
	goto directive_sectionbase_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &len)
	  || (!len) || (len > MAX_FILL_LEN))
	{
	  error(ERRTYP_ERROR, "can't deduce bytes count value from expression", 0);
	  goto directive_sectionbase_freeexpr;
	}

      /* creat instruction node */
      instr = instr_new(obj->cur_section, obj->cur_scope, NULL, len, 0);
      instr_set_source(instr, dup_location(in_parser_get_location()), INSTR_TYPE_DATA);
      instr_set_asm(instr, len);

      /* set instruction binary */
      memset(instr->val.bin, (int)val, len);

    directive_sectionbase_freeexpr:
      expr_free(expr);
    }

  return;

 directive_fill_err:
  error(ERRTYP_ERROR, "invalid use of .fill directive", 0);
  return;
}

void		directive_pad(const char			*param,
			     struct object_s		*obj)
{
  const char		*end;
  char			str[2][MAX_LINE_LEN];
  unsigned int		val, len;
  struct instr_s	*instr;
  struct expr_s		*expr;

  if ((scan_store(param, &end, "#%n#%o#", str[0], str[1]) != 2) || *end)
    goto directive_fill_err;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .pad directive", 0);
      return;
    }

  instr = obj->cur_scope->last_instr.prev;

  if (!(instr->flag & INSTR_TYPE_DATA) || !(instr->flag & INSTR_ASM))
    {
      error(ERRTYP_ERROR, ".pad directive must follow a data directive", 0);
      return;
    }

  if (((val = strtol(str[0], (void*)&end, 0)) > 0xff) || *end)
    goto directive_fill_err;

  if ((expr = build_expr(str[1], obj, in_parser_get_location())))
    {
      if (expr_reduce(&expr))
	goto directive_sectionbase_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &len)
	  || (!len) || (len > MAX_FILL_LEN))
	{
	  error(ERRTYP_ERROR, "can't deduce bytes count value from expression", 0);
	  goto directive_sectionbase_freeexpr;
	}

      if (len < instr->len)
	{
	  error(ERRTYP_ERROR, "previous data size greater than pad size", 0);
	  goto directive_sectionbase_freeexpr;
	}

      if (len == instr->len)
	{
	  error(ERRTYP_WARN_MORE, "no space to pad here", 0);
	  goto directive_sectionbase_freeexpr;
	}

      len -= instr->len;

      /* creat instruction node */
      instr = instr_new(obj->cur_section, obj->cur_scope, NULL, len, 0);
      instr_set_source(instr, dup_location(in_parser_get_location()),
		       INSTR_TYPE_DATA);
      instr_set_asm(instr, len);

      /* set instruction binary */
      memset(instr->val.bin, (int)val, len);

    directive_sectionbase_freeexpr:
      expr_free(expr);
    }

  return;

 directive_fill_err:
  error(ERRTYP_ERROR, "invalid use of .pad directive", 0);
  return;
}

void		directive_reserve(const char		*param,
				  struct object_s	*obj)
{
  const char		*end;
  char			str[MAX_LINE_LEN];
  unsigned int		len;
  struct instr_s	*instr;
  struct expr_s		*expr;

  if ((scan_store(param, &end, "#%o#", str) != 1) || *end)
    goto directive_reserve_err;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .reserve directive", 0);
      return;
    }

  if ((expr = build_expr(str, obj, in_parser_get_location())))
    {
      if (expr_reduce(&expr))
	goto directive_sectionbase_freeexpr;

      if (!(expr->flag & EXPR_TYPE_NUM) || num_get_uint(&expr->val.num, &len)
	  || (!len) || (len > MAX_RESERVE_LEN))
	{
	  error(ERRTYP_ERROR, "can't deduce bytes count value from expression", 0);
	  goto directive_sectionbase_freeexpr;
	}

      /* creat instruction node */
      instr = instr_new(obj->cur_section, obj->cur_scope, NULL, 0, 0);
      instr_set_source(instr, dup_location(in_parser_get_location()), INSTR_TYPE_UNINIT);
      instr_set_asm(instr, len);

    directive_sectionbase_freeexpr:
      expr_free(expr);
    }

  return;

 directive_reserve_err:
  error(ERRTYP_ERROR, "invalid use of .reserve directive", 0);
  return;
}

void		directive_db(const char			*param,
			    struct object_s		*obj)
{
  aasm_u8_t		bin[MAX_DB_LEN];
  struct instr_s	*instr;
  unsigned int		len = 0;
  
  while (1)
    {
      const char	*end;
      unsigned int	val;

      param = scan_skip(param, automata_spacecomment);

      if (len >= MAX_DB_LEN)
	goto directive_db_err;

      if ((val = strtoul(param, (char **)&end, 0)) > 0xff)
	goto directive_db_err;

      if (param == end)
	goto directive_db_err;

      bin[len++] = val;

      end = scan_skip(end, automata_spacecomment);

      if (!*end)
	break;

      if (*end != OPERAND_SEP_CHAR)
	goto directive_db_err;

      param = end + 1;
    }

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .db directive", 0);
      return;
    }

  /* creat instruction node */
  instr = instr_new(obj->cur_section, obj->cur_scope, NULL, len, 0);
  instr_set_source(instr, dup_location(in_parser_get_location()),
		   INSTR_TYPE_DATA);
  instr_set_asm(instr, len);

  /* set instruction binary string */
  memcpy(instr->val.bin, bin, len);

  return;

 directive_db_err:
  error(ERRTYP_ERROR, "invalid use of .db directive", 0);
  return;
}

static int	directive_dump_reloc_reduce(RELOC_REDUCE_PARAMS)
{
  struct expr_s		*expr = *reloc->expr;
  const unsigned int	*endian;

  endian = obj->msc.cl.nm.asm_->call.asm_.endian_permut(reloc->bits_size >> 3);

  if (expr->flag & EXPR_TYPE_NUM)
    {
      /* write value */
      return (!num_write(&expr->val.num, reloc->bits_size >> 3,
			 reloc->instr->val.bin, endian,
			 reloc->instr->location));
    }

  /* write relocation addend if any */
  if ((reloc->flag & (RELOC_ADDEND | RELOC_ADDEND_DIRECT_OUT | RELOC_ADDEND_VALUE))
      == (RELOC_ADDEND | RELOC_ADDEND_DIRECT_OUT | RELOC_ADDEND_VALUE))
    {
      struct num_value_s	tmp;

      /* write relocation addend directly in instruction */
      num_init_int(&tmp, reloc->addend);
      num_write(&tmp, reloc->bits_size >> 3,
		reloc->instr->val.bin + reloc->offset,
		endian, reloc->instr->location);
      reloc->flag ^= RELOC_ADDEND_VALUE;
    }

  return (0);
}

void		directive_dump(const char		*param,
			      struct object_s		*obj)
{
  struct expr_s		*operands[MAX_OPERAND_COUNT];
  int			op_count, i;
  struct error_locat_s	*location;

  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "no section defined before .dump directive", 0);
      return;
    }

  location = dup_location(in_parser_get_location());

  /* parse dump operands */
  if ((op_count = parse_operands(param, operands,
				 obj, location)) < 0)
    {
      free_location(location);
      return;
    }

  if (!op_count)
    {
      error(ERRTYP_ERROR, "invalid use of .dump directive", 0);
      return;
    }

  for (i = 0; i < op_count; i++)
    {
      struct instr_s	*instr;
      unsigned int	byte_size;
      struct expr_s	*e = operands[i];

      /* check size operator presence */
      if (!(((e->flag & EXPR_TYPE_OPERATION) && e->val.op->size)))
	error(ERRTYP_WARN_MORE, ".dump directive: no size operator on expression root", 0);

      /* reduce expression */
      if (expr_reduce(&e))
	continue;

      /* check expression size on root node */
      if (!e->size || ((e->size - 1) & e->size))
	{
	  error(ERRTYP_ERROR, "unable to determine expression size for .dump directive", 0);
	  continue;
	}

      byte_size = size_flag2bytes[e->size];

      /* creat data instruction in current scope */
      instr = instr_new(obj->cur_section, obj->cur_scope,
			NULL, byte_size, 0);
      instr_set_source(instr, dup_location(in_parser_get_location()),
		       INSTR_TYPE_DATA);

      if (e->flag & EXPR_TYPE_NUM)
	{
	  /* write direct value */
	  num_write(&e->val.num, byte_size,
		    instr->val.bin, obj->msc.cl.nm.asm_->call.asm_.endian_permut(byte_size),
		    instr->location);
	  expr_free(e);
	}
      else
	{
	  /* add relocation on value */
	  struct reloc_s	*reloc = reloc_add(obj->cur_section);

	  reloc->bits_size = byte_size << 3;
	  reloc->bits_shift = 0;
	  reloc->expr_ = e;
	  reloc->expr = &reloc->expr_;
	  reloc->instr = instr;
	  reloc->offset = 0;
	  reloc->reduce = &directive_dump_reloc_reduce;
	  reloc->flag = 0;
	}

      /* set instruction binary form */
      instr_set_asm(instr, byte_size);
    }
}

