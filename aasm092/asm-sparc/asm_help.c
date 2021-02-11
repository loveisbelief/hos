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

#include <stdio.h>

#include "asm.h"

/*
** EXPORTED FUNCTION (Optional)
** Display help information about instruction
*/

int
asm_sparc_instr_help	(const struct instr_info_s	*info,
			 FILE				*out)
{
  fprintf(out, "%s instruction quick help:\n", info->name);

  /* INSTRUCTION QUICK HELP DISPLAY CODE HERE */

  fprintf(out, "No information available.\n");

  return (0);
}

