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

#ifdef __DJGPP__
# include <stdio.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <unistd.h>
#endif

#include "aasm/modules.h"

#include "directive.h"
#include "parse.h"
#include "scan.h"

void		directive_include(const char		*param,
				  struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;
  unsigned int		i;

  if (!scan_store(param, &end, "#%w#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .include directive", 0);
      return;
    }

  for (i = 0; i < include_path_depth; i++)
    {
      char		filepath[MAX_LINE_LEN];
#ifdef __DJGPP__
      FILE		*test;
#else
      struct stat	st;
#endif

      sprintf(filepath, "%s/%s", include_path[i], str);

#ifdef __DJGPP__
      if (!(test = fopen(filepath, "r")))
	continue;
      else
	fclose(test);
#else
      if (stat(filepath, &st))
	continue;

      if (!S_ISREG(st.st_mode))
	{
	  error_submsg(error(ERRTYP_ERROR, "can't include `%s', invalid file type", 0),
		       filepath);
	  return;
	}
#endif

      parse_open(filepath);

      return;
    }

  error_submsg(error(ERRTYP_ERROR, "can't include `%s', file not found", 0),
	       str);

  return;
}

