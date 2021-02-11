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

#ifndef EXPR_H_
# define EXPR_H_

#include <stdio.h>

#include "num.h"

/*
** operator
*/

struct				expr_s;
struct				section_s;
struct				symbol_s;
struct				register_s;
struct				error_locat_s;
struct				object_s;

typedef int			(expr_eval_t)(struct expr_s	**node);

# define OPER_BINARY		0x100
# define OPER_UNARY		0x200
# define OPER_BRACE_OPEN	0x400
# define OPER_BRACE_CLOSE	0x800
# define OPER_BRACE		(OPER_BRACE_OPEN | OPER_BRACE_CLOSE)

# define OPER_COMMUT		0x1000
# define OPER_NO_NUM_EVAL	0x2000 /* operator can't be reduced with num value */
# define OPER_SIZE_OPERATOR	0x4000
# define OPER_RIGHT_ASSOCIAT	0x8000

struct				operator_s
{
  expr_eval_t			*eval;
  unsigned int			flag;
  unsigned int			priority;
  const struct operator_s	*c_brace; /* link to closing brace operator */
  const char			*token;
  unsigned int			size; /* size mask used for size operators */
};

extern const struct operator_s	oper_u_size_byte;
extern const struct operator_s	oper_u_size_word;
extern const struct operator_s	oper_u_size_dword;
extern const struct operator_s	oper_u_size_fword;
extern const struct operator_s	oper_u_size_qword;
extern const struct operator_s	oper_u_size_tword;
extern const struct operator_s	oper_u_size_oword;

extern const struct operator_s	oper_u_oparen;
extern const struct operator_s	oper_u_obrack;
extern const struct operator_s	oper_b_cparen;
extern const struct operator_s	oper_b_cbrack;
extern const struct operator_s	oper_b_separ;

extern const struct operator_s	oper_u_plus;
extern const struct operator_s	oper_u_neg;

extern const struct operator_s	oper_u_not;

extern const struct operator_s	oper_b_eq;

extern const struct operator_s	oper_b_add;
extern const struct operator_s	oper_b_sub;
extern const struct operator_s	oper_b_mul;
extern const struct operator_s	oper_b_div;
extern const struct operator_s	oper_b_mod;
extern const struct operator_s	oper_b_pow;

extern const struct operator_s	oper_b_and;
extern const struct operator_s	oper_b_or;
extern const struct operator_s	oper_b_xor;

extern const struct operator_s	oper_b_shl;
extern const struct operator_s	oper_b_shr;

/*
** expresition
*/

# define EXPR_TYPE_NUM		0x1
# define EXPR_TYPE_SYMBOL	0x2
# define EXPR_TYPE_REGISTER	0x4
# define EXPR_TYPE_OPERATION	0x8
# define EXPR_TYPE_SECTION	0x10
# define EXPR_TYPE_PARAM	0x20

# define EXPR_TYPE_MASK		0x3f

# define EXPR_HAS_PARAMS	0x100

struct				expr_s
{
  unsigned int			flag;
  unsigned int			size; /* size flag */

  union
  {
    unsigned int		param;
    struct num_value_s		num;
    struct symbol_s		*sym;
    const struct register_s	*reg;
    const struct operator_s	*op;
    const struct section_s	*sec;
    void			*_void;
  }				val;

  struct error_locat_s		*location;

  struct expr_s			*left; /* used as param experssion with SYMBOLS */
  struct expr_s			*right;
  struct expr_s			*up; /* only used during expression build */
};

# define EXPR_PARAMS(e)		((e)->left)
# define EXPR_PARAM_MAX		20

void
expr_fout		(const struct expr_s	*e,
			 FILE			*out,
			 const struct object_s	*obj);

void
expr_fout_sec		(const struct expr_s	*e,
			 FILE			*out,
			 const struct section_s	*sec,
			 const struct object_s	*obj);

struct expr_s		*
expr_new		(void);

void
expr_free		(struct expr_s		*expr);

void
expr_node_free		(struct expr_s		*expr);

int
expr_split_params	(struct expr_s		*expr,
			 struct expr_s		**params,
			 unsigned int		max);

int
expr_reduce		(struct expr_s		**expr);

void
expr_macro_replace	(struct expr_s		**expr,
			 struct expr_s		**params);

struct expr_s		*
expr_num_get_zero	(void);

struct expr_s		*
expr_dup		(const struct expr_s	*expr);

struct expr_s		*
expr_dup_loc		(const struct expr_s	*expr,
			 struct error_locat_s	*loc);

struct exprs_s		*
expr_newref		(struct expr_s		*expr);



int
expr_cmp		(const struct expr_s	*a,
			 const struct expr_s	*b);

void
expr_clean		(void);

void
expr_init		(void);

#endif
