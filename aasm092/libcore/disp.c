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

#include <stdio.h>
#include <unistd.h>

#include "aasm/disp.h"
#include "aasm/color.h"

static unsigned int	disp_flag_g = 0;

int		disp_color_set(char		**dummy)
{
  if (isatty(1) && isatty(2))
    disp_flag_g |= DISP_FLAG_COLOR;
  
  return (0);
}

int		disp_quiet_set(char		**dummy)
{
  disp_flag_g |= DISP_FLAG_QUIET;

  return (0);
}

void		disp_error(FILE			*out,
			   const char		*filename,
			   unsigned int		line,
			   const char		*msg)
{
  const char	*tmp;

  tmp = (disp_flag_g & DISP_FLAG_COLOR)
    ? COLOR_RED "error:" COLOR_STD : "error:";

  if (!filename)
    filename = "";

  fprintf(out, "%s:%i: %s %s\n",
	  filename, line, tmp, msg);
}

void		disp_warning(FILE		*out,
			     const char		*filename,
			     unsigned int	line,
			     const char		*msg)
{
  const char	*tmp;

  tmp = (disp_flag_g & DISP_FLAG_COLOR)
    ? COLOR_BROWN "warning:" COLOR_STD : "warning:";

  if (!filename)
    filename = "";

  fprintf(out, "%s:%i: %s %s\n",
	  filename, line, tmp, msg);
}

void		disp_action(FILE		*out,
			    const char		*text)
{
  if (disp_flag_g & DISP_FLAG_QUIET)
    return;

  fprintf(out, "%s: ", text);
  fflush(out);
}

void		disp_info(FILE			*out,
			  const char		*title,
			  const char		*text)
{
  if (disp_flag_g & DISP_FLAG_QUIET)
    return;

  if (disp_flag_g & DISP_FLAG_COLOR)
    fprintf(out, "["COLOR_GREEN"%s"COLOR_STD": %s] ",
	    title, text);
  else
    fprintf(out, "[%s: %s] ", title, text);
  fflush(out);
}

void		disp_action_end(FILE		*out)
{
  if (disp_flag_g & DISP_FLAG_QUIET)
    return;

  if (disp_flag_g & DISP_FLAG_COLOR)
    fprintf(out, "["COLOR_GREEN"Done"COLOR_STD"]\n");
  else
    fprintf(out, "[Done]\n");
}

void		disp_action_failed(FILE		*out)
{
  if (disp_flag_g & DISP_FLAG_QUIET)
    return;

  if (disp_flag_g & DISP_FLAG_COLOR)
    fprintf(out, "["COLOR_RED"Failed"COLOR_STD"]\n");
  else
    fprintf(out, "[Failed]\n");
}

void		disp_hex(aasm_u8_t		*buf,
			 unsigned int		len,
			 FILE			*file)
{
  while (len--)
    fprintf(file, "%02x ", *buf++);
}

