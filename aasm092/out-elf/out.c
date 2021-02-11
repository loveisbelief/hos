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

#include <unistd.h>

#include "aasm/object.h"
#include "aasm/disp.h"
#include "aasm/error.h"

#include "elf.h"

int		out_elf_write(struct object_s	*obj)
{
  char		filename[512];
  FILE		*file;
  int		res = -1;

  sprintf(filename, "%s.o", obj->name);
  disp_info(stdout, "writing", filename);

  if ((file = fopen(filename, "w+b")))
    {
      if (!(res = elf_write(obj, file)))
	{
	  fclose(file);
	  return (0);
	}
      unlink(filename);
      fclose(file);
    }

  if (res < 0)
    error_submsg(error(ERRTYP_ERROR,
		       "elf: output error, can't write `%s'",
		       NULL), filename);

  return (-1);
}

void		out_elf_directive(struct object_s	*obj,
				   const char		*param)
{
  error(ERRTYP_ERROR, "elf: unknown .mod_out directive", 0);
}

