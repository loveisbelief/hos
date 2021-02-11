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

#include "aasm/reloc.h"
#include "aasm/expr.h"
#include "aasm/symbol.h"
#include "aasm/proc.h"
#include "aasm/scope.h"

#include "asm.h"
#include "flags.h"

/*#define __DEBUG_ 1*/

static const unsigned int	intel_endian[AASM_MAX_VAL_SIZE] =
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static int	asm_reloc_reduce(RELOC_REDUCE_PARAMS)
{
  struct expr_s				*expr = *reloc->expr;

  /* relativ relocation with symbol */
  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_SYMBOL):
      if (reloc->flag & RELOC_TYPE_RELATIV)
	{
	  struct instr_s		*lbl_instr;

	  switch (expr->val.sym->flag & SYMBOL_TYPE_MASK)
	    {
	    case (SYMBOL_TYPE_LABEL):
	      lbl_instr = expr->val.sym->val.instr;
	      break;

	    case (SYMBOL_TYPE_PROC):
	      lbl_instr = &expr->val.sym->val.proc->scope.first_instr;
	      break;

	    default:
	    case (SYMBOL_TYPE_EXPR):
	    case (SYMBOL_TYPE_EXTERN):
	      goto asm_reloc_reduce_addend;

	    }

	  if (sec != expr->val.sym->section)
	    {
	      error(ERRTYP_WARN, "x86: relative relocation to other section",
		    reloc->instr->location);
	      goto asm_reloc_reduce_addend;
	    }

	  if (lbl_instr->flag & INSTR_ADDR)
	    {
	      struct num_value_s	label;
	      struct num_value_s	target;
	      
	      num_init_uint(&label, lbl_instr->next->offset);
	      num_init_uint(&target, reloc->instr->offset + reloc->instr->len);
	      
	      num_sub(&label, &target, &target);
	      target.flag |= NUM_SIGNED;

	      /* add num size warning */
	      if ((reloc->instr->info->i_flag & I_FLAG_SHORT_IMEDIAT)
		  && (num_get_byte_size(&(*reloc->expr)->val.num) <= 1)
		  && (reloc->bits_size > 8))
		error_submsg(error(ERRTYP_WARN_OPT, "x86: byte value could be used instead of %s value",
				   reloc->instr->location), size_name[reloc->bits_size >> 3]);

	      if (num_write(&target, reloc->bits_size >> 3,
			    reloc->instr->val.bin + reloc->offset,
			    intel_endian, reloc->instr->location))
		break;
	      
#ifdef __DEBUG_
	      printf(" X86 LABEL RELATIV RELOC RESOLVED\n");
#endif	      
	      return (-1);
	    }
	}

    asm_reloc_reduce_addend:

    case (EXPR_TYPE_SECTION):
      if ((reloc->flag & (RELOC_ADDEND | RELOC_ADDEND_DIRECT_OUT | RELOC_ADDEND_VALUE))
	  == (RELOC_ADDEND | RELOC_ADDEND_DIRECT_OUT | RELOC_ADDEND_VALUE))
	{
	  struct num_value_s	tmp;
	  
	  /* write relocation addend directly in instruction */
	  num_init_int(&tmp, reloc->addend);
	  num_write(&tmp, reloc->bits_size >> 3,
		    reloc->instr->val.bin + reloc->offset,
		    intel_endian, reloc->instr->location);
	  reloc->flag ^= RELOC_ADDEND_VALUE;

#ifdef __DEBUG_
	  printf(" X86 DIRECT VALUE ADDEND WRITE\n");
#endif	      
	}
      break;

    case (EXPR_TYPE_NUM):
      /* relocation with num value */

      if (reloc->flag & RELOC_TYPE_RELATIV)
	error(ERRTYP_WARN, "x86: numerical value used for relativ target instruction",
	      reloc->instr->location);

      /* add num size warning */
      if ((reloc->instr->info->i_flag & I_FLAG_SHORT_IMEDIAT)
	  && (num_get_byte_size(&(*reloc->expr)->val.num) <= 1)
	  && (reloc->bits_size > 8))
	    error_submsg(error(ERRTYP_WARN_OPT, "x86: byte value could be used instead of %s value",
			       reloc->instr->location), size_name[reloc->bits_size >> 3]);

      if (num_write(&(*reloc->expr)->val.num,
		    reloc->bits_size >> 3,
		    reloc->instr->val.bin + reloc->offset,
		    intel_endian, reloc->instr->location))
	break;

#ifdef __DEBUG_
      printf(" X86 NUM RELOC RESOLVED\n");
#endif

      return (-1);
    }

  return (0);
}

static int	asm_reloc_reduce_fact(RELOC_REDUCE_PARAMS)
{
  struct expr_s				*expr = *reloc->expr;

  if (expr->flag & EXPR_TYPE_NUM)
    {
      unsigned int			val;

      if (num_get_uint(&expr->val.num, &val))
	error(ERRTYP_ERROR, "x86: invalid scale factor in address", expr->location);
      else
	switch (val)
	  {
	  case (1):
	    /* instr->val.bin[offset] |= OP_FIELD_SIB_SCAL1; */
	    return (-1);
	    
	  case (2):
	    reloc->instr->val.bin[reloc->offset] |= OP_FIELD_SIB_SCAL2;
	    return (-1);
	    
	  case (4):
	    reloc->instr->val.bin[reloc->offset] |= OP_FIELD_SIB_SCAL4;
	    return (-1);
	    
	  case (8):
	    reloc->instr->val.bin[reloc->offset] |= OP_FIELD_SIB_SCAL8;
	    return (-1);
	    
	  default:
	    error(ERRTYP_ERROR, "x86: invalid scale factor in address", expr->location);
	  }
    }

  return (0);
}

void		asm_x86_set_factor(struct instr_s	*instr,
				   unsigned int		offset,
				   struct section_s	*sec,
				   struct expr_s	*expr)
{
  if (expr->flag & EXPR_TYPE_NUM)
    {
      unsigned int			val;

      if (num_get_uint(&expr->val.num, &val))
	error(ERRTYP_ERROR, "x86: invalid scale factor in address", expr->location);
      else
	switch (val)
	  {
	  case (1):
	    /* instr->val.bin[offset] |= OP_FIELD_SIB_SCAL1; */
	    break;
	    
	  case (2):
	    instr->val.bin[offset] |= OP_FIELD_SIB_SCAL2;
	    break;
	    
	  case (4):
	    instr->val.bin[offset] |= OP_FIELD_SIB_SCAL4;
	    break;
	    
	  case (8):
	    instr->val.bin[offset] |= OP_FIELD_SIB_SCAL8;
	    break;
	    
	  default:
	    error(ERRTYP_ERROR, "x86: invalid scale factor in address", expr->location);
	    break;
	  }
    }
  else
    {
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = 2;
      reloc->bits_shift = 6;
      reloc->expr_ = expr;
      reloc->expr = &reloc->expr_;
      reloc->instr = instr;
      reloc->offset = offset;
      reloc->reduce = &asm_reloc_reduce_fact;
      reloc->flag = 0;
    }
}

unsigned int	asm_x86_set_imm_iexpr(struct instr_s	*instr,
				      unsigned int	offset,
				      unsigned int	size,
				      struct section_s	*sec,
				      struct expr_s	**expr)
{
  unsigned int		byte_size = size_flag2bytes[size & RELOC_SIZE_MASK];

  if ((*expr)->flag & EXPR_TYPE_NUM)
    {
      /* add num size warning */
      if ((instr->info->i_flag & I_FLAG_SHORT_IMEDIAT)
	  && (num_get_byte_size(&(*expr)->val.num) <= 1)
	  && (byte_size > 1))
	error_submsg(error(ERRTYP_WARN_OPT, "x86: byte value could be used instead of %s value",
			   instr->location), size_name[byte_size]);

      num_write(&(*expr)->val.num, byte_size,
		instr->val.bin + offset, intel_endian,
		instr->location);
    }
  else
    { 
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = byte_size * 8;
      reloc->bits_shift = 0;
      reloc->expr = expr;
      reloc->instr = instr;
      reloc->offset = offset;
      reloc->reduce = &asm_reloc_reduce;
      reloc->flag = 0;
    }

  return (byte_size);
}

unsigned int		asm_x86_set_imm_riexpr(struct instr_s	*instr,
					       unsigned int	offset,
					       unsigned int	size,
					       struct section_s	*sec,
					       struct expr_s	**expr)
{
  unsigned int		byte_size = size_flag2bytes[size & RELOC_SIZE_MASK];

  if ((*expr)->flag & EXPR_TYPE_NUM)
    {
      error(ERRTYP_WARN_MORE, "x86: numerical value used for relativ target instruction", instr->location);

      /* add num size warning */
      if ((instr->info->i_flag & I_FLAG_SHORT_IMEDIAT)
	  && (num_get_byte_size(&(*expr)->val.num) <= 1)
	  && (byte_size > 1))
	error_submsg(error(ERRTYP_WARN_OPT, "x86: byte value could be used instead of %s value",
			   instr->location), size_name[byte_size]);
      
      num_write(&(*expr)->val.num, byte_size,
		instr->val.bin + offset, intel_endian,
		instr->location);
    }
  else
    { 
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = byte_size * 8;
      reloc->bits_shift = 0;
      reloc->expr = expr;
      reloc->instr = instr;
      reloc->offset = offset;
      reloc->reduce = &asm_reloc_reduce;
      reloc->addend = -(byte_size);
      reloc->flag = RELOC_ADDEND | RELOC_TYPE_RELATIV | RELOC_ADDEND_VALUE;
    }

  return (byte_size);
}

/* used in asm_mem.c */
unsigned int	asm_x86_set_imm_expr(struct instr_s	*instr,
				     unsigned int	offset,
				     unsigned int	size,
				     struct section_s	*sec,
				     struct expr_s	*expr)
{
  unsigned int		byte_size = size_flag2bytes[size & RELOC_SIZE_MASK];

  if (expr->flag & EXPR_TYPE_NUM)
    {
      num_write(&expr->val.num, byte_size,
		instr->val.bin + offset, intel_endian,
		instr->location);
    }
  else
    {
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = byte_size * 8;
      reloc->bits_shift = 0;
      reloc->expr_ = expr;
      reloc->expr = &reloc->expr_;
      reloc->instr = instr;
      reloc->offset = offset;
      reloc->reduce = &asm_reloc_reduce;
      reloc->flag = 0;
    }

  return (byte_size);
}

const unsigned int	*asm_x86_endian_permut(unsigned int	byte_size)
{
  return (intel_endian);
}

