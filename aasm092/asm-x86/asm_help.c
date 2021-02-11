/*
**    This file is part of AASM.
**
**    AASM is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
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

#include <stdio.h>

#include "flags.h"
#include "asm.h"

int			asm_x86_instr_help(const struct instr_info_s	*info,
					   FILE				*out)
{
  unsigned int		i = 0;

  fprintf(out,
	  "-------------------------------------------------------------------------------\n"
	  "                              %s instruction help\n"
	  "-------------------------------------------------------------------------------\n\n",
	  info->name);
  
  fprintf(out, "\n");

  while (info[i].asm_instr)
    {
      unsigned int	j;
      unsigned int	max;

      fprintf(out, "  desc      %s\n", info[i].desc);

      max = info[i].i_flag & I_OP_COUNT_MASK;

      fprintf(out, "  instr set ");
      if (info[i].i_flag & I_386O)
	fprintf(out, "i386 ");
      if (info[i].i_flag & I_486O)
	fprintf(out, "i486 ");
      if (info[i].i_flag & I_586O)
	fprintf(out, "i586 ");
      if (info[i].i_flag & I_686O)
	fprintf(out, "i686 ");
      if (info[i].i_flag & I_FPU)
	fprintf(out, "fpu ");
      if (info[i].i_flag & I_MMX)
	fprintf(out, "mmx ");
      if (info[i].i_flag & I_MMX_EXT)
	fprintf(out, "mmx2 ");
      if (info[i].i_flag & I_3DNOW)
	fprintf(out, "3dnow ");
      if (info[i].i_flag & I_3DNOW_EXT)
	fprintf(out, "3dnow2 ");
      if (info[i].i_flag & I_SSE)
	fprintf(out, "sse ");
      if (info[i].i_flag & I_SSE2)
	fprintf(out, "sse2 ");

      fprintf(out, "\n");

      {
	unsigned int	k = 1, l;

	fprintf(out, "  opcode    ");

	if (info[i].i_flag & I_PREBYTE)
	  k++;
	if (info[i].i_flag & I_PREBYTE_INSTR)
	  k++;
	if (info[i].i_flag & I_SUFIXE)
	  k++;
	if (info[i].i_flag & I_PREFIXE_SSE)
	  k++;
	
	for (l = 0; l < k; l++)
	  fprintf(out, "0x%02x ", info[i].fix[l]);
      }

      fprintf(out, "\n  %-9s ", info[i].name);

      for (j = 0; j < max; j++)
	{
	  unsigned int	i_size = info[i].i_size[j];
	  unsigned int	i_type = info[i].i_type[j];

	  if (j > 0)
	    fprintf(out, ", ");

	  if (i_size & SIZE_FLAG_BYTE)
	    fprintf(out, "byte ");
	  if (i_size & SIZE_FLAG_WORD)
	    fprintf(out, "word ");
	  if (i_size & SIZE_FLAG_DWORD)
	    fprintf(out, "dword ");
	  if (i_size & SIZE_FLAG_FWORD)
	    fprintf(out, "fword ");
	  if (i_size & SIZE_FLAG_QWORD)
	    fprintf(out, "qword ");
	  if (i_size & SIZE_FLAG_TWORD)
	    fprintf(out, "tbyte ");
	  if (i_size & SIZE_FLAG_OWORD)
	    fprintf(out, "oword ");

	  fprintf(out, "| ");

	  if (i_type & OP_TYPE_REGISTER)
	    {
	      if (i_type & OP_TYPE_GENERAL)
		fprintf(out, "reg ");
	      else
		{
		  if (i_type & OP_TYPE_ACCUMULATOR)
		    fprintf(out, "al/ax/eax ");

		  if (i_type & OP_TYPE_COUNTER)
		    fprintf(out, "cl ");

		  if (i_type & OP_TYPE_DATA)
		    fprintf(out, "dx ");
		}

	      if (i_type & OP_TYPE_SEG_CS)
		fprintf(out, "cs ");

	      if (i_type & OP_TYPE_SEG_NEW)
		fprintf(out, "fs gs ");

	      if (i_type & OP_TYPE_SEG_OLD)
		fprintf(out, "ss ds es ");

	      if (i_type & OP_TYPE_CONTROL)
		fprintf(out, "cr? ");

	      if (i_type & OP_TYPE_DEBUG)
		fprintf(out, "dr? ");

	      if (i_type & OP_TYPE_MMX)
		fprintf(out, "mm? ");

	      if (i_type & OP_TYPE_SSE)
		fprintf(out, "xmm? ");

	      if (i_type & OP_TYPE_FPU_TOP)
		fprintf(out, "st0 ");

	      if (i_type & OP_TYPE_FPU_TAIL)
	      fprintf(out, "st? ");
	    }

	  if (i_type & OP_TYPE_MEM)
	    fprintf(out, "[address] ");

	  if (i_type & OP_TYPE_IM)
	    fprintf(out, "imm value ");
	}

      printf("\n\n");

      i++;
    }

  printf("\n");

  return (0);
}
