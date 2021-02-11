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

#include "aasm/modules.h"
#include "aasm/arch_id.h"

#ifdef AASM_STATIC
static const struct module_info_s	asm_x86_module_info =
#else
const struct module_info_s		module_info =
#endif
{
  1,
  "asm-x86",
  "Alexandre Becoulet 2002",
  "Generic x86 assembler",
  {0, 9, 1},
  ARCH_ID_X86,
  MODULE_CLASS_ASM,
  1
};

#ifdef AASM_STATIC

#include "asm.h"

struct module_s				asm_x86_module =
{
  &asm_x86_module_info,
  0,
  0,
  NULL,
  NULL,
  {
    {
      (void*)&asm_x86_get_instr_perf,
      (void*)&asm_x86_get_reg,
      (void*)&asm_x86_process,
      (void*)&asm_x86_reg_size,
      (void*)&asm_x86_reg_name,
      (void*)&asm_x86_instr_name,
      (void*)&asm_x86_max_binary_size,
      (void*)&asm_x86_instr_help,
      (void*)&asm_x86_directive,
      (void*)&asm_x86_endian_permut,
    }
  }
};

#endif

