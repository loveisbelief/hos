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

#ifndef MOD_IN_IMPORT_H_
# define MOD_IN_IMPORT_H_

# define CURRENT_IN_API_VERSION	1

struct				object_s;

typedef int			in_open_t		(const char		*filename);

typedef int			in_process_t		(struct object_s	*obj);

typedef void			in_close_t		(void);

typedef struct error_locat_s *	in_get_location_t	(void);

struct				module_in_calls_s
{
  in_open_t			*open;
  in_process_t			*process;
  in_close_t			*close;
  in_get_location_t		*get_location;
};

# define MOD_IN_LOAD_LIST	\
{\
  {"open", 0},\
  {"process", 0},\
  {"close", 0},\
  {"get_location", 0},\
  {NULL, 0}\
}

#define MOD_IN_LOAD_LIST_SIZE	4

#endif

