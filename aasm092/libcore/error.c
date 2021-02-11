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

#include <stdio.h>
#include <string.h>

#include "aasm/error.h"
#include "aasm/memory.h"
#include "aasm/disp.h"
#include "aasm/modules.h"

static struct mem_s	error_pool_g;

static const struct mem_desc_s	error_mem_desc =
  {
    "error",
    NULL
  };

struct error_s	*error_g = NULL;

unsigned int	error_count_g = 0;
unsigned int	warning_count_g = 0;

unsigned int	error_mask_g = ERRTYP_ERROR | ERRTYP_WARN;

void		error_init (void)
{
  mem_init (&error_pool_g, sizeof (struct error_s), &error_mem_desc);
}

void		error_clean (void)
{
  mem_clean (&error_pool_g);
}

void		error_destroy (void)
{
    error_count_g = warning_count_g = 0;
    while (error_g)
    {
	int		i;
	struct error_s	*err;

	err = error_g;
	error_g = error_g->next;
	for (i = 0; i < err->submsg_count; i++)
	  dup_free((char*)err->submsg[i]);
	mem_push (err, &error_pool_g);
    }
}

struct error_s	*error(unsigned int			type,
		       const char			*msg,
		       const struct error_locat_s	*location)
{
  struct error_s	*err;
  struct error_s	*tmp;

  if (!(error_mask_g & type))
    return (NULL);

  if (type & ERRTYP_ERROR_MASK)
    error_count_g++;
  else
    {
      warning_count_g++;

      if (warning_count_g == ERROR_WARN_LIMIT)
	{
	  type = ERRTYP_WARN;
	  msg = "maximum warning count reached, further warning will be ignored";
	}

      if (warning_count_g > ERROR_WARN_LIMIT)
	return (NULL);
    }

  err = mem_pop (&error_pool_g);
  memset(err, 0, sizeof(struct error_s));
  err->msg = msg;
  err->type = type;

  if (location)
    memcpy(&err->location, location, sizeof (struct error_locat_s));
  else
    memcpy(&err->location, get_location(), sizeof (struct error_locat_s));

  if ((tmp = error_g))
    {
      while (tmp->next)
	tmp = tmp->next;
      tmp->next = err;
    }
  else
    error_g = err;

  return (err);
}

struct error_s	*error_submsg(struct error_s	*err,
			      const char	*submsg)
{
  if (err)
    {
      if (err->submsg_count < ERROR_MAX_SUBMSG)
	err->submsg[err->submsg_count++] = xstrdup(submsg);
    }

  return (err);
}

struct error_s	*error_sublocation(struct error_s		*err,
				   const struct error_locat_s	*loc)
{
  if (err)
    {
      char	loc_txt[512];

      sprintf(loc_txt, "%s:%u", loc->filename, loc->pos.line);

      if (err->submsg_count < ERROR_MAX_SUBMSG)
	err->submsg[err->submsg_count++] = xstrdup(loc_txt);
    }

  return (err);
}

struct error_s	*error_subint(struct error_s			*err,
			      int				val)
{
  if (err)
    {
      char	loc_txt[32];

      sprintf(loc_txt, "%i", val);

      if (err->submsg_count < ERROR_MAX_SUBMSG)
	err->submsg[err->submsg_count++] = xstrdup(loc_txt);
    }

  return (err);
}

static void	error_line_sort(void)
{
  int				done = 1;

  while (done)		/* bubble sort on error list */
    {
      struct error_s		*tmp, **prev;

      done = 0;
      prev = &error_g;
      tmp = error_g;
 
      while (tmp && tmp->next)
        {
          if ((tmp->location.filename == tmp->next->location.filename)
	      && (tmp->location.pos.line > tmp->next->location.pos.line))
            {
	      struct error_s	*tmp2;

              done++;
              *prev = tmp->next;
              prev = &tmp->next->next;
              tmp2 = tmp->next->next;
              tmp->next->next = tmp;
              tmp->next = tmp2;
            }
          else
            {
              prev = &tmp->next;
              tmp = tmp->next;
            }
        }
    }
}

void		error_disp(FILE		*out)
{
  struct error_s	*err;

  error_line_sort();

  err = error_g;
  while (err)
    {
      char		disp[ERROR_MAX_DISPBUF + 1];

      /* forge error message text */
      disp[ERROR_MAX_DISPBUF] = 0;
      sprintf(disp, err->msg,
	       err->submsg[0], err->submsg[1],
	       err->submsg[2], err->submsg[3]);

      /* display error/warning */
      if (err->type == ERRTYP_ERROR)
	disp_error(out, err->location.filename,
		   err->location.pos.line, disp);
      else
	disp_warning(out, err->location.filename,
		     err->location.pos.line, disp);

      err = err->next;
    }
}

const struct error_locat_s	*get_location(void)
{
  static const struct error_locat_s	empty = { NULL, {0} };

  if (mod_gl_scope.cl.nm.in)
    return (mod_gl_scope.cl.nm.in->call.in.get_location());
  else
    return (&empty);
}

void		error_set_locat(struct error_locat_s	*location)
{
  memcpy(location, get_location(), sizeof (struct error_locat_s));
}
