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

#include "aasm/reloc.h"
#include "aasm/expr.h"
#include "aasm/symbol.h"
#include "aasm/proc.h"

#include "asm.h"
#include "fields.h"

const unsigned int	*
asm_sparc_endian_permut	(unsigned int			byte_size)
{
  /* big endian table */
  static const unsigned int	big_endian[AASM_MAX_VAL_SIZE] =
    {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

  return (&big_endian[AASM_MAX_VAL_SIZE - byte_size]);
}

static void
asm_opcode_or		(opcode_bin_t		opcode,
			 aasm_u8_t		*bin)
{
  bin[0] |= (opcode >> 24) & 0xff;
  bin[1] |= (opcode >> 16) & 0xff;
  bin[2] |= (opcode >> 8) & 0xff;
  bin[3] |= opcode & 0xff;
}

static opcode_bin_t
asm_reloc_wr_unsigned	(struct instr_s		*instr,
			 unsigned int		bit_size,
			 struct num_value_s	*num)
{
  unsigned int	value;
  int		size;

  size = num_get_bit_size(num, 1);

  if (size < 0)
    {
      error(ERRTYP_ERROR, "sparc: negative value can't fit in unsigned location",
	    instr->location);
      return (0);
    }

  if (size > bit_size)
    {
      struct error_s	*err;
      
      err = error(ERRTYP_ERROR, "sparc: %s bits value can't fit in %s bits unsigned location",
		  instr->location);
      error_subint(err, size);
      error_subint(err, (signed)bit_size);
      return (0);
    }

  num_get_uint(num, &value);

  return (value & ((1 << bit_size) - 1));
}

static opcode_bin_t
asm_reloc_wr_signed	(struct instr_s		*instr,
			 unsigned int		bit_size,
			 struct num_value_s	*num)
{
  int		size;
  int		value;

  size = num_get_bit_size(num, 0);

  if (size > bit_size)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR, "sparc: %s bits value can't fit in %s bits signed location",
		  instr->location);
      error_subint(err, size);
      error_subint(err, (signed)bit_size);
      return (0);
    }

  num_get_int(num, &value);

  return (value & ((1 << bit_size) - 1));
}

static int
asm_reloc_reduce_simm	(RELOC_REDUCE_PARAMS)
{
  struct expr_s		*expr = *reloc->expr;

  switch (expr->flag & EXPR_TYPE_MASK)
    {
      opcode_bin_t	res;

    case (EXPR_TYPE_NUM):
      if (reloc->flag & RELOC_SIGNED)
	res = asm_reloc_wr_signed(reloc->instr,
				  reloc->bits_size,
				  &expr->val.num);
      else
	res = asm_reloc_wr_unsigned(reloc->instr,
				    reloc->bits_size,
				    &expr->val.num);

      res <<= reloc->bits_shift;
      asm_opcode_or(res, reloc->instr->val.bin);

      return (-1);

    case (EXPR_TYPE_OPERATION):
      if (!(expr->right->flag & EXPR_TYPE_NUM))
	break;

      /* reduce relocation size if AND mask present */
      if (expr->val.op == &oper_b_and)
	{
	  unsigned int	val;

	  if (!(val = num_is_mask(&expr->right->val.num))
	      || (val > 64))
	    {
	      error(ERRTYP_WARN, "sparc: and value is not a valid relocation bit mask", expr->location);
	      break;
	    }

	  /* discard AND operation from reloc expr */
	  expr = *reloc->expr = expr->left;

	  /* update ralocation size */
	  if (reloc->bits_size > val)
	    reloc->bits_size = val;

	  /* break if next node is not an operation (not a shr) */
	  if (!(expr->flag & EXPR_TYPE_OPERATION))
	    break;
	}

      /* set relocation bits shift amount if SHR op present */
      if (expr->val.op == &oper_b_shr)
	{
	  unsigned int	val;

	  /* abort if already shifted relocation */
	  if (reloc->bits_shift)
	    {
	      error(ERRTYP_ERROR, "sparc: can't use shifted value for this relocation", expr->location);
	      break;
	    }

	  if (num_get_uint(&expr->right->val.num, &val)
	      || (val > 64))
	    {
	      error(ERRTYP_WARN, "sparc: shift value is not valid for relocation", expr->location);
	      break;
	    }

	  /* discard AND operation from reloc expr */
	  *reloc->expr = expr->left;

	  /* update relocation bit shift */
	  reloc->bits_shift = -val;
	}
    }

  return (0);
}

opcode_bin_t
asm_set_simm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr)
{
  if ((*expr)->flag & EXPR_TYPE_NUM)
    {
      return (asm_reloc_wr_signed(instr, bit_size,
				  &(*expr)->val.num)
	      << bit_shift);
    }
  else
    {
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = bit_size;
      reloc->bits_shift = bit_shift;
      reloc->expr = expr;
      reloc->instr = instr;
      reloc->offset = 0;
      reloc->reduce = &asm_reloc_reduce_simm;
      reloc->flag = RELOC_SIGNED;
    }

  return (0);
}

opcode_bin_t
asm_set_imm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr)
{
  if ((*expr)->flag & EXPR_TYPE_NUM)
    {
      return (asm_reloc_wr_unsigned(instr, bit_size,
				    &(*expr)->val.num)
	      << bit_shift);
    }
  else
    {
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = bit_size;
      reloc->bits_shift = bit_shift;
      reloc->expr = expr;
      reloc->instr = instr;
      reloc->offset = 0;
      reloc->reduce = &asm_reloc_reduce_simm;
      reloc->flag = 0;
    }

  return (0);
}

static opcode_bin_t
asm_reloc_wr_relativ	(struct instr_s		*instr,
			 unsigned int		bit_size,
			 struct num_value_s	*num)
{
  int		size;
  int		value;

  size = num_get_bit_size(num, 0);

  if (size - 2 > (signed)bit_size)
    {
      struct error_s	*err;

      err = error(ERRTYP_ERROR, "sparc: %s bits value can't fit in %s bits signed location",
		  instr->location);
      error_subint(err, size - 2);
      error_subint(err, (signed)bit_size);
      return (0);
    }

  num_get_int(num, &value);

  if (value & 0x3)
    error(ERRTYP_ERROR, "sparc: relative displacement value is not dword aligned",
	  instr->location);
  value /= 4;

  return (value & ((1 << bit_size) - 1));
}

static int
asm_reloc_reduce_rimm	(RELOC_REDUCE_PARAMS)
{
  struct expr_s		*expr = *reloc->expr;
  opcode_bin_t		res;

  switch (expr->flag & EXPR_TYPE_MASK)
    {
    case (EXPR_TYPE_SYMBOL):
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
	    return (0);
	  }

	if (sec != expr->val.sym->section)
	  {
	    error(ERRTYP_WARN, "sparc: relative relocation to other section",
		  reloc->instr->location);
	    return (0);
	  }

	if (lbl_instr->flag & INSTR_ADDR)
	    {
	      struct num_value_s	label;
	      struct num_value_s	target;
	      
	      num_init_uint(&label, lbl_instr->next->offset);
	      num_init_uint(&target, reloc->instr->offset);

	      num_sub(&label, &target, &target);
	      target.flag |= NUM_SIGNED;

	      res = asm_reloc_wr_relativ(reloc->instr,
					 reloc->bits_size,
					 &target);

	      break;
	    }
      }
      return (0);

    case (EXPR_TYPE_NUM):
      res = asm_reloc_wr_relativ(reloc->instr,
				 reloc->bits_size,
				 &expr->val.num);
      break;

    default:
      return (0);
    }

  res <<= reloc->bits_shift;
  asm_opcode_or(res, reloc->instr->val.bin);
  
  return (-1);
}

opcode_bin_t
asm_set_rimm		(struct instr_s		*instr,
			 unsigned int		bit_size,
			 unsigned int		bit_shift,
			 struct section_s	*sec,
			 struct expr_s		**expr)
{
  if ((*expr)->flag & EXPR_TYPE_NUM)
    {
      return (asm_reloc_wr_relativ(instr, bit_size,
				   &(*expr)->val.num)
	      << bit_shift);
    }
  else
    {
      struct reloc_s	*reloc = reloc_add(sec);

      reloc->bits_size = bit_size;
      reloc->bits_shift = bit_shift;
      reloc->expr = expr;
      reloc->instr = instr;
      reloc->offset = 0;
      reloc->reduce = &asm_reloc_reduce_rimm;
      reloc->flag = RELOC_TYPE_RELATIV;
    }

  return (0);
}

