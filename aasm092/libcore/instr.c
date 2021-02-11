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

#include <string.h>
#include <assert.h>

#include "aasm/memory.h"
#include "aasm/instr.h"
#include "aasm/modules.h"
#include "aasm/error.h"
#include "aasm/expr.h"
#include "aasm/scope.h"
#include "aasm/section.h"
#include "aasm/comments.h"

void			instr_set_source(struct instr_s			*instr,
					 struct error_locat_s		*location,
					 unsigned int			type)
{
  assert(!(instr->flag & INSTR_SOURCE));

  instr->flag |= INSTR_SOURCE | type;
  instr->location = location;
}

void			instr_set_asm(struct instr_s		*instr,
				      unsigned int		len)
{
  instr->len = len;
  instr->flag |= INSTR_ASM;
}

int			instr_help(char				*name)
{
  struct instr_perf_s	*iperf;

  if (module_lock(&mod_gl_scope, MODULE_CLASS_ASM))
    return (-1);

  if (!mod_gl_scope.cl.nm.asm_->call.asm_.instr_help)
    {
      error(ERRTYP_ERROR, "no help provided by assembler module", 0);
      return (-1);
    }

  if (!(iperf = mod_gl_scope.cl.nm.asm_->call.asm_.get_instr_perf(name, strlen(name))))
    {
      error_submsg(error(ERRTYP_ERROR, "instruction `%s' not recognized by assembler module", 0), name);
      return (-1);
    }

  fprintf(stdout, "\n");
  mod_gl_scope.cl.nm.asm_->call.asm_.instr_help(iperf->info, stdout);

  return (0);
}

/* Memory management for instr */
static struct resize_mem_s instr_pool_g;

void instr_init (void) { mem_resize_init (&instr_pool_g, 
					  sizeof (struct instr_s)); }
void instr_clean (void) { mem_resize_clean (&instr_pool_g); }

struct instr_s		*instr_new(struct section_s		*sec,
				   struct scope_s		*scope,
				   const struct instr_info_s	*info,
				   unsigned int			bin_size,
				   unsigned int			op_count)
{
  struct instr_s	*instr;
  unsigned int		real = bin_size + op_count * sizeof (struct expr_s);

  instr = mem_resize_pop (&instr_pool_g, real);
  memset (instr, 0, sizeof(struct instr_s) + real);

  instr->info = info;
  instr->bin_size = bin_size;
  instr->op_count = op_count;

  instr->next = &scope->last_instr;
  instr->prev = scope->last_instr.prev;
  scope->last_instr.prev->next = instr;
  scope->last_instr.prev = instr;

  if (sec)
    instr->asm_flag = sec->asm_flag;

  instr->operands = (void*)((aasm_u8_t*)instr + sizeof (struct instr_s));
  instr->val.bin = (void*)((aasm_u8_t*)instr->operands + op_count * sizeof(struct expr_s *));

  return (instr);
}

struct instr_s		*instr_dup_src(struct instr_s		*instr,
				       struct instr_s		*prev,
				       struct error_locat_s	*location)
{
  unsigned int		i;
  struct instr_s	*new;
  unsigned int		real = instr->bin_size + sizeof (struct expr_s) * instr->op_count;

  new = mem_resize_pop (&instr_pool_g, real);
  memset (new, 0, sizeof(struct instr_s) + real);

  new->bin_size = instr->bin_size;
  new->op_count = instr->op_count;

  /* set binary data pointer */
  new->operands = (void*)((aasm_u8_t*)new + sizeof (struct instr_s));
  new->val.bin = (void*)((aasm_u8_t*)new->operands + instr->op_count * sizeof(struct expr_s *));

  /* update links to insert new instruction */
  new->prev = prev;
  new->next = prev->next;
  prev->next->prev = new;
  prev->next = new;

  /* copy instruction TYPE */
  new->flag = (instr->flag & INSTR_TYPE_MASK) | INSTR_SOURCE;

  /* copy instruction source info if needed */

  new->info = instr->info;

  for (i = 0; i < new->op_count; i++)
    new->operands[i] = expr_dup(instr->operands[i]);

  new->location = location;

  return (new);
}

/*
** This function free all instructions in scope
*/
void			instrs_free (struct scope_s	*scp)
{
  unsigned int		i;
  struct instr_s	*instr = scp->first_instr.next;
  struct instr_s	*next;

  while (instr && (next = instr->next))
    {
      instr->prev = NULL;
      instr->next = NULL;

      for (i = 0; i != instr->op_count; ++i)
	if (instr->operands[i]) 
	  expr_free (instr->operands[i]);

      instr->op_count = 0;

      del_instr_comment (instr);

      if (instr->flag & INSTR_SOURCE)
	free_location (instr->location);

      mem_resize_push (instr, &instr_pool_g);

      instr = next;
    }
}
