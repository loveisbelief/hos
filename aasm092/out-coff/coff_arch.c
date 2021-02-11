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

#include <stdlib.h>

#include "aasm/error.h"
#include "aasm/expr.h"
#include "aasm/reloc.h"
#include "aasm/instr.h"
#include "aasm/arch_id.h"
#include "aasm/symbol.h"

#include "coff.h"

static unsigned int
coff_addend_i386	(struct out_object_s	*out_obj,
			 struct reloc_s		*rel)
{
  if ((rel->bits_size != 32) || (rel->bits_shift > 0))
    {
      error(ERRTYP_ERROR, "coff: non 32bits relocation on i386", (*rel->expr)->location);
      return (COFF_RELTYP_I386_NONE);
    }

  if (rel->flag & RELOC_TYPE_RELATIV)
    {
      switch (out_obj->flag & COFF_VARIANT_MASK)
	{
	case (COFF_VARIANT_GNU):
	  rel->flag |= RELOC_ADDEND | RELOC_ADDEND_VALUE;
	  rel->addend = rel->addend
	    - rel->instr->next->offset /* relativ gnu reloc */
	    + 4; /* cancel default x86 sizeof(4 bytes) addend */
	  break;

	case (COFF_VARIANT_MS):
	  /* no addend needed here, just correct potential addend to
	     cancel default x86 sizeof(4 bytes) offset */
	  rel->addend += 4;
	  break;
	}

      return (COFF_RELTYP_I386_REL32);
    }
  else
    {
      switch (out_obj->flag & COFF_VARIANT_MASK)
	{
	  struct expr_s		*expr;
	  struct out_symbol_s	*out_sym;

	case (COFF_VARIANT_GNU):
	  expr = *rel->expr;	/* shortcut */
	  if ((expr->flag & EXPR_TYPE_SYMBOL)
	      && (out_sym = expr->val.sym->out_sym))
	    {
	      /* use relocation addend to
		 express symbol offset */
	      rel->flag |= RELOC_ADDEND | RELOC_ADDEND_VALUE;
	      rel->addend += out_sym->offset;
	      break;
	    }

	case (COFF_VARIANT_MS):
	  /* symbol offset is read from
	     symbol table entry: no addend */
	  break;
	}

      return (COFF_RELTYP_I386_DIR32);
    }
}

static unsigned int
coff_addend_m68k	(struct out_object_s	*out_obj,
			 struct reloc_s		*rel)
{
  return (0);
}

const struct out_arch_s	*
coff_get_arch		(struct arch_s		*arch)
{
  static const struct out_arch_s	archs[2] =
    {
      /* i386 */
      {
	&coff_write_ulsb16, &coff_write_ulsb32,
	&coff_addend_i386,
	COFF_MAGIC_I386
      },

      /* 68000 */
      {
	&coff_write_umsb16, &coff_write_umsb32,
	&coff_addend_m68k,
	COFF_MAGIC_M68K
      },
    };

  switch (arch->id)
    {
    case (ARCH_ID_X86):
      return (&archs[0]);
      /*
    case (ARCH_ID_M68K):
      return (&archs[1]);
      */
    default:
      return (NULL);
    }
}

