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

#include <string.h>
#include <stdio.h>

#include "aasm/types.h"

#include "elf.h"

void
elf_write_ulsb64	(void			*ptr,
			 aasm_u64_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[4] = (val >> 32) & 0xff;
  ((aasm_u8_t*)ptr)[5] = (val >> 40) & 0xff;
  ((aasm_u8_t*)ptr)[6] = (val >> 48) & 0xff;
  ((aasm_u8_t*)ptr)[7] = (val >> 56) & 0xff;
}

void
elf_write_umsb64	(void			*ptr,
			 aasm_u64_t		val)
{
  ((aasm_u8_t*)ptr)[0] = (val >> 56) & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 48) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 40) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 32) & 0xff;
  ((aasm_u8_t*)ptr)[4] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[5] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[6] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[7] = val & 0xff;
}

void
elf_write_slsb64	(void			*ptr,
			 aasm_s64_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[4] = (val >> 32) & 0xff;
  ((aasm_u8_t*)ptr)[5] = (val >> 40) & 0xff;
  ((aasm_u8_t*)ptr)[6] = (val >> 48) & 0xff;
  ((aasm_u8_t*)ptr)[7] = (val >> 56) & 0xff;
  /*
  if (val < 0)
    memset(((aasm_u8_t*)ptr) + 4, 0xff, 4);
  else
    memset(((aasm_u8_t*)ptr) + 4, 0, 4);
  */
}

void
elf_write_smsb64	(void			*ptr,
			 aasm_s64_t		val)
{
  /*
  if (val < 0)
    memset(ptr, 0xff, 4);
  else
    memset(ptr, 0, 4);
  */
  ((aasm_u8_t*)ptr)[0] = (val >> 56) & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 48) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 40) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 32) & 0xff;
  ((aasm_u8_t*)ptr)[4] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[5] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[6] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[7] = val & 0xff;
}

void
elf_write_ulsb32	(void			*ptr,
			 aasm_u32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 24) & 0xff;
}

void
elf_write_umsb32	(void			*ptr,
			 aasm_u32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[3] = val & 0xff;
}

void
elf_write_slsb32	(void			*ptr,
			 aasm_s32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[3] = (val >> 24) & 0xff;
}

void
elf_write_smsb32	(void			*ptr,
			 aasm_s32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = (val >> 24) & 0xff;
  ((aasm_u8_t*)ptr)[1] = (val >> 16) & 0xff;
  ((aasm_u8_t*)ptr)[2] = (val >> 8) & 0xff;
  ((aasm_u8_t*)ptr)[3] = val & 0xff;
}

void
elf_write_ulsb16	(void			*ptr,
			 aasm_u32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val & 0xff;
  ((aasm_u8_t*)ptr)[1] = val >> 8;
}

void
elf_write_umsb16	(void			*ptr,
			 aasm_u32_t		val)
{
  ((aasm_u8_t*)ptr)[0] = val >> 8;
  ((aasm_u8_t*)ptr)[1] = val & 0xff;
}

