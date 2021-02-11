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
**    Copyright Cedric Bail, 2002
**    contact : cedric.bail@epita.fr
*/

#ifndef		MEMSTRUCT_H_
# define	MEMSTRUCT_H_

/*
** Internal structure for the allocator
*/

struct			mem_desc_s
{
  const char		*name;
  void			(*data_disp)(void *data);
};

struct			mem_s
{
  unsigned int		sizeof_struct;
  unsigned int		entry_used;
  unsigned int		list_used;
  unsigned int		list_max;

  unsigned int		free_used;
  unsigned int		free_max;

  void			**list;

  void			**free_list;

  const struct mem_desc_s	*desc;
};

struct			resize_mem_s
{
  unsigned int		sizeof_struct;

  unsigned int		list_used;
  unsigned int		list_max;

  unsigned int		free_used;
  unsigned int		free_max;

  void			**list;
  unsigned int		*allowed;

  void			**free_list;
  unsigned int		*freed_size;
};

#endif
