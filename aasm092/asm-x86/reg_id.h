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

#ifndef REG_ID_H_

/*
** register numbers
**
** used in cpu opcodes
*/

# define REG_NUM_EAX		0
# define REG_NUM_ECX		1
# define REG_NUM_EDX		2
# define REG_NUM_EBX		3
# define REG_NUM_ESP		4
# define REG_NUM_EBP		5
# define REG_NUM_ESI		6
# define REG_NUM_EDI		7

# define REG_NUM_AX		0
# define REG_NUM_CX		1
# define REG_NUM_DX		2
# define REG_NUM_BX		3
# define REG_NUM_SP		4
# define REG_NUM_BP		5
# define REG_NUM_SI		6
# define REG_NUM_DI		7

# define REG_NUM_AL		0
# define REG_NUM_CL		1
# define REG_NUM_DL		2
# define REG_NUM_BL		3
# define REG_NUM_AH		4
# define REG_NUM_CH		5
# define REG_NUM_DH		6
# define REG_NUM_BH		7

# define REG_NUM_ES		0
# define REG_NUM_CS		1
# define REG_NUM_SS		2
# define REG_NUM_DS		3
# define REG_NUM_FS		4
# define REG_NUM_GS		5

# define REG_NUM_MM0		0
# define REG_NUM_MM1		1
# define REG_NUM_MM2		2
# define REG_NUM_MM3		3
# define REG_NUM_MM4		4
# define REG_NUM_MM5		5
# define REG_NUM_MM6		6
# define REG_NUM_MM7		7

# define REG_NUM_ST0		0
# define REG_NUM_ST1		1
# define REG_NUM_ST2		2
# define REG_NUM_ST3		3
# define REG_NUM_ST4		4
# define REG_NUM_ST5		5
# define REG_NUM_ST6		6
# define REG_NUM_ST7		7

# define REG_NUM_XMM0		0
# define REG_NUM_XMM1		1
# define REG_NUM_XMM2		2
# define REG_NUM_XMM3		3
# define REG_NUM_XMM4		4
# define REG_NUM_XMM5		5
# define REG_NUM_XMM6		6
# define REG_NUM_XMM7		7

# define REG_NUM_CR0		0
# define REG_NUM_CR2		2
# define REG_NUM_CR3		3
# define REG_NUM_CR4		4

# define REG_NUM_DR0		0
# define REG_NUM_DR1		1
# define REG_NUM_DR2		2
# define REG_NUM_DR3		3
# define REG_NUM_DR4		4
# define REG_NUM_DR5		5
# define REG_NUM_DR6		6
# define REG_NUM_DR7		7

/* 
** regster ids
**
** eax, ax, al and ah have the same reg id
*/

# define REG_ID_EAX		0
# define REG_ID_ECX		1
# define REG_ID_EDX		2
# define REG_ID_EBX		3
# define REG_ID_ESP		4
# define REG_ID_EBP		5
# define REG_ID_ESI		6
# define REG_ID_EDI		7

# define REG_ID_AX		0
# define REG_ID_CX		1
# define REG_ID_DX		2
# define REG_ID_BX		3
# define REG_ID_SP		4
# define REG_ID_BP		5
# define REG_ID_SI		6
# define REG_ID_DI		7

# define REG_ID_AL		0
# define REG_ID_CL		1
# define REG_ID_DL		2
# define REG_ID_BL		3
# define REG_ID_AH		0
# define REG_ID_CH		1
# define REG_ID_DH		2
# define REG_ID_BH		3

# define REG_ID_ES		8
# define REG_ID_CS		9
# define REG_ID_SS		10
# define REG_ID_DS		11
# define REG_ID_FS		12
# define REG_ID_GS		13

# define REG_ID_MM0		14
# define REG_ID_MM1		15
# define REG_ID_MM2		16
# define REG_ID_MM3		17
# define REG_ID_MM4		18
# define REG_ID_MM5		19
# define REG_ID_MM6		20
# define REG_ID_MM7		21

# define REG_ID_ST0		14
# define REG_ID_ST1		15
# define REG_ID_ST2		16
# define REG_ID_ST3		17
# define REG_ID_ST4		18
# define REG_ID_ST5		19
# define REG_ID_ST6		20
# define REG_ID_ST7		21

# define REG_ID_XMM0		22
# define REG_ID_XMM1		23
# define REG_ID_XMM2		24
# define REG_ID_XMM3		25
# define REG_ID_XMM4		26
# define REG_ID_XMM5		27
# define REG_ID_XMM6		28
# define REG_ID_XMM7		29

# define REG_ID_CR0		30
# define REG_ID_CR2		31
# define REG_ID_CR3		32
# define REG_ID_CR4		33

# define REG_ID_DR0		34
# define REG_ID_DR1		35
# define REG_ID_DR2		36
# define REG_ID_DR3		37
# define REG_ID_DR4		38
# define REG_ID_DR5		39
# define REG_ID_DR6		40
# define REG_ID_DR7		41

# define REG_ID_COUNT		42

# define REG_ID_STACK		4

/* register overlap mask */

# define REG_BYTE_LO_PART	0x1
# define REG_BYTE_HI_PART	0x2
# define REG_WORD_HI_PART	0x4
# define REG_WORD_LO_PART	0x3
# define REG_DWORD_PART		0x7

#endif

