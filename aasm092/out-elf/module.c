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

#include "aasm/modules.h"
#include "aasm/arch_id.h"

#ifdef AASM_STATIC

# ifdef ELF_64BITS
static const struct module_info_s	out_elf64_module_info =
# else
static const struct module_info_s	out_elf32_module_info =
# endif

#else
const struct module_info_s		module_info =
#endif
{
  1,
#ifdef ELF_64BITS
  "out-elf64",
#else
  "out-elf32",
#endif
  "Alexandre Becoulet 2002",
#ifdef ELF_64BITS
  "64bits ELF writer",
#else
  "32bits ELF writer",
#endif
  {0, 9, 1},
  ARCH_ID_NONE,
  MODULE_CLASS_OUT,
  1
};

#ifdef AASM_STATIC

#include "elf.h"

# ifdef ELF_64BITS
struct module_s				out_elf64_module =
{
  &out_elf64_module_info,
# else
struct module_s				out_elf32_module =
{
  &out_elf32_module_info,
# endif
  0,
  0,
  NULL,
  NULL,
  {
    {
      (void*)&out_elf_write,
      (void*)&out_elf_directive,
      (void*)&out_elf_newobject,
      (void*)&out_elf_newsection,
      (void*)&out_elf_newsymbol,
      (void*)&out_elf_clean,
    }
  }
};

#endif

