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
#include "aasm/arch_id.h"

#include "elf.h"

/*
** 32 & 64 bits architectures follow
*/

static unsigned int
elf_reltype_sparc	(struct reloc_s		*rel)
{
  if (rel->flag & RELOC_TYPE_RELATIV)
    {
      /* relativ relocations */
      switch (rel->bits_shift)
	{
	case (0):
	  /* not shifted values */
	  switch (rel->bits_size)
	    {
	    case (8):  return (R_SPARC_DISP8);
	    case (16): return (R_SPARC_DISP16);
	    case (32): return (R_SPARC_DISP32);
	    case (22): return (R_SPARC_WDISP22);
	    case (30): return (R_SPARC_WDISP30);
	    }
	}
    }
  else
    {
      /* direct relocations */
      switch (rel->bits_shift)
	{
	case (0):
	  /* not shifted values */
	  switch (rel->bits_size)
	    {
	    case (8):  return (R_SPARC_8);
	    case (10): return (R_SPARC_LO10);
	    case (16): return (R_SPARC_16);
	    case (32): return (R_SPARC_32);
	    case (13): return (R_SPARC_13);
	    case (22): return (R_SPARC_22);
	    }
	case (-10):
	  if (rel->bits_size == 22) return (R_SPARC_HI22);
	}
    }

  {
    struct error_s	*err;

    err = error(ERRTYP_ERROR, "elf: unhandled relocation on sparc (%s bits, %s bits shifted)",
		(*rel->expr)->location);
    error_subint(err, (signed)rel->bits_size);
    error_subint(err, rel->bits_shift);
  }

  return (R_SPARC_NONE);
}

#ifdef	ELF_64BITS

/*
** 64 bits only architectures follow
*/

static unsigned int
elf_reltype_fcpu	(struct reloc_s		*rel)
{
  if ((rel->bits_size != 64) || (rel->bits_shift > 0))
    {
      error(ERRTYP_ERROR, "elf: unhandled F-CPU relocation", (*rel->expr)->location);
      return (R_FCPU_NONE);
    }

  return ((rel->flag & RELOC_TYPE_RELATIV) ? R_FCPU_PC17 : R_FCPU_LE_64);
}

#else

/*
** 32 bits only architectures follow
*/

static unsigned int
elf_reltype_i386	(struct reloc_s		*rel)
{
  if ((rel->bits_size != 32) || (rel->bits_shift > 0))
    {
      error(ERRTYP_ERROR, "elf: non 32bits relocation on i386", (*rel->expr)->location);
      return (R_386_NONE);
    }

  return ((rel->flag & RELOC_TYPE_RELATIV) ? R_386_PC32 : R_386_32);
}

#endif

const struct out_arch_s	*
elf_get_arch			(struct arch_s		*arch)
{
#ifndef	ELF_64BITS
  static const struct out_arch_s	archs[3] = /* update count for elf64 module */
#else
  static const struct out_arch_s	archs[3] = /* update count for elf32 module */
#endif
    {
      /*
      ** 32 & 64 bits architectures follow
      */

#define INDEX_SPARC_V8		0	/* index in list */
      /* sparc v8 */
      {
	&elf_write_umsb16,
	&elf_write_umsb32, &elf_write_umsb64,
	&elf_write_smsb32, &elf_write_smsb64,
	ELFDATA2MSB, EM_SPARC, &elf_reltype_sparc,
	ELF_ARCHFLG_RELA
      },

#define INDEX_SPARC_V9		(INDEX_SPARC_V8 + 1)	/* index in list */
      /* sparc v9 */
      {
	&elf_write_umsb16,
	&elf_write_umsb32, &elf_write_umsb64,
	&elf_write_smsb32, &elf_write_smsb64,
	ELFDATA2MSB, EM_SPARCV9, &elf_reltype_sparc,
	ELF_ARCHFLG_RELA
      },

#define INDEX_LAST32_64		INDEX_SPARC_V9

#ifdef ELF_64BITS

      /*
      ** 64 bits only architectures follow
      */

#define INDEX_FCPU		(INDEX_LAST32_64 + 1)	/* index in list */

      /* 1 F-CPU */
      {
	&elf_write_umsb16,
	&elf_write_umsb32, &elf_write_umsb64,
	&elf_write_smsb32, &elf_write_smsb64,
	ELFDATA2MSB, EM_FCPU, &elf_reltype_fcpu,
	ELF_ARCHFLG_NONE
      },

#else

      /*
      ** 32 bits only architectures follow
      */

#define INDEX_X86		(INDEX_LAST32_64 + 1)	/* index in list */

      /* 1 x86 */
      {
	&elf_write_ulsb16,
	&elf_write_ulsb32, &elf_write_ulsb64,
	&elf_write_slsb32, &elf_write_slsb64,
	ELFDATA2LSB, EM_386, &elf_reltype_i386,
	ELF_ARCHFLG_NONE
      },

#endif

    };

  switch (arch->id)
    {

      /*
      ** 32 & 64 bits architectures follow
      */

    case (ARCH_ID_SPARC):
      switch (arch->sub_id)
	{
#ifndef ELF_64BITS
	default:
#endif
	case (ARCH_SID_SPARC_V8): return (&archs[INDEX_SPARC_V8]);
#ifdef ELF_64BITS
	default:
#endif
	case (ARCH_SID_SPARC_V9): return (&archs[INDEX_SPARC_V9]);
	}

#ifdef ELF_64BITS

      /*
      ** 64 bits only architectures follow
      */

    case (ARCH_ID_FCPU):
      return (&archs[INDEX_FCPU]);

#else

      /*
      ** 32 bits only architectures follow
      */

    case (ARCH_ID_X86):
      return (&archs[INDEX_X86]);

#endif

    default:
      return (NULL);
    }
}

