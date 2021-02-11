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

#ifndef ERROR_H_
# define ERROR_H_

# include <stdio.h>

# define ERROR_MAX_SUBMSG	4
# define ERROR_MAX_DISPBUF	256

# define ERRTYP_ERROR_MASK	(ERRTYP_ERROR)
# define ERRTYP_WARN_MASK	(ERRTYP_WARN | ERRTYP_WARN_OPT\
				| ERRTYP_WARN_MORE | ERRTYP_WARN_CTRL)

# define ERRTYP_NONE		0x0
# define ERRTYP_ERROR		0x1
# define ERRTYP_WARN		0x2
# define ERRTYP_WARN_MORE	0x4
# define ERRTYP_WARN_OPT	0x8
# define ERRTYP_WARN_CTRL	0x10

# define ERRTYP_WARN_ALL	ERRTYP_WARN_MASK

# define ERROR_WARN_LIMIT	100

struct				error_locat_s
{
  const char			*filename;
  union
  {
    unsigned int		line;
    unsigned int		addr;
  }				pos;
};

struct				error_s
{
  unsigned int		type;

  const char			*msg;
  const char			*submsg[ERROR_MAX_SUBMSG];
  unsigned int			submsg_count;

  struct error_locat_s		location;
  struct error_s		*next;
};

extern struct error_s		*error_g;
extern unsigned int		error_count_g;
extern unsigned int		warning_count_g;

extern unsigned int		error_mask_g;

void				error_init	(void);
void				error_clean	(void);
void				error_destroy	(void);

struct error_s			*error		(unsigned int			type,
						 const char			*msg,
						 const struct error_locat_s	*location);

struct error_s			*error_submsg	(struct error_s			*err,
						 const char			*submsg);

struct error_s			*error_sublocation(struct error_s		*err,
						   const struct error_locat_s	*loc);

struct error_s			*error_subint(struct error_s			*err,
					      int				val);

void				error_disp	(FILE				*out);

void				error_set_locat	(struct error_locat_s		*location);

struct error_locat_s		*dup_location(const struct error_locat_s	*location);
struct error_locat_s		*location_newref (struct error_locat_s		*location);

void				location_init (void);
void				location_clean (void);
void				free_location (struct error_locat_s* location);

const struct error_locat_s	*get_location(void);

#endif /* !ERROR_H_ */



