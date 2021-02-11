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
#include "aasm/error.h"
#include "aasm/disp.h"

#include "binary.h"

int		out_binary_write(struct object_s	*obj)
{
  FILE		*file;
  char		filename[512];

  /* write raw binary file */
  sprintf(filename, "%s.bin", obj->name);

  disp_info(stdout, "writing", filename);

  if ((file = fopen(filename, "w+b")))
    {
      if (binary_write(obj, file))
	{
	  fclose(file);
	  unlink(filename);
	  return (-1);
	}
      fclose(file);
    }
  else
    goto out_write_err;
  
  /* write symbol list file */
  sprintf(filename, "%s.sym", obj->name);

  disp_info(stdout, "writing", filename);

  if ((file = fopen(filename, "w+")))
    {
      if (binary_write_sym(obj, file))
	{
	  fclose(file);
	  unlink(filename);
	  return (-1);
	}
      fclose(file);

      return (0);
    }

 out_write_err:
  error_submsg(error(ERRTYP_ERROR,
		     "binary: output error, can't write `%s'",
		     NULL), filename);
  return (-1);
}

/*
void		out_binary_directive(struct object_s	*obj,
				    const char		*param)
{
  error(ERRTYP_ERROR, "binary: unknown .out directive", 0);
}

*/
