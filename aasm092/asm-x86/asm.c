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

#include <string.h>

#include "aasm/instr.h"
#include "aasm/object.h"
#include "aasm/error.h"
#include "aasm/section.h"

#include "flags.h"
#include "asm.h"

const char		*asm_x86_instr_name(const struct instr_info_s	*instr)
{
  return (instr->name);
}

unsigned int		asm_x86_max_binary_size(const struct instr_info_s	*instr)
{
  return (X86_MAX_INSTR_SIZE);
}

