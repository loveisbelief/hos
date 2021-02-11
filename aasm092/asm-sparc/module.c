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
**    Copyright:
**    contact  :
*/

#include "aasm/modules.h"
#include "aasm/arch_id.h"

#ifdef AASM_STATIC
static const struct module_info_s	asm_sparc_module_info =
#else
const struct module_info_s		module_info =
#endif
{
  1,
  "asm-sparc",
  "Alexandre Becoulet 2003",
  "SPARC assembler",
  {0, 9, 1},
  ARCH_ID_SPARC,
  MODULE_CLASS_ASM,
  0
};

#ifdef AASM_STATIC

#include "asm.h"

struct module_s				asm_sparc_module =
{
  &asm_sparc_module_info,
  0,
  0,
  NULL,
  NULL,
  {
    {
      (void*)&asm_sparc_get_instr_perf,
      (void*)&asm_sparc_get_reg,
      (void*)&asm_sparc_process,
      (void*)&asm_sparc_reg_size,
      (void*)&asm_sparc_reg_name,
      (void*)&asm_sparc_instr_name,
      (void*)&asm_sparc_max_binary_size,
      (void*)&asm_sparc_instr_help,	/* optional */
      (void*)&asm_sparc_directive,	/* optional */
      (void*)&asm_sparc_endian_permut,
    }
  }
};

#endif

