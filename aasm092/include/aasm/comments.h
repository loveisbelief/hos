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
**    Copyright Cedric BAIL, 2002
**    contact : cedric.bail@epita.fr
*/

#ifndef COMMENT_H_
# define COMMENT_H_

struct			instr_s;
struct			scope_s;

struct comment_s
{
  char			*str;
  struct comment_s	*next;
};

void			comment_clean		(void);
void			comment_init		(void);

void			add_instr_comment	(const char	*str,
						 struct instr_s	*instr);

void			del_instr_comment	(struct instr_s	*instr);

void			add_scope_comment	(const char	*str,
						 struct scope_s	*scope);

void			del_scope_comment	(struct scope_s	*scope);

#endif
