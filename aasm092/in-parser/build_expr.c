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
#include <string.h>

#include "aasm/expr.h"
#include "aasm/error.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/object.h"
#include "aasm/modules.h"

#include "scan.h"
#include "parse.h"
#include "build_expr.h"
#include "operator.h"

static const struct operator_s		*oper_lst[2][256] =
  {
    /* unary operators */
    {
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */

      NULL, /* ! */ NULL, /* " */ NULL, /* # */
      NULL, /* $ */ NULL, /* % */ NULL, /* & */ NULL, /* ' */
      &oper_u_oparen, /* ( */     NULL, /* ) */ 
      NULL, /* * */               &oper_u_plus, /* + */
      NULL, /* , */               &oper_u_neg, /* - */
      NULL, /* . */               NULL, /* / */
      NULL, /* 0 */ NULL, /* 1 */ NULL, /* 2 */ NULL, /* 3 */
      NULL, /* 4 */ NULL, /* 5 */ NULL, /* 6 */ NULL, /* 7 */
      NULL, /* 8 */ NULL, /* 9 */ NULL, /* : */ NULL, /* ; */
      NULL, /* < */ NULL, /* = */ NULL, /* > */ NULL, /* ? */
      NULL, /* @ */ 

      NULL, /* A */ NULL, /* B */ NULL, /* C */ NULL, /* D */
      NULL, /* E */ NULL, /* F */ NULL, /* G */ NULL, /* H */
      NULL, /* I */ NULL, /* J */ NULL, /* K */ NULL, /* L */
      NULL, /* M */ NULL, /* N */ NULL, /* O */ NULL, /* P */
      NULL, /* Q */ NULL, /* R */ NULL, /* S */ NULL, /* T */
      NULL, /* U */ NULL, /* V */ NULL, /* W */ NULL, /* X */
      NULL, /* Y */ NULL, /* Z */

      &oper_u_obrack, /* [ */     NULL, /* \ */
      NULL, /* ] */ NULL, /* ^ */
      NULL, /* _ */ NULL, /* ` */

      NULL, /* a */ NULL, /* b */ NULL, /* c */ NULL, /* d */
      NULL, /* e */ NULL, /* f */ NULL, /* g */ NULL, /* h */
      NULL, /* i */ NULL, /* j */ NULL, /* k */ NULL, /* l */
      NULL, /* m */ NULL, /* n */ NULL, /* o */ NULL, /* p */
      NULL, /* q */ NULL, /* r */ NULL, /* s */ NULL, /* t */
      NULL, /* u */ NULL, /* v */ NULL, /* w */ NULL, /* x */
      NULL, /* y */ NULL, /* z */

      NULL, /* { */ NULL, /* | */ NULL, /* } */ &oper_u_not, /* ~ */
    },

    /* binary operators */
    {
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */ NULL, /*   */ NULL, /*   */ NULL, /*   */
      NULL, /*   */

      NULL, /* ! */ NULL, /* " */ NULL, /* # */
      NULL, /* $ */ &oper_b_mod, /* % */ &oper_b_and, /* & */ NULL, /* ' */
      NULL, /* ( */               &oper_b_cparen, /* ) */ 
      &oper_b_mul, /* * */        &oper_b_add, /* + */
      &oper_b_separ, /* , */      &oper_b_sub, /* - */
      NULL, /* . */               &oper_b_div, /* / */
      NULL, /* 0 */ NULL, /* 1 */ NULL, /* 2 */ NULL, /* 3 */
      NULL, /* 4 */ NULL, /* 5 */ NULL, /* 6 */ NULL, /* 7 */
      NULL, /* 8 */ NULL, /* 9 */ NULL, /* : */ NULL, /* ; */
      NULL, /* < */ &oper_b_eq, /* = */ NULL, /* > */ NULL, /* ? */
      NULL, /* @ */ 

      NULL, /* A */ NULL, /* B */ NULL, /* C */ NULL, /* D */
      NULL, /* E */ NULL, /* F */ NULL, /* G */ NULL, /* H */
      NULL, /* I */ NULL, /* J */ NULL, /* K */ NULL, /* L */
      NULL, /* M */ NULL, /* N */ NULL, /* O */ NULL, /* P */
      NULL, /* Q */ NULL, /* R */ NULL, /* S */ NULL, /* T */
      NULL, /* U */ NULL, /* V */ NULL, /* W */ NULL, /* X */
      NULL, /* Y */ NULL, /* Z */

      NULL, /* [ */ NULL, /* \ */ 
      &oper_b_cbrack, /* ] */     &oper_b_xor, /* ^ */
      NULL, /* _ */ NULL, /* ` */

      NULL, /* a */ NULL, /* b */ NULL, /* c */ NULL, /* d */
      NULL, /* e */ NULL, /* f */ NULL, /* g */ NULL, /* h */
      NULL, /* i */ NULL, /* j */ NULL, /* k */ NULL, /* l */
      NULL, /* m */ NULL, /* n */ NULL, /* o */ NULL, /* p */
      NULL, /* q */ NULL, /* r */ NULL, /* s */ NULL, /* t */
      NULL, /* u */ NULL, /* v */ NULL, /* w */ NULL, /* x */
      NULL, /* y */ NULL, /* z */

      NULL, /* { */ &oper_b_or, /* | */ NULL, /* } */ NULL, /* ~ */
    }
  };

static struct expr_s	*build_val(struct expr_s		**root,
				   struct expr_s		*cur,
				   struct error_locat_s		*loc)
{
  struct expr_s		*new = expr_new ();

  new->up = cur;
  new->location = loc;
  new->size = SIZE_UNDEF;

  if (cur)
    cur->right = new;
  else
    (*root = new)->up = 0;

  return (new);
}

static struct expr_s	*build_oper_binary(struct expr_s		**root,
					   struct expr_s		*cur,
					   const struct operator_s	*oper,
					   struct error_locat_s		*loc)
{
  struct expr_s		*new = expr_new ();

  new->flag = EXPR_TYPE_OPERATION;
  new->val.op = oper;
  new->location = loc;
  new->size = SIZE_UNDEF;

  /* add operator with priority order */
  if (oper->flag & OPER_RIGHT_ASSOCIAT)
    while (cur->up && cur->up->val.op->priority > oper->priority)
      cur = cur->up;
  else
    while (cur->up && cur->up->val.op->priority >= oper->priority)
      cur = cur->up;

  new->up = cur->up;
  new->left = cur;		/* current sub-tree become 
				   operator left sub-tree */
  if (!cur->up)
    *root = new;		/* set root if no parent */
  else
    cur->up->right = new;	/* set current sub-tree
				   parent right link */
  
  cur->up = new;	/* set current parent link
			   and new current */
  return (new);
}

static struct expr_s	*build_oper_unary(struct expr_s			**root,
					  struct expr_s			*cur,
					  const struct operator_s	*oper,
					  struct error_locat_s		*loc)
{
  struct expr_s		*new = expr_new ();

  new->flag = EXPR_TYPE_OPERATION;
  new->val.op = oper;
  new->location = loc;
  new->size = SIZE_UNDEF;

  if ((new->up = cur))
    cur->right = new; /* add unary operator to
			 sub-tree */
  else
    (*root = new)->up = NULL; /* add unary as root */

  return (new);
}

static struct expr_s		*build_expr_(const char			**str,
					     const struct operator_s	*c_brace,
					     struct object_s		*obj,
					     struct error_locat_s	*loc);

static struct expr_s	*build_oper_brace(struct expr_s			**root,
					  struct expr_s			*cur,
					  const struct operator_s	*oper,
					  const char			**str,
					  struct object_s		*obj,
					  struct error_locat_s		*loc)
{
  struct expr_s		*sub;

  if ((sub = build_expr_(str, oper->c_brace, obj, loc)))
    {
      if ((sub->up = cur))
	cur->right = sub; /* add sub-tree to tree */
      else
	(*root = sub)->up = NULL; /* attach sub-tree to root */

      return (sub);
    }

  return (NULL);
}

static struct expr_s		*build_expr_(const char			**str,
					     const struct operator_s	*c_brace,
					     struct object_s		*obj,
					     struct error_locat_s	*loc)
{
  struct expr_s			*root, *cur;

  cur = root = NULL;

  while (1)
    {
      const struct operator_s	*oper;
      char			scan_buffer[MAX_LINE_LEN + 1];

      /* end of string reached */
      if (!(*(*str = scan_skip(*str, automata_spacecomment))))
	{
	  if (c_brace)
	    {
	      error(ERRTYP_ERROR, "unexpected end of expression, missing bracket/parentheses", 0);
	      goto build_expr_err;
	    }
	  break;
	}

      /* unary operator or value */
      if ((!cur) || ((cur->flag & EXPR_TYPE_OPERATION) && !cur->right))
	{
	  struct symbol_s	*sym;
	  const char		*tmp;

	  /* unary operator */
	  if ((oper = oper_lst[0][(unsigned)**str]))
	    {
	      (*str)++;

	      if (oper->flag & OPER_UNARY)
		cur = build_oper_unary(&root, cur, oper, loc);

	      if (oper->flag & OPER_BRACE_OPEN)
		if (!(cur = build_oper_brace(&root, cur, oper, str, obj, loc)))
		  goto build_expr_err;

	      continue;
	    }

	  switch (**str)
	    {
	    case (SECTION_NAME_1ST_CHAR):
	      /* section name */

	      tmp = scan_get_end(*str, automata_section, str, scan_buffer);

	      {
		struct section_s		*sec = section_find(obj, tmp);

		if (**str == SECTION_USE_CHAR)
		  {
		    /* section name with symbol */
		    if (*(tmp = scan_get_end(*str + 1, automata_idf, str, scan_buffer))
			&& (sym = symbol_use(tmp, &sec->scope, 0)))
		      goto symbol_build_val;
		  }
		else
		  {
		    cur = build_val(&root, cur, loc);
		    cur->flag = EXPR_TYPE_SECTION;
		    cur->val.sec = sec;
		    continue;
		  }
	      }

	      /* invalid symbol */
	      error_submsg(error(ERRTYP_ERROR, "`%s' is not a valid symbol name", 0),
			   scan_get(*str, automata_word, scan_buffer));

	      goto build_expr_err;

	    case (REGISTER_1ST_CHAR):
	      /* %register */
	      if (*(tmp = scan_get_end(*str + 1, automata_idf, str, scan_buffer)))
		{
		  const struct register_s	*reg;
		  unsigned int			len = strlen(tmp);

		  if ((reg = obj->msc.cl.nm.asm_->call.asm_.get_reg(tmp, len)))
		    {
		      cur = build_val(&root, cur, loc);
		      cur->flag = EXPR_TYPE_REGISTER;
		      cur->val.reg = reg;
		      cur->size = obj->msc.cl.nm.asm_->call.asm_.reg_size(reg);
		      continue;
		    }
		}

	      /* invalid register */
	      error_submsg(error(ERRTYP_ERROR, "`%s' is not a valid register name", 0), tmp);

	      goto build_expr_err;

	    case (EXPR_PARAM_1ST_CHAR):
	      if (*(tmp = scan_get_end(*str + 1, automata_dec, str, scan_buffer)))
		{
		  cur = build_val(&root, cur, loc);
		  cur->flag = EXPR_TYPE_PARAM;
		  cur->val.param = (unsigned)atoi(tmp);
		  continue;
		}
	      /* invalid parameter */
	      error_submsg(error(ERRTYP_ERROR, "`$%s' is not a valid expression parameter", 0),
			   scan_get(*str, automata_word, scan_buffer));

	      goto build_expr_err;
	    }

	  /* symbol identifier string */
	  if (*(tmp = scan_get_end(*str, automata_idf, str, scan_buffer)))
	    {
	      const struct operator_perf_s	*opperf;
	      const struct register_s		*reg;
	      unsigned int			len = strlen(tmp);

	      /* unary operator */
	      if ((opperf = in_operator_set(tmp, len))
		  && (opperf->op->flag & OPER_UNARY))
		{
		  cur = build_oper_unary(&root, cur, opperf->op, loc);
		  continue;
		}

	      /* register */
	      if ((reg = obj->msc.cl.nm.asm_->call.asm_.get_reg(tmp, len)))
		{
		  cur = build_val(&root, cur, loc);
		  cur->flag = EXPR_TYPE_REGISTER;
		  cur->val.reg = reg;
		  cur->size = obj->msc.cl.nm.asm_->call.asm_.reg_size(reg);
		  continue;
		}

	      /* symbol */
	      if ((sym = symbol_use(tmp, obj->cur_scope, -1)))
		{
		symbol_build_val:
		  cur = build_val(&root, cur, loc);
		  cur->flag = EXPR_TYPE_SYMBOL;
		  cur->val.sym = sym;

		  *str = scan_skip(*str, automata_spacecomment);
		  if (**str == SYM_PARAM_CHAR)
		    {
		      /* symbol name with parameter */

		      (*str)++;
		      cur->left = build_expr_(str, &oper_b_cparen, obj, loc);
		      cur->flag |= EXPR_HAS_PARAMS;
		    }
		  continue;
		}
	      /* invalid register/identifier */
	      error_submsg(error(ERRTYP_ERROR,
				 "`%s' is not a valid register/symbol name", 0), tmp);

	      goto build_expr_err;
	    }

	  /* numerical value */
	  if (*(tmp = scan_get_end(*str, automata_num, str, scan_buffer)))
	    {
	      cur = build_val(&root, cur, loc);
	      cur->flag = EXPR_TYPE_NUM;

	      if (build_expr_num(tmp, &cur->val.num))
		goto build_expr_err;

	      continue;
	    }

	  /* invalid char */
	  error_submsg(error(ERRTYP_ERROR,
			     "unexpected token `%s' in expression, value/unary operator expected", 0),
		       scan_get(*str, automata_singlechar, scan_buffer));
	  goto build_expr_err;

	}
      /* binary operator and brace close */
      else
	{
	  const char		*tmp;

	  if ((oper = oper_lst[1][(unsigned)**str]))
	    {
	      /* brace close */
	      if (c_brace && (c_brace == oper))
		{
		  (*str)++;
		  break;
		}

	      /* binary operator */
	      if (oper->flag & OPER_BINARY)
		{
		  (*str)++;
		  cur = build_oper_binary(&root, cur, oper, loc);
		  continue;
		}
	    }

	  if (*(tmp = scan_get_end(*str, automata_idf, str, scan_buffer)))
	    {
	      unsigned int			len = strlen(tmp);
	      const struct operator_perf_s	*opperf;

	      /* binary operator */
	      if ((opperf = in_operator_set(tmp, len))
		  && (opperf->op->flag & OPER_BINARY))
		{
		  cur = build_oper_binary(&root, cur, opperf->op, loc);
		  continue;
		}

	      error_submsg(error(ERRTYP_ERROR,
				 "unexpected token `%s' in expression, binary operator expected", 0),
			   tmp);
	      goto build_expr_err;
	    }

	  /* invalid char */
	  error_submsg(error(ERRTYP_ERROR,
			     "unexpected token `%s' in expression, binary operator expected", 0),
		       scan_get(*str, automata_word, scan_buffer));
	  goto build_expr_err;
	}

    }

  if (!cur || ((cur->flag & EXPR_TYPE_OPERATION) && !cur->right))
    {
      error(ERRTYP_ERROR, "unexpected end of expression", 0);
      goto build_expr_err;
    }

  return (root);

 build_expr_err:

  if (root)
    expr_free(root);
  return (NULL);
}

struct expr_s			*build_expr(const char			*str,
					    struct object_s		*obj,
					    struct error_locat_s	*loc)
{
  return (build_expr_(&str, NULL, obj, loc));
}

