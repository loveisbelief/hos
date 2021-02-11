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
#include <stdio.h>

#include "aasm/expr.h"
#include "aasm/symbol.h"
#include "aasm/section.h"
#include "aasm/object.h"
#include "aasm/modules.h"
#include "aasm/refcount.h"

static void		expr_fout_(const struct expr_s		*e,
				   FILE				*out,
				   unsigned int			up_priority,
				   const struct section_s	*sec,
				   const struct object_s	*obj)
{
  if ((e->size != SIZE_MASK)
      && !(e->flag & EXPR_TYPE_REGISTER))
    fprintf(out, "%s ",
	    size_name[size_flag2bytes[e->size & SIZE_MASK]]);
  
  switch (e->flag & EXPR_TYPE_MASK)
    {
      unsigned int	priority;

    case (EXPR_TYPE_NUM):
      num_fout(&e->val.num, out);
      break;

    case (EXPR_TYPE_SYMBOL):
      if (sec && e->val.sym->section
	  && (sec != e->val.sym->section))
	fprintf(out, "%s:", e->val.sym->section->name);
      fputs(e->val.sym->name, out);
      break;

    case (EXPR_TYPE_REGISTER):
      fputs(obj->msc.cl.nm.asm_->call.asm_.reg_name(e->val.reg), out);
      break;

    case (EXPR_TYPE_OPERATION):
      if (e->val.op->flag & OPER_BRACE)
	priority = 0;
      else
	priority = e->val.op->priority;

      if (e->val.op->priority < up_priority)
	fputc('(', out);

      if (e->val.op->flag & OPER_BINARY)
	{
	  expr_fout_(e->left, out, priority, sec, obj);
	  fprintf(out, " %s ", e->val.op->token);
	}
      else
	{
	  if (e->val.op->flag & OPER_SIZE_OPERATOR)
	    fprintf(out, "%s ", e->val.op->token);
	  else
	    fputs(e->val.op->token, out);
	}

      expr_fout_(e->right, out, priority, sec, obj);

      if (e->val.op->c_brace)
	fprintf(out, "%s", e->val.op->c_brace->token);
      else
	if (e->val.op->priority < up_priority)
	  fputc(')', out);
      break;

    case (EXPR_TYPE_SECTION):
      fputs(e->val.sec->name, out);
      break;

    case (EXPR_TYPE_PARAM):
      fprintf(out, "$%u", e->val.param);

    }
}

void			expr_fout_sec(const struct expr_s	*e,
				      FILE			*out,
				      const struct section_s	*sec,
				      const struct object_s	*obj)
{
  expr_fout_(e, out, 0, sec, obj);
}

void			expr_fout(const struct expr_s	*e,
				  FILE			*out,
				  const struct object_s	*obj)
{
  expr_fout_(e, out, 0, NULL, obj);
}

struct expr_s		*expr_num_get_zero(void)
{
  struct expr_s		*new = expr_new ();

  new->flag = EXPR_TYPE_NUM;
  new->flag = SIZE_MASK;

  return (new);
}

int		expr_cmp(const struct expr_s	*a,
			 const struct expr_s	*b)
{
  switch (a->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):
      /* compare operators */
      if ((b->flag & EXPR_TYPE_OPERATION)
	  && (a->val.op == b->val.op))
	{
	  if (a->val.op->flag & OPER_BINARY)
	    {
	      /* compare operator subtrees */
	      if ((expr_cmp(a->left, b->left)
		   && expr_cmp(a->right, b->right)))
		return (-1);

	      /* compare opeartor permuted subtrees
		 if operator is commutativ */
	      return ((a->val.op->flag & OPER_COMMUT)
		      && expr_cmp(a->left, b->right)
		      && expr_cmp(a->right, b->left));
	    }
	  else
	    return (expr_cmp(a->right, b->right));
	}
      return (0);

    case (EXPR_TYPE_REGISTER):
      /* compare register nodes */
      return ((b->flag & EXPR_TYPE_REGISTER)
	      && (a->val.reg == b->val.reg));

    case (EXPR_TYPE_NUM):
      /* compare numbers  */
      return ((b->flag & EXPR_TYPE_NUM)
	      && num_cmp(&a->val.num, &b->val.num));

    case (EXPR_TYPE_SYMBOL):
      /* compare symbols */
      return ((b->flag & EXPR_TYPE_SYMBOL)
	      && symbol_cmp(a->val.sym, b->val.sym));

    case (EXPR_TYPE_SECTION):
      /* compare symbols */
      return ((b->flag & EXPR_TYPE_SECTION)
	      && (a->val.sec == b->val.sec));

    case (EXPR_TYPE_PARAM):
      /* compare symbols */
      return ((b->flag & EXPR_TYPE_PARAM)
	      && (a->val.param == b->val.param));
    }

  return (0);
}

/* Memory management for expr */
static struct refcount_s	expr_pool_g;

#ifdef DEBUG

static void
expr_disp	(void	*data)

{
  const struct expr_s	*e = data;

  switch (e->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_NUM):
      num_disp(&e->val.num);
      break;

    case (EXPR_TYPE_SYMBOL):
      fputs(e->val.sym->name, stderr);
      break;

    case (EXPR_TYPE_REGISTER):
      fprintf(stderr, "reg");
      break;

    case (EXPR_TYPE_OPERATION):
      if (e->val.op->flag & OPER_BINARY)
	expr_disp(e->left);
      fputs(e->val.op->token, stderr);
      fputc('(', stderr);
      expr_disp(e->right);      
      fputc(')', stderr);
      break;

    case (EXPR_TYPE_PARAM):
      fprintf(stderr, "$%u", e->val.param);
    }
}

#endif

static const struct mem_desc_s	expr_mem_desc =
  {
    "expr",
#ifdef DEBUG
    &expr_disp,
#else
    NULL,
#endif
  };

void
expr_init (void)
{
  refcount_init (&expr_pool_g, sizeof (struct expr_s), &expr_mem_desc);
}

void
expr_clean (void)
{
  refcount_clean (&expr_pool_g);
}

void			expr_free(struct expr_s		*expr)
{
  if (expr->flag & EXPR_TYPE_OPERATION)
    {
      if ((expr->val.op->flag & OPER_BINARY)
	  && expr->left)
	expr_free(expr->left);
      if (expr->right)
	expr_free(expr->right);
    }

  refcount_push (expr, &expr_pool_g);
}

void			expr_node_free(struct expr_s	*expr)
{
  refcount_push (expr, &expr_pool_g);
}

struct expr_s		*expr_new()
{
  struct expr_s		*new = refcount_pop (&expr_pool_g);
  memset(new, 0, sizeof(struct expr_s));

  return new;
}

struct expr_s		*expr_dup(const struct expr_s	*expr)
{
  struct expr_s		*new = refcount_pop (&expr_pool_g);

  /* copy current node */
  memcpy (new, expr, sizeof (struct expr_s));

  /* copy left and right sub-trees */
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):

      if (expr->val.op->flag & OPER_BINARY)
	new->left = expr_dup(expr->left);
      new->right = expr_dup(expr->right);
      break;

    case (EXPR_TYPE_SYMBOL):
      if (expr->flag & EXPR_HAS_PARAMS)
	EXPR_PARAMS(new) = expr_dup(EXPR_PARAMS(expr));
      break;
    }

  return (new);
}

struct expr_s		*expr_dup_loc(const struct expr_s	*expr,
				      struct error_locat_s	*loc)
{
  struct expr_s		*new = refcount_pop (&expr_pool_g);

  /* copy current node */
  memcpy (new, expr, sizeof (struct expr_s));
  new->location = loc;

  /* copy left and right sub-trees */
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):

      if (expr->val.op->flag & OPER_BINARY)
	new->left = expr_dup_loc(expr->left, loc);
      new->right = expr_dup_loc(expr->right, loc);
      break;

    case (EXPR_TYPE_SYMBOL):
      if (expr->flag & EXPR_HAS_PARAMS)
	EXPR_PARAMS(new) = expr_dup_loc(EXPR_PARAMS(expr), loc);
      break;
    }

  return (new);
}

struct exprs_s		*expr_newref (struct expr_s	*expr)
{
  return refcount_inc (expr);
}
