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
#include "aasm/symbol.h"

static int		expr_unroll(struct expr_s	*expr)
{
  if ((expr->left->flag & EXPR_TYPE_OPERATION)
      && (expr->val.op == expr->left->val.op))
    {
      /*
      **   +
      **  / \
      ** +
      */

      if (expr->right->flag & EXPR_TYPE_NUM)
	{
	  /*
	  **   +
	  **  / \
	  ** +   v
	  */

	  if (expr->left->right->flag & EXPR_TYPE_NUM)
	    {
	      /*
	      **     +          +
	      **    / \        / \
	      **   +   v  >>  +   o
	      **  / \        / \
	      ** o   v      v   v
	      */

	      struct expr_s	*tmp;

	      tmp = expr->right;
	      expr->right = expr->left->left;
	      expr->left->left = tmp;
	    }
	  else
	    if (expr->left->left->flag & EXPR_TYPE_NUM)
	      {
		/*
		**     +          +
		**    / \        / \
		**   +   v  >>  +   o
		**  / \        / \
		** v   o      v   v
		*/

		struct expr_s	*tmp;

		tmp = expr->right;
		expr->right = expr->left->right;
		expr->left->right = tmp;
	      }
	  
	  return (expr->left->val.op->eval(&expr->left));
	}
      else
	{
	  /*
	  **   +
	  **  / \
	  ** +   o
	  */

	  if ((expr->right->flag & EXPR_TYPE_OPERATION)
	      && (expr->val.op == expr->right->val.op))
	    {
	      /*
	      **         +                +
	      **       /   \             / \
	      **      +     +    >>     +   a
	      **     / \   / \         / \
	      **   ...  b c   d      ...  b
	      **   /                 / \
	      **  a                 c   d
	      */

		struct expr_s	*tmp, *tmp2;

		tmp = expr->left;
		while ((tmp->left->flag & EXPR_TYPE_OPERATION)
		       && (expr->val.op == tmp->left->val.op))
		  tmp = tmp->left;

		tmp2 = expr->right;
		expr->right = tmp->left;
		tmp->left = tmp2;

		return (expr_unroll(expr->left)
			|| expr_unroll(expr));
	    }
	  else
	    if ((expr->left->right->flag
		 & expr->left->left->flag
		 & EXPR_TYPE_NUM))
	      {
		/*
		**     +          +
		**    / \        / \
		**   +   o  >>  v   o
		**  / \ 
		** v   v
		*/

		return (expr->right->val.op->eval(&expr->right));
	      }
	    else
	      if (expr->left->left->flag & EXPR_TYPE_NUM)
		{
		  /*
		  **     +          +
		  **    / \        / \
		  **   +   o  >>  +   v
		  **  / \        / \
		  ** v   o      o   o
		  */
		  
		  struct expr_s	*tmp;
		  
		  tmp = expr->right;
		  expr->right = expr->left->left;
		  expr->left->left = tmp;
		}
	      else
		if (expr->left->right->flag & EXPR_TYPE_NUM)
		  {
		    /*
		    **     +          +
		    **    / \        / \
		    **   +   o  >>  +   v
		    **  / \        / \
		    ** o   v      o   o
		    */
		      
		    struct expr_s	*tmp;
		    
		    tmp = expr->right;
		    expr->right = expr->left->right;
		    expr->left->right = tmp;
		  }
	}
    }
  else
    if ((expr->right->flag & EXPR_TYPE_OPERATION)
	&& (expr->val.op == expr->right->val.op))
      {
	/*
	**   +
	**  / \
	**     +
	*/
	
	if (expr->left->flag & EXPR_TYPE_NUM)
	  {
	    /*
	    **   +
	    **  / \
	    ** v   +
	    */
	    
	    if (expr->right->left->flag & EXPR_TYPE_NUM)
	      {
		/*
		**   +           +
		**  / \         / \
		** v   +   >>  o   +
		**    / \         / \
		**   v   o       v   v
		*/
		
		struct expr_s	*tmp;
		
		tmp = expr->left;
		expr->left = expr->right->right;
		expr->right->right = tmp;
	      }
	    else
	      if (expr->right->right->flag & EXPR_TYPE_NUM)
		{
		  /*
		  **   +           +
		  **  / \         / \
		  ** v   +   >>  o   +
		  **    / \         / \
		  **   o   v       v   v
		  */
		  
		  struct expr_s	*tmp;
		  
		  tmp = expr->left;
		  expr->left = expr->right->left;
		  expr->right->left = tmp;
		}
	    
	    return (expr->right->val.op->eval(&expr->right));
	  }
	else
	  {
	    /*
	    **   +
	    **  / \
	    ** o   +
	    */
	    
	    if ((expr->right->right->flag
		 & expr->right->left->flag
		 & EXPR_TYPE_NUM))
	      {
		/*
		**   +           +
		**  / \         / \
		** o   +   >>  o   v
		**    / \
		**   v   v
		*/
		
		return (expr->right->val.op->eval(&expr->right));
	      }
	    else
	      if (expr->right->right->flag & EXPR_TYPE_NUM)
		{
		  /*
		  **   +           +
		  **  / \         / \
		  ** o   +   >>  v   +
		  **    / \         / \
		  **   o   v       o   o
		  */
		  
		  struct expr_s	*tmp;
		  
		  tmp = expr->left;
		  expr->left = expr->right->right;
		  expr->right->right = tmp;
		}
	      else
		if (expr->right->left->flag & EXPR_TYPE_NUM)
		  {
		    /*
		    **   +           +
		    **  / \         / \
		    ** o   +   >>  v   +
		    **    / \         / \
		    **   v   o       o   o
		    */
		    
		    struct expr_s	*tmp;
		    
		    tmp = expr->left;
		    expr->left = expr->right->left;
		    expr->right->left = tmp;
		  }
	  }
      }

  return (0);
}

int
expr_split_params	(struct expr_s	*expr,
			 struct expr_s	**params,
			 unsigned int	max)
{
  if (max <= 0)
    return (-1);

  if ((expr->flag & EXPR_TYPE_OPERATION)
      && (expr->val.op == &oper_b_separ))
    {
      int		rcount, lcount;

      if ((lcount = expr_split_params(expr->left, params, max)) < 0)
	return (-1);
      if ((rcount = expr_split_params(expr->right, params + lcount, max - lcount)) < 0)
	return (-1);

      return (rcount + lcount);
    }
  else
    {
      params[0] = expr;
      return (1);
    }
}

static void
expr_param_replace	(struct expr_s		**expr,
			 struct expr_s		**params,
			 unsigned int		count,
			 struct expr_s		*cur)
{
  struct expr_s	*e = *expr;

  switch (e->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):
      if (e->val.op->flag & OPER_BINARY)
	expr_param_replace(&e->left, params, count, cur);
      expr_param_replace(&e->right, params, count, cur);
      
      break;

    case (EXPR_TYPE_PARAM):
      {
	unsigned int	param = e->val.param;

	if (param >= count)
	  {
	    error_submsg(error(ERRTYP_ERROR,
			       "more parameters excepected for symbol `%s'",
			       cur->location),
			 cur->val.sym->name);
	    return;
	  }

	expr_node_free(e);
	*expr = expr_dup(params[param]);
      }
    }
}

int
expr_reduce		(struct expr_s	**expr)
{
  struct expr_s	*cur = *expr;

  /*
    expr_disp(*expr, NULL);
    printf("\n");
  */

  switch (cur->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):
      /* reduce node sub-trees */
      
      if ((cur->left && expr_reduce(&cur->left))
	  || expr_reduce(&cur->right))
	return (-1);
      
      if ((cur->val.op->flag & OPER_COMMUT)
	  && expr_unroll(cur))
	return (-1);

      /* eval node */
      return (cur->val.op->eval(expr));

    case (EXPR_TYPE_SYMBOL):

      switch (cur->val.sym->flag & SYMBOL_TYPE_MASK)
	{
	case (SYMBOL_TYPE_EXPR):
	  /* check recursive symbol use */
	  if (cur->val.sym->flag & SYMBOL_RECURS)
	    {
	      error_submsg(error(ERRTYP_ERROR,
				 "recursive use of symbol `%s'",
				 &cur->val.sym->def_location),
			   cur->val.sym->name);
	      return (-1);
	    }
	  cur->val.sym->flag |= SYMBOL_RECURS;

	  /* duplicate expression tree before reduce */
	  *expr = expr_dup_loc(cur->val.sym->val.expr, cur->location);

	  if (cur->flag & EXPR_HAS_PARAMS)
	    {
	      struct expr_s	*params[EXPR_PARAM_MAX];
	      int		count;

	      if ((count = expr_split_params(EXPR_PARAMS(cur),
					     params, EXPR_PARAM_MAX)) < 0)
		{
		  error_submsg(error(ERRTYP_ERROR,
				     "too many parameters for `%s' symbol",
				     cur->location),
			       cur->val.sym->name);
		  return (-1);
		}

	      expr_param_replace(expr, params, (unsigned)count, cur);
	    }

	  /* reduce expression tree */
	  if (expr_reduce(expr))
	    {
	      cur->val.sym->flag ^= SYMBOL_RECURS;
	      return (-1);
	    }

	  cur->val.sym->flag ^= SYMBOL_RECURS;
	  break;

	case (SYMBOL_TYPE_MACRO):
	  error_submsg(error(ERRTYP_ERROR,
			     "use of macro symbol `%s' in expression",
			     cur->location), cur->val.sym->name);
	  return (-1);

	default:
	  if (cur->flag & EXPR_HAS_PARAMS)
	    {
	      error_submsg(error(ERRTYP_ERROR,
				 "non expression symbol `%s' used with parameters",
				 cur->location), cur->val.sym->name);
	      return (-1);
	    }

	  return (0);
	}

    case (EXPR_TYPE_NUM):

    default:
      return (0);

    case (EXPR_TYPE_PARAM):
      error(ERRTYP_ERROR,
	    "unsubstitued expression parameter (symbol need more parameters ?)",
	    cur->location);
      return (-1);
    }
}

void		expr_macro_replace(struct expr_s	**expr,
				   struct expr_s	**params)
{
  struct expr_s	*cur = *expr;

  switch (cur->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):

      if (cur->val.op->flag & OPER_BINARY)
	expr_macro_replace(&cur->left, params);
      expr_macro_replace(&cur->right, params);

      break;

    case (EXPR_TYPE_SYMBOL):
      if (cur->val.sym->flag & SYMBOL_TYPE_PARAM)
	{
	  *expr = expr_dup(params[cur->val.sym->val.number]);
	  expr_node_free(cur);
	  break;
	}
      /*
      if (cur->flag & EXPR_HAS_PARAMS)
	expr_macro_replace(&EXPR_PARAMS(cur), params);
      */
      break;

    default:
      break;
    }
}

#ifdef UNUSED

int		expr_is_num_reduce(const struct expr_s	*expr)
{
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_OPERATION):
      /* operator may not allow eval (mem access) */
      if (expr->val.op->flag & OPER_NO_NUM_EVAL)
	return (0);

      /* recursiv call on operator */
      if (expr->val.op->flag & OPER_BINARY)
	return (expr_is_num_reduce(expr->right)
		&& expr_is_num_reduce(expr->left));
      else
	return (expr_is_num_reduce(expr->right));

      /* register is not evaluable */
    case (EXPR_TYPE_REGISTER):
      return (0);

      /* num value is num value :) */
    case (EXPR_TYPE_NUM):
      return (1);

    case (EXPR_TYPE_SYMBOL):

      switch (expr->val.sym->flag & SYMBOL_TYPE_MASK)
	{
	case (SYMBOL_TYPE_PROC):
	case (SYMBOL_TYPE_LABEL):
	  return (1);

	case (SYMBOL_TYPE_EXPR):
	  if (!(expr->val.sym->flag & SYMBOL_DEFINED))
	    return (0);
	  return (expr_is_num_reduce(expr->val.sym->val.expr));
	}
    }
  return (0);
}

#endif

