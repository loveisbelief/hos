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

#ifndef ASM_MEM_TABLE_H_
# define ASM_MEM_TABLE_H_

/* table input */

# define X86_MEM_BASE_ID_MASK	0x7	
# define X86_MEM_BASE_PRESENT	0x8
# define X86_MEM_BASE_WORD	0x10

# define X86_MEM_BASE_COUNT	32

# define X86_MEM_INDEX_ID_MASK	X86_MEM_BASE_ID_MASK
# define X86_MEM_INDEX_PRESENT	X86_MEM_BASE_PRESENT
# define X86_MEM_INDEX_WORD	X86_MEM_BASE_WORD

# define X86_MEM_INDEX_COUNT	X86_MEM_BASE_COUNT

# define X86_MEM_DIS_NONE	0x0
# define X86_MEM_DIS_BYTE	0x1
# define X86_MEM_DIS_WORD	0x2
# define X86_MEM_DIS_DWORD	0x3

# define X86_MEM_DIS_COUNT	4

# define X86_MEM_FACTOR_PRESENT	0x1

# define X86_MEM_FACTOR_COUNT	2

/* table output */

# define X86_MEM_RM		0x0
# define X86_MEM_SIB		0x1
# define X86_MEM_FLAG		0x2

# define X86_MEM_FLAG_ERR_MASK	0xf00
# define X86_MEM_FLAG_ERR_SHIFT	8

# define X86_MEM_ERR_16COMBIN	0x100 /* invalid 16 bits registers combinaison */
# define X86_MEM_ERR_1632COMBIN	0x200 /* word and dword register combinaison */
# define X86_MEM_ERR_ESP_INDEX	0x300 /* esp used as index register */
# define X86_MEM_ERR_DISDWORD	0x400 /* dword displacement with word register */
# define X86_MEM_ERR_DISWORD	0x500 /* word displacement with dword register */
# define X86_MEM_ERR_16FACT	0x600 /* scale factor with word register */
# define X86_MEM_ERR_INVDIS	0x700 /* invalid displacement size */
# define X86_MEM_ERR_INV16	0x800 /* invalid 16 bits register */
# define X86_MEM_ERR_UNKNOW	0x900 /* should not append */

# define X86_MEM_FLAG_SIB	0x01 /* sib byte is present */

# define X86_MEM_FLAG_DIS_BYTE	0x02 /* byte mem displacement */
# define X86_MEM_FLAG_DIS_WORD	0x04 /* word mem displacement */
# define X86_MEM_FLAG_DIS_DWORD	0x08 /* dword mem displacement */
# define X86_MEM_FLAG_DIS_MASK	(X86_MEM_FLAG_DIS_BYTE | X86_MEM_FLAG_DIS_WORD | X86_MEM_FLAG_DIS_DWORD)

# define X86_MEM_FLAG_CODE16	0x10 /* 16 bits address mode */

# define X86_MEM_OUT_COUNT	3

struct				mem_table_s
{
  unsigned char			rm;
  unsigned char			sib;
  unsigned int			flag;
};

extern const struct mem_table_s	mem_table\
			        [X86_MEM_FACTOR_COUNT]\
			        [X86_MEM_DIS_COUNT]\
			        [X86_MEM_BASE_COUNT]\
				[X86_MEM_INDEX_COUNT];

#endif
