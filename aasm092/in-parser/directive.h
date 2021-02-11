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

#ifndef DIRECTIV_H_
# define DIRECTIV_H_

struct				object_s;

# define MAX_DB_LEN		256
# define MAX_RESERVE_LEN	16384
# define MAX_FILL_LEN		16384
# define DIRECTIV_PARAMS	const char		*param,\
				struct object_s		*obj

typedef void			(*directive_fct_t)(DIRECTIV_PARAMS);

struct				directive_s
{
  const char			*name;
  directive_fct_t		fct;
  unsigned int			state_mask; /* parser state mask */
};

struct directive_s		*in_directive_set(const char	*str,
						 unsigned int	len);

void		directive_objname	(DIRECTIV_PARAMS);
void		directive_objdesc	(DIRECTIV_PARAMS);
void		directive_objcopyright	(DIRECTIV_PARAMS);
void		directive_objbase	(DIRECTIV_PARAMS);
void		directive_debug		(DIRECTIV_PARAMS);
void		directive_define	(DIRECTIV_PARAMS);
void		directive_align		(DIRECTIV_PARAMS);
void		directive_label		(DIRECTIV_PARAMS);
void		directive_extern	(DIRECTIV_PARAMS);
void		directive_export	(DIRECTIV_PARAMS);
void		directive_include	(DIRECTIV_PARAMS);
void		directive_warning	(DIRECTIV_PARAMS);
void		directive_error		(DIRECTIV_PARAMS);
void		directive_section	(DIRECTIV_PARAMS);
void		directive_sectionbase	(DIRECTIV_PARAMS);
void		directive_sectionalign	(DIRECTIV_PARAMS);
void		directive_section_end	(DIRECTIV_PARAMS);
void		directive_ifdef		(DIRECTIV_PARAMS);
void		directive_if		(DIRECTIV_PARAMS);
void		directive_ifndef	(DIRECTIV_PARAMS);
void		directive_else		(DIRECTIV_PARAMS);
void		directive_endif		(DIRECTIV_PARAMS);
void		directive_string	(DIRECTIV_PARAMS);
void		directive_db		(DIRECTIV_PARAMS);
void		directive_fill		(DIRECTIV_PARAMS);
void		directive_pad		(DIRECTIV_PARAMS);
void		directive_reserve	(DIRECTIV_PARAMS);
void		directive_dump		(DIRECTIV_PARAMS);
void		directive_proc		(DIRECTIV_PARAMS);
void		directive_macro		(DIRECTIV_PARAMS);
void		directive_proc_end	(DIRECTIV_PARAMS);
void		directive_macro_end	(DIRECTIV_PARAMS);
void		directive_out		(DIRECTIV_PARAMS);
void		directive_asm		(DIRECTIV_PARAMS);
void		directive_mod_load	(DIRECTIV_PARAMS);

#endif
