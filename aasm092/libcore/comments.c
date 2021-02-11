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

#include "aasm/comments.h"
#include "aasm/memory.h"
#include "aasm/instr.h"
#include "aasm/scope.h"

/* Memory management for instr */
static struct mem_s	comment_pool_g;

static const struct mem_desc_s	comment_mem_desc =
  {
    "comment",
    NULL
  };

void
comment_clean (void)
{
  mem_clean (&comment_pool_g);
}

void
comment_init (void)
{
  mem_init (&comment_pool_g,
	    sizeof (struct comment_s),
	    &comment_mem_desc);
}

void	add_instr_comment (const char *str, struct instr_s *instr)
{
  struct comment_s* new = mem_pop (&comment_pool_g);

  new->str = xstrdup (str);
  new->next = instr->comments;
  instr->comments = new;
}

void	del_instr_comment (struct instr_s *instr)
{
  struct comment_s* tmp;
  struct comment_s* i;

  for (i = instr->comments; i != NULL; i = tmp)
    {
      tmp = i->next;
      dup_free (tmp->str);
      mem_push (i, &comment_pool_g);
    }

  instr->comments = NULL;
}

void	add_scope_comment (const char *str,
			   struct scope_s *scope)
{
  struct comment_s* new = mem_pop (&comment_pool_g);

  new->str = xstrdup (str);
  new->next = scope->comments;
  scope->comments = new;
}

void	del_scope_comment (struct scope_s *scope)
{
  struct comment_s* tmp;
  struct comment_s* i;

  for (i = scope->comments; i != NULL; i = tmp)
    {
      tmp = i->next;
      dup_free (tmp->str);
      mem_push (i, &comment_pool_g);
    }

  scope->comments = NULL; 
}
