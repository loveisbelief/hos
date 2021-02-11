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

#include "aasm/size.h"
#include "aasm/object.h"
#include "aasm/error.h"

#include "asm.h"

/*
** EXPORTED FUNCTION
** Return CPU endian permutation table
*/

/*
** EXPORTED FUNCTION
** Return instruction name from info structure
*/

const char		*
asm_sparc_instr_name	(const struct instr_info_s	*instr)
{
  return (instr->name);
}

/*
** EXPORTED FUNCTION
** Return instruction max binary size (used for memory allocation)
*/

unsigned int
asm_sparc_max_binary_size(const struct instr_info_s	*instr)
{
  return (4);			/* UPDATE THIS LINE */
}

