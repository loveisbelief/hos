/* -*- c -*-*/
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
**    Copyright:
**    contact  :
*/
#include <string.h>
#include "aasm/object.h"
#include "aasm/section.h"
#include "aasm/error.h"
#include "aasm/arch_id.h"
#include "fields.h"
#include "asm.h"
static void
asm_sparc_directive_arch(struct object_s		*obj,
			 const char			*param)
{
  struct section_s	*sec = obj->cur_section;
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

 if (param[0] == 'v')
 {
  switch (param[1])
  {
   case ('8'):
    switch (param[2])
    {
     case ('\x20'):
     case ('\t'):
     case ('\0'):
      param += 2;
      {
         sec->asm_flag &= ~FLG_MASK;
         sec->asm_flag |= FLG_V8;
        goto param_lex_end;
      }
    }
    break;

   case ('9'):
    switch (param[2])
    {
     case ('\x20'):
     case ('\t'):
     case ('\0'):
      param += 2;
      {
         sec->asm_flag &= ~FLG_MASK;
         sec->asm_flag |= FLG_V9;
        goto param_lex_end;
      }
    }
    break;

  }
 }
  error(ERRTYP_ERROR, "asm_sparc: unknown sparc architecture", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
void
asm_sparc_directive	(struct object_s		*obj,
			 const char			*param)
{
  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "x86: no section defined before .mod_asm directive", 0);
      return;
    }
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

 if (!strncmp("opcodes", param + 0, 7))
 {
              switch (param[7])
              {
               case ('\x20'):
               case ('\t'):
               case ('\0'):
                param += 7;
                {
                   asm_sparc_directive_arch(obj, param);
                  goto param_lex_end;
                }
              }
 }
  error(ERRTYP_ERROR, "asm_sparc: unknown .mod_asm directive for sparc module", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
