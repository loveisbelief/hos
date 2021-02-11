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
**    Copyright Alexandre Becoulet, 2003
**    contact : alexandre.becoulet@free.fr
*/

#ifndef COFF_BITS_H_
# define COFF_BITS_H_

# include "aasm/types.h"

/*
** COFF file header
*/

struct				coff_header_s
{
  aasm_u16_t			magic;		/* magic number */
  aasm_u16_t			sec_cnt;	/* section count */
  aasm_u32_t			time_stamp;	/* time stamp */
  aasm_u32_t			symtab_pos;	/* symbol table location in file */
  aasm_u32_t			sym_cnt;	/* symbol count */
  aasm_u16_t			opthdr_size;	/* optional header size */
  aasm_u16_t			flags;
};

#define SIZEOF_COFF_HEADER_S	20

#define COFF_MAGIC_I386		0x014c
#define COFF_MAGIC_MIPS		0x0166
#define COFF_MAGIC_ALPHA	0x0184
#define COFF_MAGIC_ARM		0x01c0
#define COFF_MAGIC_POWERPC	0x01f0
#define COFF_MAGIC_M68K		0x0268

/* unix djgpp ms */
#define COFF_HDRFLG_NO_RELOC	0x0001		/* no relocations in object */
#define COFF_HDRFLG_RESOLVED	0x0002		/* no unresolved symbols in object */
#define COFF_HDRFLG_NO_LINE	0x0004		/* no line number info */
#define COFF_HDRFLG_NO_LOCAL	0x0008		/* no local symbol in sym table */

/* ms */
#define COFF_HDRFLG_LE		0x0080		/* file is little endian */
/* unix */
#define COFF_HDRFLG_16LE	0x0080		/* file is 16 bits little endian */

/* ms */
#define COFF_HDRFLG_32		0x0100		/* file is 32 bits */
/* unix djgpp */
#define COFF_HDRFLG_32LE	0x0100		/* file is 32 bits little endian */

/*
** COFF file section table (follow header/optional header)
*/

#define COFF_SECNAME_SIZE	8

struct				coff_sechdr_s
{
  char				name[COFF_SECNAME_SIZE];
						/* section name */
  aasm_u32_t			ph_addr;	/* physical address / virtual size (ms) */
  aasm_u32_t			vt_addr;	/* virtual address */
  aasm_u32_t			size;		/* section size */
  aasm_u32_t			data_pos;	/* section data location in file */
  aasm_u32_t			reloc_pos;	/* reloc table location in file */
  aasm_u32_t			line_pos;	/* line number location in file */
  aasm_u16_t			reloc_cnt;	/* relocation count */
  aasm_u16_t			line_cnt;	/* line info count */
  aasm_u32_t			flags;
};

#define SIZEOF_COFF_SECHDR_S	40

/* djgpp unix ms */
#define COFF_SECFLG_TYPECODE	0x00000020
#define COFF_SECFLG_TYPEDATA	0x00000040
#define COFF_SECFLG_TYPEBSS	0x00000080

/* unix ms */
#define COFF_SECFLG_COMMENT	0x00000200 /* comment/info section type */

/* ms */
#define COFF_SECFLG_ALIGN_1	0x00100000
#define COFF_SECFLG_ALIGN_2	0x00200000
#define COFF_SECFLG_ALIGN_4	0x00300000
#define COFF_SECFLG_ALIGN_8	0x00400000
#define COFF_SECFLG_ALIGN_16	0x00500000
#define COFF_SECFLG_ALIGN_32	0x00600000
#define COFF_SECFLG_ALIGN_64	0x00700000
#define COFF_SECFLG_ALIGN_128	0x00800000
#define COFF_SECFLG_ALIGN_256	0x00900000
#define COFF_SECFLG_ALIGN_512	0x00a00000
#define COFF_SECFLG_ALIGN_1024	0x00b00000
#define COFF_SECFLG_ALIGN_2048	0x00d00000
#define COFF_SECFLG_ALIGN_4096	0x00d00000
#define COFF_SECFLG_ALIGN_8192	0x00e00000

/* ms */
#define COFF_SECFLG_EXEC	0x20000000
#define COFF_SECFLG_READ	0x40000000
#define COFF_SECFLG_WRITE	0x80000000

/*
** COFF symbol table entry
*/

#define COFF_SYM_SHRTNAM_SIZE	8

struct				coff_sym_s
{
  union
  {
    struct
    {
      aasm_u32_t		is_longname;
      aasm_u32_t		str_pos;	/* name in str table if is_longname != 0 */
    }				ext;
    char			short_name[COFF_SYM_SHRTNAM_SIZE];
						/* name if is_longname == 0 */
  }				name;

  aasm_u32_t			value;		/* symbol value */
  aasm_u16_t			section;	/* section number */
  aasm_u16_t			type;		/* symbol type */
  aasm_u8_t			class;		/* symbol storage class */
  aasm_u8_t			aux;		/* symbol aux entries */
};

#define SIZEOF_COFF_SYM_S	18

#define COFF_SYMSEC_UNDEF	0x0000		/* undefined/extern symbol */
#define COFF_SYMSEC_ABS		0xffff		/* constant symbol */
#define COFF_SYMSEC_DEBUG	0xfffe		/* debug symbol */

#define COFF_SYMTYP_NULL	0x00
#define COFF_SYMTYP_VOID	0x01
#define COFF_SYMTYP_CHAR	0x02
#define COFF_SYMTYP_SHORT	0x03
#define COFF_SYMTYP_INT		0x04
#define COFF_SYMTYP_LONG	0x05
#define COFF_SYMTYP_FLOAT	0x06
#define COFF_SYMTYP_DOUBLE	0x07
#define COFF_SYMTYP_STRUCT	0x08
#define COFF_SYMTYP_UNION	0x09
#define COFF_SYMTYP_ENUM	0x0a
#define COFF_SYMTYP_MOE		0x0b
#define COFF_SYMTYP_UCHAR	0x0c
#define COFF_SYMTYP_USHORT	0x0d
#define COFF_SYMTYP_UINT	0x0e
#define COFF_SYMTYP_ULONG	0x0f
#define COFF_SYMTYP_LONGDBL	0x10

#define COFF_SYMTYP_PTR		0x10
#define COFF_SYMTYP_FUNC	0x20
#define COFF_SYMTYP_ARRAY	0x30

#define COFF_SYMCLS_NULL	0
#define COFF_SYMCLS_AUTO	1
#define COFF_SYMCLS_EXTERN	2
#define COFF_SYMCLS_STATIC	3
#define COFF_SYMCLS_REGISTER	4
#define COFF_SYMCLS_EXTDEF	5
#define COFF_SYMCLS_LABEL	6
#define COFF_SYMCLS_ULABEL	7
#define COFF_SYMCLS_MOS		8
#define COFF_SYMCLS_ARG		9
#define COFF_SYMCLS_STRARG	10
#define COFF_SYMCLS_MOU		11
#define COFF_SYMCLS_UNTAG	12
#define COFF_SYMCLS_TPDEF	13
#define COFF_SYMCLS_USTATIC	14
#define COFF_SYMCLS_ENTAG	15
#define COFF_SYMCLS_MOE		16
#define COFF_SYMCLS_REGPARM	17
#define COFF_SYMCLS_FIELD	18
#define COFF_SYMCLS_AUTOARG	19
#define COFF_SYMCLS_LASTENT	20
#define COFF_SYMCLS_BLOCK	100
#define COFF_SYMCLS_FCN		101
#define COFF_SYMCLS_EOS		102
#define COFF_SYMCLS_FILE	103
#define COFF_SYMCLS_LINE	104
#define COFF_SYMCLS_ALIAS	105
#define COFF_SYMCLS_HIDDEN	106
#define COFF_SYMCLS_EFCN	255

/*
** COFF string table (follow symbol table)
*/

struct				coff_str_table_s
{
  aasm_u32_t			size;

  /*
  ** string data following
  */
};

#define SIZEOF_COFF_STR_TABLE_S	4

struct				coff_reloc_s
{
  aasm_u32_t			address;
  aasm_u32_t			symbol;
  aasm_u16_t			type;
};

#define COFF_RELOC_S_SIZE	10	/* struct size is not multiple of 4
					   sizeof(struct coff_reloc_s) may not work */

#define COFF_RELTYP_I386_NONE	0x0000	/* unix ms */
#define COFF_RELTYP_I386_DIR16	0x0001	/* unix */
#define COFF_RELTYP_I386_REL16	0x0002	/* unix */
#define COFF_RELTYP_I386_DIR32	0x0006	/* unix djgpp ms */
#define COFF_RELTYP_I386_REL32	0x0014	/* unix djgpp ms */

#endif

