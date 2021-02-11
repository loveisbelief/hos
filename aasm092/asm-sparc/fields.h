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
**    Copyright: Alexandre Becoulet (c) 2003
**    contact  :
*/

#ifndef FIELDS_H_
# define FIELDS_H_

# define	FLD_OP(x)		(unsigned)((x) << 30)
# define	FLD_OP2(x)       	(unsigned)((x) << 22)
# define	FLD_OP3(x)		(unsigned)((x) << 19)
# define	FLD_OPF(x)		(unsigned)((x) << 5)
# define	FLD_OPCOND(x)		(unsigned)((x) << 25)

# define	FLD_RD(x)		(unsigned)((x) << 25)
# define	FLD_RS1(x)		(unsigned)((x) << 14)
# define	FLD_RS2(x)		(unsigned)(x)
# define	FLD_FCC(x)		(unsigned)((x) << 25)
# define	FLD_SIMM11(x)		(unsigned)(x)
# define	FLD_SIMM13(x)		(unsigned)(x)

# define	FLD_DISP30(x)		(unsigned)(x)
# define	FLD_DISP19(x)		(unsigned)(x)
# define	FLD_DISP22(x)		(unsigned)(x)
# define	FLD_IMM22(x)		(unsigned)(x)
# define	FLD_DISP16(x)		(unsigned)(((x) & 0x3fff) | (((x) & 0xc000) << 6))

# define	FLD_ANNUL(x)		(unsigned)((x) << 29)
# define	FLD_TAKEN(x)		(unsigned)((x) << 19)
# define	FLD_IBIT(x)		(unsigned)((x) << 13)
# define	FLD_XBIT(x)		(unsigned)((x) << 12)

# define	FLD_COND_A(x)		(unsigned)((x) << 25)
# define	FLD_COND_B(x)		(unsigned)((x) << 14)
# define	FLD_CC_A(x)		(unsigned)((x) << 20)
# define	FLD_CC_B(x)		(unsigned)((x) << 25)
# define	FLD_CC_C(x)		(unsigned)((x) << 11)
# define	FLD_RCOND_A(x)		(unsigned)((x) << 25)
# define	FLD_RCOND_B(x)		(unsigned)((x) << 10)

# define	FLD_SWTRAP(x)		(unsigned)((x) << 0)
# define	FLD_MMASK(x)		(unsigned)(x)
# define	FLD_CMASK(x)		(unsigned)((x) << 4)
# define	FLD_IMMASI(x)		(unsigned)((x) << 5)
# define	FLD_SHCNT(x)		(unsigned)(x)

/*
** op_type
*/

# define	OP_SHIFT		10
# define	OP_MASK(n)		(((1 << OP_SHIFT) - 1) << (n * OP_SHIFT))

/* n is operand index */

# define	OP_CREG(n)		(0x01 << (n * OP_SHIFT))
# define	OP_RREG(n)		(0x02 << (n * OP_SHIFT))
# define	OP_FSREG(n)		(0x04 << (n * OP_SHIFT))
# define	OP_FDREG(n)		(0x08 << (n * OP_SHIFT))
# define	OP_FQREG(n)		(0x10 << (n * OP_SHIFT))
# define	OP_FREG(n)		(0x1c << (n * OP_SHIFT))
# define	OP_ASRREG(n)		(0x20 << (n * OP_SHIFT))
# define	OP_REGOTHER(n)		(0x40 << (n * OP_SHIFT))

# define	OP_ANYREG(n)		(0x7f << (n * OP_SHIFT))

# define	OP_NONREGMEM(n)		(0x80 << (n * OP_SHIFT))
# define	OP_REGMEM(n)		(0x100 << (n * OP_SHIFT))
# define	OP_MEM(n)		(OP_NONREGMEM(n) | OP_REGMEM(n))

# define	OP_IMM(n)		(0x200 << (n * OP_SHIFT))

# define	OP_ADDR(n)		(OP_IMM(n) | OP_RREG(n))

# define	REG_OTHER_PSR		1
# define	REG_OTHER_FSR		0
# define	REG_OTHER_FQ		3
# define	REG_OTHER_CQ		4
# define	REG_OTHER_CSR		2
# define	REG_OTHER_WIM		5
# define	REG_OTHER_ASR		6
# define	REG_OTHER_TBR		7
# define	REG_OTHER_FCC		8

/*
** i_flags
*/

# define	FLG_OPCOUNT_MASK	0x7
# define	FLG_OPCOUNT(n)		(n & FLG_OPCOUNT_MASK)

# define	FLG_V8		0x10
# define	FLG_V9		0x20
# define	FLG_MASK	(FLG_V8 | FLG_V9)

# define	FLG_SYNTHETIC	0x40
# define	FLG_WARNING	0x80

# define	FLG_DELAY_SLOT	0x100

/*
** register num
*/

#define REG_G(n)		(n)
#define REG_O(n)		((n) + 8)
#define REG_L(n)		((n) + 16)
#define REG_I(n)		((n) + 24)

#endif

