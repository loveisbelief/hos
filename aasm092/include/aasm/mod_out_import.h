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

#ifndef MOD_OUT_IMPORT_H_
# define MOD_OUT_IMPORT_H_

# define CURRENT_OUT_API_VERSION	1

struct				out_object_s;
struct				out_section_s;
struct				out_symbol_s;

struct				object_s;
struct				scope_s;
struct				symbol_s;
struct				section_s;

typedef int			out_write_t	(struct object_s	*obj);

typedef void			out_directive_t	(struct object_s	*obj,
						 const char		*param);

typedef struct out_object_s	*out_newobject_t (struct object_s	*obj);

typedef struct out_section_s	*out_newsection_t (struct object_s	*obj,
						   struct section_s	*sec);

typedef struct out_symbol_s	*out_newsymbol_t (struct object_s	*obj,
						  struct scope_s	*sc,
						  struct symbol_s	*sym);

typedef void			out_clean_t	(struct object_s	*obj);

struct				module_out_calls_s
{
  out_write_t			*write;

  out_directive_t		*directive;
  out_newobject_t		*newobject;
  out_newsection_t		*newsection;
  out_newsymbol_t		*newsymbol;
  out_clean_t			*clean;
};

# define MOD_OUT_LOAD_LIST	\
{\
  {"write", 0},\
  {"directive", MODULE_LOAD_OPTIONAL},\
  {"newobject", MODULE_LOAD_OPTIONAL},\
  {"newsection", MODULE_LOAD_OPTIONAL},\
  {"newsymbol", MODULE_LOAD_OPTIONAL},\
  {"clean", MODULE_LOAD_OPTIONAL},\
  {NULL, 0}\
}

# define MOD_OUT_LOAD_LIST_SIZE	6

#endif

