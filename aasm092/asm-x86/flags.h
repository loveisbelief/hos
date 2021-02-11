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

#ifndef FLAGS_H_
# define FLAGS_H_

# include "aasm/size.h"

#define X86_MAX_INSTR_SIZE	14

#define OP_TYPE_COUNTER		(0x00000002)
#define OP_TYPE_ACCUMULATOR	(0x00000004)
#define OP_TYPE_GENERAL		(0x00000008)
#define OP_TYPE_DATA		(0x00000010)
#define OP_TYPE_SEG_CS		(0x00000020)
#define OP_TYPE_SEG_OLD		(0x00000040)
#define OP_TYPE_SEG_NEW		(0x00000080)
#define OP_TYPE_SEG		(OP_TYPE_SEG_OLD | OP_TYPE_SEG_NEW)
#define OP_TYPE_CONTROL		(0x00000100)
#define OP_TYPE_FPU_TOP		(0x00000200)
#define OP_TYPE_FPU_TAIL	(0x00000400)
#define OP_TYPE_FPU		(OP_TYPE_FPU_TOP | OP_TYPE_FPU_TAIL)
#define OP_TYPE_MMX		(0x00000800)
#define OP_TYPE_SSE		(0x00001000)
#define OP_TYPE_DEBUG		(0x00002000)

#define OP_TYPE_IM		(0x00000001)
#define OP_TYPE_MEM		(0x00004000)

#define OP_TYPE_WORK		(OP_TYPE_COUNTER | OP_TYPE_ACCUMULATOR\
				| OP_TYPE_GENERAL | OP_TYPE_DATA)

#define OP_TYPE_REGISTER	(OP_TYPE_COUNTER | OP_TYPE_ACCUMULATOR\
				| OP_TYPE_GENERAL | OP_TYPE_DATA\
				| OP_TYPE_SEG | OP_TYPE_CONTROL\
				| OP_TYPE_CONTROL | OP_TYPE_DEBUG\
				| OP_TYPE_FPU | OP_TYPE_MMX | OP_TYPE_SSE)

#define OP_SIZE_MASK		(0x7f)

#define OP_SIZE_8BITS		(SIZE_FLAG_BYTE)
#define OP_SIZE_16BITS		(SIZE_FLAG_WORD)
#define OP_SIZE_32BITS		(SIZE_FLAG_DWORD)
#define OP_SIZE_48BITS		(SIZE_FLAG_FWORD)
#define OP_SIZE_64BITS		(SIZE_FLAG_QWORD)
#define OP_SIZE_80BITS		(SIZE_FLAG_TWORD)
#define OP_SIZE_128BITS		(SIZE_FLAG_OWORD)

#define OP_SIZE_8_16_32		(OP_SIZE_8BITS | OP_SIZE_16BITS | OP_SIZE_32BITS)

/*** opcode bits and prefix bytes ***/

#define PREFIXE_INV_SIZE	0x00000066
#define PREFIXE_INV_ASIZE	0x00000067

#define OP_FIELD_W		0x00000001
#define OP_FIELD_D		0x00000002
#define OP_FIELD_S		0x00000002

#define OP_FIELD_MOD_MEM	0x00000000
#define OP_FIELD_MOD_MEM_D8	0x00000040
#define OP_FIELD_MOD_MEM_D32	0x00000080
#define OP_FIELD_MOD_REG	0x000000C0

#define OP_FIELD_RM_INDEX	0x00000004
#define OP_FIELD_RM_IM		0x00000005

#define OP_FIELD_SIB_SCAL1	0x00000000
#define OP_FIELD_SIB_SCAL2	0x00000040
#define OP_FIELD_SIB_SCAL4	0x00000080
#define OP_FIELD_SIB_SCAL8	0x000000C0

#define OP_FIELD_SIB_32		0x00000005

#define OP_FIELD_FPU_SIZE	0x00000004

/*** instruction flag (i_flag field) ***/

#define I_OP_COUNT_0		0x00000000
#define I_OP_COUNT_1		0x00000001
#define I_OP_COUNT_2		0x00000002
#define I_OP_COUNT_3		0x00000003
#define I_OP_COUNT_MASK		0x00000003

#define I_FLAG_OP_SAME_SIZE	0x00000004
/*#define I_FLAG_LAST		0x00000008*/
#define I_FLAG_SHORT_IMEDIAT	0x00000010	/* short form instruction exist */
#define I_FLAG_W_FIELD		0x00000020

#define I_PREBYTE		0x00000040 /* pre byte added after size prefix */
#define I_PREBYTE_INSTR		0x00000080 /* pre byte added before size prefix */
#define I_NO_PREFIXE_A		0x00000100
#define I_NO_PREFIXE		0x00000200
#define I_NO_DIRECTION_FLAG	0x00000400	/* do not add direction bit */
#define I_INVERT_DIR		0x00000800	/* invert order for reg, reg instruction */
#define I_SELECT_SIZE_OP12	0x00001000	/* consider operand 1 and 2 size for instruction match */
#define I_SELECT_SIZE_OP3	0x00002000	/* consider operand 1 and 2 size for instruction match */
#define I_SUFIXE		0x00004000
#define I_PREFIXE_SSE		0x00008000
#define I_1RM_SECOND_OP		0x00010000	/* use second operand for 1_rm */
#define I_FPU_MEM_SIZE		0x00020000
#define I_RELATIV_IM		0x00040000	/* immediate value is relative to next instruction */

#define I_686O			0x00100000
#define I_586O			0x00200000
#define I_486O			0x00400000
#define I_386O			0x00800000

#define I_686			I_686O
#define I_586			(I_686 | I_586O)
#define I_486			(I_586 | I_486O)
#define I_386			(I_486 | I_386O)

#define I_ALL_X86		I_386

#define I_FPU			0x02000000
#define I_MMX			0x04000000
#define I_3DNOW			0x08000000
#define I_MMX_EXT		0x10000000
#define I_3DNOW_EXT		0x20000000
#define I_SSE			0x40000000
#define I_SSE2			0x80000000

#define I_ALL_EXT		(I_FPU | I_MMX | I_3DNOW\
				| I_MMX_EXT | I_3DNOW_EXT\
				| I_SSE | I_SSE2)

#endif
