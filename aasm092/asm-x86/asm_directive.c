/* -*- c -*- */
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
#include <string.h>
#include "aasm/instr.h"
#include "aasm/object.h"
#include "aasm/error.h"
#include "aasm/section.h"
#include "flags.h"
#include "asm.h"
static void	asm_x86_directive_cpu(struct section_s		*section,
				      const char		*param)
{
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('6'):
   if (!strncmp("86", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag &= ~I_ALL_X86;
           section->asm_flag |= I_686O;
          goto param_lex_end;
        }
      }
   }
  break;

 case ('4'):
   if (!strncmp("86", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag &= ~I_ALL_X86;
           section->asm_flag |= I_486O;
          goto param_lex_end;
        }
      }
   }
  break;

 case ('3'):
   if (!strncmp("86", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag &= ~I_ALL_X86;
           section->asm_flag |= I_386O;
          goto param_lex_end;
        }
      }
   }
  break;

 case ('5'):
   if (!strncmp("86", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag &= ~I_ALL_X86;
           section->asm_flag |= I_586O;
          goto param_lex_end;
        }
      }
   }
  break;

}
   error(ERRTYP_ERROR, "x86: unknown cpu", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
static void	asm_x86_directive_ext(struct section_s		*section,
				      const char		*param)
{
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('n'):
   if (!strncmp("one", param + 1, 3))
   {
        switch (param[4])
        {
         case ('\x20'):
         case ('\t'):
         case ('\0'):
          param += 4;
          {
             section->asm_flag &=	~I_ALL_EXT;
            goto param_lex_end;
          }
        }
   }
  break;

 case ('3'):
   if (!strncmp("dnow", param + 1, 4))
   {
          switch (param[5])
          {
           case ('\x20'):
           case ('\t'):
           case ('\0'):
            param += 5;
            {
               section->asm_flag |= I_3DNOW;
              goto param_lex_end;
            }
           case ('e'):
             if (!strncmp("xt", param + 6, 2))
             {
                switch (param[8])
                {
                 case ('\x20'):
                 case ('\t'):
                 case ('\0'):
                  param += 8;
                  {
                     section->asm_flag |= I_3DNOW_EXT;
                    goto param_lex_end;
                  }
                }
             }
            break;

          }
   }
  break;

 case ('m'):
   if (!strncmp("mx", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag |= I_MMX;
          goto param_lex_end;
        }
       case ('e'):
         if (!strncmp("xt", param + 4, 2))
         {
            switch (param[6])
            {
             case ('\x20'):
             case ('\t'):
             case ('\0'):
              param += 6;
              {
                 section->asm_flag |= I_MMX_EXT;
                goto param_lex_end;
              }
            }
         }
        break;

      }
   }
  break;

 case ('s'):
   if (!strncmp("se", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag |= I_SSE;
          goto param_lex_end;
        }
       case ('2'):
        switch (param[4])
        {
         case ('\x20'):
         case ('\t'):
         case ('\0'):
          param += 4;
          {
             section->asm_flag |= I_SSE2;
            goto param_lex_end;
          }
        }
        break;

      }
   }
  break;

 case ('f'):
   if (!strncmp("pu", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           section->asm_flag |= I_FPU;
          goto param_lex_end;
        }
      }
   }
  break;

}
   error(ERRTYP_ERROR, "x86: unknown cpu extension", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
static void	asm_x86_directive_mode(struct section_s		*section,
				       const char		*param)
{
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('1'):
   if (!strncmp("6bits", param + 1, 5))
   {
            switch (param[6])
            {
             case ('\x20'):
             case ('\t'):
             case ('\0'):
              param += 6;
              {
                 section->asm_flag |= ASM_SECTION_CODE16;
                 section->asm_flag &= ~ASM_SECTION_CODE32;
                goto param_lex_end;
              }
            }
   }
  break;

 case ('3'):
   if (!strncmp("2bits", param + 1, 5))
   {
            switch (param[6])
            {
             case ('\x20'):
             case ('\t'):
             case ('\0'):
              param += 6;
              {
                 section->asm_flag |= ASM_SECTION_CODE32;
                 section->asm_flag &= ~ASM_SECTION_CODE16;
                goto param_lex_end;
              }
            }
   }
  break;

}
   error(ERRTYP_ERROR, "x86: unknown cpu mode, only 16bits/32bits are valid modes", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
void		asm_x86_directive(struct object_s	*obj,
				  const char		*param)
{
  if (!obj->cur_section)
    {
      error(ERRTYP_ERROR, "x86: no section defined before .mod_asm directive", 0);
      return;
    }
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('e'):
   if (!strncmp("xt", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           asm_x86_directive_ext(obj->cur_section, param);
          goto param_lex_end;
        }
      }
   }
  break;

 case ('c'):
   if (!strncmp("pu", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           asm_x86_directive_cpu(obj->cur_section, param);
          goto param_lex_end;
        }
      }
   }
  break;

 case ('m'):
   if (!strncmp("ode", param + 1, 3))
   {
        switch (param[4])
        {
         case ('\x20'):
         case ('\t'):
         case ('\0'):
          param += 4;
          {
             asm_x86_directive_mode(obj->cur_section, param);
            goto param_lex_end;
          }
        }
   }
  break;

}
  error(ERRTYP_ERROR, "x86: unknown .mod_asm directive for x86 module", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
