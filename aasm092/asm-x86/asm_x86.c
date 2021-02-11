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

#include "aasm/types.h"
#include "aasm/instr.h"
#include "aasm/expr.h"

#include "flags.h"
#include "asm.h"

#include "opcodes_id.h"

/*
** 2 operands
** 1 reg/mem, 1 reg/mem
*/

void			asm_i_2rm(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;
  aasm_u8_t		tmp = 0;

  /* set d field */
  if ((op_flag[1] & OP_TYPE_MEM)
      && (!(instr->info->i_flag & I_NO_DIRECTION_FLAG)))
	tmp = OP_FIELD_D;

  /* add SSE prefix if needed */
  if (instr->info->i_flag & I_PREFIXE_SSE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* operand and address size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* add w field if instruction use 16 or 32 bits operand */
  if ((instr->info->i_flag & I_FLAG_W_FIELD)
      && (op_size[0] & (OP_SIZE_16BITS | OP_SIZE_32BITS)))
    tmp |= OP_FIELD_W;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* copy opcode byte ored with flag */
  instr->val.bin[i++] = tmp | instr->info->fix[j++];

  /* copy reg/mem byte */
  tmp = instr->info->fix[j++];

  if ((op_flag[0] | op_flag[1]) & OP_TYPE_MEM)
    {
      if (op_flag[0] & OP_TYPE_MEM)
	{
	  instr->val.bin[i] = tmp | OP_FIELD_MOD_MEM
	    | (instr->operands[1]->val.reg->num << 3);
	  i = asm_x86_set_mem(instr, i, instr->operands[0], sec);
	}
      else
	{
	  instr->val.bin[i] = tmp | OP_FIELD_MOD_MEM
	    | (instr->operands[0]->val.reg->num << 3);
	  i = asm_x86_set_mem(instr, i, instr->operands[1], sec);
	}
    }
  else
    {
      /* copy register numbers */
      if (instr->info->i_flag & I_INVERT_DIR)
	instr->val.bin[i++] = tmp | OP_FIELD_MOD_REG
	  | (instr->operands[0]->val.reg->num << 3)
	  | (instr->operands[1]->val.reg->num);
      else
	instr->val.bin[i++] = tmp | OP_FIELD_MOD_REG
	  | (instr->operands[0]->val.reg->num)
	  | (instr->operands[1]->val.reg->num << 3);
    }

  /* copy suffix byte */
  if (instr->info->i_flag & I_SUFIXE)
    instr->val.bin[i++] = instr->info->fix[j];

  /* add immediate value as third operand */
  if (instr->info->i_type[2] & OP_TYPE_IM)
    i += asm_x86_set_imm_iexpr(instr, i, op_size[2],
			       sec, &instr->operands[2]);

  /* set instruction size */
  instr_set_asm(instr, i);
}

/*
** 2 operands
** 1 reg/mem, 1 imediate
*/

void	asm_i_1rm1im		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;
  aasm_u8_t		tmp = 0;

  /* add SSE prefix if needed */
  if (instr->info->i_flag & I_PREFIXE_SSE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* operand and address size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* add w field if instruction use 16 or 32 bits operand */
  if ((instr->info->i_flag & I_FLAG_W_FIELD)
      && (op_size[0] & (OP_SIZE_16BITS | OP_SIZE_32BITS)))
    tmp |= OP_FIELD_W;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* copy opcode byte ored with flag */
  instr->val.bin[i++] = tmp | instr->info->fix[j++];

  /* copy reg/mem byte */
  tmp = instr->info->fix[j++];

  if (op_flag[0] & OP_TYPE_MEM)
    {
      instr->val.bin[i] = tmp | OP_FIELD_MOD_MEM;
      i = asm_x86_set_mem(instr, i, instr->operands[0], sec);
    }
  else
    instr->val.bin[i++] = tmp | OP_FIELD_MOD_REG
      | (instr->operands[0]->val.reg->num);

  i += asm_x86_set_imm_iexpr(instr, i, op_size[1], sec,
			     &instr->operands[1]);

  instr_set_asm(instr, i);
}

/*
** 1 operands
** 1 imediate
*/

void	asm_i_1im		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;
  aasm_u8_t		tmp = 0;

  /* operand size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* add w field if instruction use 16 or 32 bits operand */
  if ((instr->info->i_flag & I_FLAG_W_FIELD)
      && (op_size[0] & (OP_SIZE_16BITS | OP_SIZE_32BITS)))
    tmp |= OP_FIELD_W;

  /* copy opcode byte ored with flag */
  instr->val.bin[i++] = tmp | instr->info->fix[j++];

  {
    unsigned int	op_im = op_flag[0] & OP_TYPE_IM ? 0 : 1;

    if (instr->info->i_flag & I_RELATIV_IM)
      /* have to be last instruction bytes */
      i += asm_x86_set_imm_riexpr(instr, i, op_size[op_im], sec, &instr->operands[op_im]);
    else
      i += asm_x86_set_imm_iexpr(instr, i, op_size[op_im], sec, &instr->operands[op_im]);
  }

  instr_set_asm(instr, i);
}

/*
** 2 operands
** 2 imediate
*/

void	asm_i_2im		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;

  /* operand size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[1] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[1] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* copy opcode byte */
  instr->val.bin[i++] = instr->info->fix[j++];

  if (instr->info->i_flag & I_INVERT_DIR)
    {
      i += asm_x86_set_imm_iexpr(instr, i, op_size[1],
				 sec, &instr->operands[1]);
  
      i += asm_x86_set_imm_iexpr(instr, i, op_size[0],
				 sec, &instr->operands[0]);
    }
  else
    {
      i += asm_x86_set_imm_iexpr(instr, i, op_size[0],
				 sec, &instr->operands[0]);
      
      i += asm_x86_set_imm_iexpr(instr, i, op_size[1],
				 sec, &instr->operands[1]);
    }

  instr_set_asm(instr, i);
}

/*
** in/out specific
**
*/

void	asm_i_inout		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0, op;
  aasm_u8_t		tmp = 0;

  switch (instr->info->id)
    {
    case (OPC_IN_0):
    case (OPC_IN_1):
      op = 0;
      break;

 /* case (OPC_OUT_0): */
 /* case (OPC_OUT_1): */
    default:
      op = 1;
      break;
    }

  /* operand size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if (op_size[op] & OP_SIZE_32BITS)
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if (op_size[op] & OP_SIZE_16BITS)
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* add w field if instruction use 16 or 32 bits operand */
  if (op_size[op] & (OP_SIZE_16BITS | OP_SIZE_32BITS))
    tmp |= OP_FIELD_W;

  /* copy opcode byte */
  instr->val.bin[i++] = instr->info->fix[j++] | tmp;

  /* add immediate as seconde operand if needed */
  if (instr->info->i_type[0] & OP_TYPE_IM)
    i += asm_x86_set_imm_iexpr(instr, i, SIZE_FLAG_BYTE,
			       sec, &instr->operands[0]);

  if (instr->info->i_type[1] & OP_TYPE_IM)
    i += asm_x86_set_imm_iexpr(instr, i, SIZE_FLAG_BYTE,
			       sec, &instr->operands[1]);

  instr_set_asm(instr, i);
}

/*
** 1 operands
** 1 short reg
*/

void	asm_i_1r_short		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0, op = 0;
  aasm_u8_t		tmp = 0;

  /* operand size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* add w field if instruction use 16 or 32 bits operand */
  if ((instr->info->i_flag & I_FLAG_W_FIELD)
      && (op_size[0] & (OP_SIZE_16BITS | OP_SIZE_32BITS)))
    tmp |= OP_FIELD_W << 3;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* copy opcode byte ored with flag */
  if (instr->info->i_flag & I_1RM_SECOND_OP)
    op++;

  if (instr->info->i_flag & I_INVERT_DIR)
    instr->val.bin[i++] = tmp | instr->info->fix[j]
      | (instr->operands[op]->val.reg->num << 3);
  else
    instr->val.bin[i++] = tmp | instr->info->fix[j]
      | (instr->operands[op]->val.reg->num);

  /* add immediate as seconde operand if needed */
  if (instr->info->i_type[1] & OP_TYPE_IM)
    i += asm_x86_set_imm_iexpr(instr, i, op_size[1],
			       sec, &instr->operands[1]);

  instr_set_asm(instr, i);
}

/*
** 1 operands
** 1 reg/mem
*/

void	asm_i_1rm		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;
  aasm_u8_t		tmp = 0;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE_INSTR)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* operand and address size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((op_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((op_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* fpu memory access size flag */
  if ((instr->info->i_flag & I_FPU_MEM_SIZE)
      && !(op_size[0] & OP_SIZE_32BITS))
    tmp |= OP_FIELD_FPU_SIZE;

  /* add w field if instruction use 16 or 32 bits operand */
  if ((instr->info->i_flag & I_FLAG_W_FIELD)
      && (op_size[0] & (OP_SIZE_16BITS | OP_SIZE_32BITS)))
    tmp |= OP_FIELD_W;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* copy opcode byte ored with flag */
  instr->val.bin[i++] = tmp | instr->info->fix[j++];

  /* copy reg/mem byte */
  tmp = instr->info->fix[j++];

  if ((op_flag[0] | op_flag[1]) & OP_TYPE_MEM)
    {
      instr->val.bin[i] = tmp;
      i = asm_x86_set_mem(instr, i, instr->operands[0], sec);
    }
  else
    {
      if (instr->info->i_flag & I_1RM_SECOND_OP)
	instr->val.bin[i++] = tmp | OP_FIELD_MOD_REG
	  | instr->operands[1]->val.reg->num;
      else
	instr->val.bin[i++] = tmp | OP_FIELD_MOD_REG
	  | instr->operands[0]->val.reg->num;
    }

  instr_set_asm(instr, i);
}

/*
** 0 operands
*/

void	asm_i_void		(ASM_I_PARAMS)
{
  unsigned int		i = 0, j = 0;

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE_INSTR)
    instr->val.bin[i++] = instr->info->fix[j++];

  /* operand size prefix bytes */
  if (instr->asm_flag & ASM_SECTION_CODE16)
    {
      if ((instr->info->i_size[0] & OP_SIZE_32BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }
  else
    {
      if ((instr->info->i_size[0] & OP_SIZE_16BITS)
	  && !(instr->info->i_flag & I_NO_PREFIXE))
	instr->val.bin[i++] = PREFIXE_INV_SIZE;
    }

  /* copy opcode prebyte from fix binary part */
  if (instr->info->i_flag & I_PREBYTE)
      instr->val.bin[i++] = instr->info->fix[j++];
 
  /* copy opcode byte */
  instr->val.bin[i++] = instr->info->fix[j++];

  /* copy suffix byte */
  if (instr->info->i_flag & I_SUFIXE)
    instr->val.bin[i++] = instr->info->fix[j++];

  instr_set_asm(instr, i);
}
