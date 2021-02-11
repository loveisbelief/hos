/*
**    This file is part of AASM.
**
**    AASM is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
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
#include "aasm/instr.h"
#include "aasm/error.h"

#include "flags.h"
#include "asm_mem_table.h"
#include "asm.h"

static int		asm_mem_xchg_idxfact(struct expr_s	**tree,
					     struct expr_s	**xchg)
{
  struct expr_s			*cur = *tree;

  if ((cur->flag & EXPR_TYPE_OPERATION)
      && (cur->val.op == &oper_b_add))
    {
      /* ? + ? */
      if ((cur->left->flag & EXPR_TYPE_OPERATION)
	  && (cur->left->val.op == &oper_b_mul)
	  && ((cur->left->left->flag | cur->left->right->flag)
	      & EXPR_TYPE_REGISTER))
	{
	  /* (reg * ?) + ?*/

	  *xchg = cur->left;
	  *tree = cur->right;
	  expr_node_free(cur);
	  return (-1);
	}
      
      if ((cur->right->flag & EXPR_TYPE_OPERATION)
	  && (cur->right->val.op == &oper_b_mul)
	  && ((cur->right->left->flag | cur->right->right->flag)
	      & EXPR_TYPE_REGISTER))
	{
	  /* ? + (reg * ?) */

	  *xchg = cur->right;
	  *tree = cur->left;
	  expr_node_free(cur);
	  return (-1);
	}

      return (asm_mem_xchg_idxfact(&cur->right, xchg)
	  || asm_mem_xchg_idxfact(&cur->left, xchg));
    }

  return (0);
}

static int		asm_mem_xchg_base(struct expr_s	**tree,
					  struct expr_s	**xchg)
{
  struct expr_s			*cur = *tree;

  if ((cur->flag & EXPR_TYPE_OPERATION)
      && (cur->val.op == &oper_b_add))
    {
      /* ? + ? */
      if (cur->left->flag & EXPR_TYPE_REGISTER)
	{
	  /* reg + ? */
	  *xchg = cur->left;
	  *tree = cur->right;
	  expr_node_free(cur);
	  return (-1);
	}
      
      if (cur->right->flag & EXPR_TYPE_REGISTER)
	{
	  /* reg + ? */
	  *xchg = cur->right;
	  *tree = cur->left;
	  expr_node_free(cur);
	  return (-1);
	}
      
      return (asm_mem_xchg_base(&cur->right, xchg)
	      || asm_mem_xchg_base(&cur->left, xchg));
    }
  return (0);
}

static int		asm_mem_get_idx_fact(struct expr_s	*expr,
					     struct expr_s	**fact,
					     struct expr_s	**index)
{
  if (expr->left->flag & EXPR_TYPE_REGISTER)
    {
      *index = expr->left;
      *fact = expr->right;
      expr_node_free(expr);
      return (0);
    }

  if (expr->right->flag & EXPR_TYPE_REGISTER)
    {
      *index = expr->right;
      *fact = expr->left;
      expr_node_free(expr);
      return (0);
    }

  return (-1);
}

unsigned int		asm_x86_set_mem(struct instr_s		*instr,
				    unsigned int		offset,
				    struct expr_s		*expr_,
				    struct section_s		*sec)
{
  struct expr_s			*base = NULL;
  struct expr_s			*dis = NULL;
  struct expr_s			*index = NULL;
  struct expr_s			*factor = NULL;

  {
    struct expr_s		*expr = expr_dup(expr_->right);
    
    /*
    ** get base register , displacment,
    ** index register and scale factor
    ** from expression
    */
    
    switch (expr->flag & EXPR_TYPE_MASK)
      {
      case (EXPR_TYPE_REGISTER):
	/* [reg] */
	base = expr;
	break;
	
      case (EXPR_TYPE_OPERATION):
	if(expr->val.op == &oper_b_add)
	  {
	    struct expr_s		*tmp;
	    /* [? + ? + ? ...] */
	    
	    if (asm_mem_xchg_idxfact(&expr, &tmp))
	      {
		/* [ ? + (fact * reg) ]*/
		asm_mem_get_idx_fact(tmp, &factor, &index);
	      }
	    
	    /* ? */
	    
	    asm_mem_xchg_base(&expr, &base);
	    if (!index)
	      asm_mem_xchg_base(&expr, &index);

	    if (expr->flag & EXPR_TYPE_REGISTER)
	      /* reg */
	      {
		if (!base)
		  base = expr;
		else
		  if (!index)
		    index = expr;
		  else
		    {
		      expr_node_free(expr);
		      goto mem_error_invalid;
		    }
		break;
	      }
	    
	    /* dis */
	    dis = expr;
	    
	  }
	else
	  /* [reg * factor] or [factor * reg] */
	  if(expr->val.op == &oper_b_mul)
	    {
	      if (asm_mem_get_idx_fact(expr, &factor, &index))
		goto mem_error_invalid;
	    }
	  else
	    goto mem_error_invalid;
	
	break;
	
      /* [num] */
      default:
	dis = expr;
	
      }
  }

#ifdef __DEBUG_

  printf("BASE: ");
  if (base) 
    expr_disp(base);
  printf("   DIS: ");
  if (dis) 
    expr_disp(dis);
  printf("   INDEX: ");
  if (index) 
    expr_disp(index);
  printf("   FACT: ");
  if (factor) 
    expr_disp(factor);
  printf("\n");

#endif

  {
    const struct mem_table_s	*entry;
    unsigned int		dis_n = 0;
    unsigned int		base_n = 0;
    unsigned int		index_n = 0;

    if (base)
      {
	if (!(base->val.reg->mem & X86_MEM_BASE_PRESENT))
	  goto mem_error_badreg;
	  
	base_n = base->val.reg->mem;
      }

    if (index)
      {
	if (!(index->val.reg->mem & X86_MEM_INDEX_PRESENT))
	  goto mem_error_badreg;
	  
	index_n = index->val.reg->mem;
      }

    if (dis)
      {
	switch (dis->size)
	  {
	  case (SIZE_UNDEF):
	    if ((index_n | base_n) & X86_MEM_BASE_WORD)
	      goto asm_mem_assum_dis16;	/* assume word displacement if word reg */

	    if (index_n || base_n)
	      goto asm_mem_assum_dis32;	/* assume dword displacement if dword reg */

	    if (instr->asm_flag & ASM_SECTION_CODE16)
	      goto asm_mem_assum_dis16;	/* assume word displacement if code is 16 bits */

	  asm_mem_assum_dis32:
	    dis_n = X86_MEM_DIS_DWORD;
	    error(ERRTYP_WARN_MORE, "x86: address displacement size unknown, dword assumed", dis->location);
	    break;
	    
	  asm_mem_assum_dis16:
	    dis_n = X86_MEM_DIS_WORD;
	    error(ERRTYP_WARN_MORE, "x86: address displacement size unknown, word assumed", dis->location);
	    break;

	  case (SIZE_FLAG_BYTE):
	    dis_n = X86_MEM_DIS_BYTE;
	    break;
	    
	  case (SIZE_FLAG_WORD):
	    dis_n = X86_MEM_DIS_WORD;
	    break;
	    
	  case (SIZE_FLAG_DWORD):
	    dis_n = X86_MEM_DIS_DWORD;
	    break;
	    
	  default:
	    error(ERRTYP_ERROR, "x86: invalid address displacement size", dis->location);
	    return (offset);
	  }
      }

    entry = &mem_table[factor ? X86_MEM_FACTOR_PRESENT : 0][dis_n][base_n][index_n];

    /* repport error */

    switch (entry->flag & X86_MEM_FLAG_ERR_MASK)
      {
	struct error_s		*err;

      case (0):
	break;

      case (X86_MEM_ERR_16COMBIN):
	err = error(ERRTYP_ERROR, "x86: word register `%s' can't be used with `%s' in address", expr_->location);
	error_submsg(err, base->val.reg->name);
	error_submsg(err, index->val.reg->name);
	return (offset);

      case (X86_MEM_ERR_1632COMBIN):
	err = error(ERRTYP_ERROR, "x86: `%s' and `%s' address registers have different size", expr_->location);
	error_submsg(err, base->val.reg->name);
	error_submsg(err, index->val.reg->name);
	return (offset);

      case (X86_MEM_ERR_ESP_INDEX):
	error(ERRTYP_ERROR, "x86: `esp' used as index register in address", expr_->location);
	return (offset);

      case (X86_MEM_ERR_DISDWORD):
	err = error(ERRTYP_ERROR, "x86: dword displacement used with word address register `%s'", expr_->location);
	error_submsg(err, base ? base->val.reg->name : index->val.reg->name);
	return (offset);

      case (X86_MEM_ERR_DISWORD):
	err = error(ERRTYP_ERROR, "x86: word displacement used with dword address register `%s'", expr_->location);
	error_submsg(err, base ? base->val.reg->name : index->val.reg->name);
	return (offset);

      case (X86_MEM_ERR_16FACT):
	err = error(ERRTYP_ERROR, "x86: scale factor used with word address register `%s'", expr_->location);
	error_submsg(err, index->val.reg->name);
	return (offset);

      case (X86_MEM_ERR_INVDIS):
	error(ERRTYP_ERROR, "x86: invalid address displacement size", expr_->location);
	return (offset);

      default:
	error(ERRTYP_ERROR, "x86: unexpected address error", expr_->location);
	return (offset);

      };

    /* add address size prefix */
      if (((entry->flag & X86_MEM_FLAG_CODE16) && (!(instr->asm_flag & ASM_SECTION_CODE16)))
	  || (!(entry->flag & X86_MEM_FLAG_CODE16) && (instr->asm_flag & ASM_SECTION_CODE16)))
	{
	  unsigned int	bin_begin = instr->info->i_flag & I_PREBYTE_INSTR ? 1 : 0;
	  
	  memmove(instr->val.bin + 1 + bin_begin, instr->val.bin + bin_begin, offset + 1 - bin_begin);
	  instr->val.bin[bin_begin] = PREFIXE_INV_ASIZE;
	  offset++;
	}

    /* copy rm and sib bytes */
    instr->val.bin[offset++] |= entry->rm;
    
    if (entry->flag & X86_MEM_FLAG_SIB)
      {
	instr->val.bin[offset] = entry->sib;
	if (factor)
	  asm_x86_set_factor(instr, offset, sec, factor);
	offset++;
      }

    /* set displacement */
    switch (entry->flag & X86_MEM_FLAG_DIS_MASK)
      {
      case (0):
	break;

      case (X86_MEM_FLAG_DIS_BYTE):
	if (!dis)
	  dis = expr_num_get_zero();
	offset += asm_x86_set_imm_expr(instr, offset, SIZE_FLAG_BYTE, sec, dis);
	break;

      case (X86_MEM_FLAG_DIS_WORD):
	if (!dis)
	  dis = expr_num_get_zero();
	offset += asm_x86_set_imm_expr(instr, offset, SIZE_FLAG_WORD, sec, dis);
	break;

      case (X86_MEM_FLAG_DIS_DWORD):
	if (!dis)
	  dis = expr_num_get_zero();
	offset += asm_x86_set_imm_expr(instr, offset, SIZE_FLAG_DWORD, sec, dis);
	break;
      }
  }

  return (offset);

 mem_error_badreg:
  error(ERRTYP_ERROR, "x86: invalid register in address expression", expr_->location);
  return (offset);

 mem_error_invalid:
  error(ERRTYP_ERROR, "x86: invalid address expression", expr_->location);
  return (offset);
}
