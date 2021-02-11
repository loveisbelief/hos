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
#include "aasm/object.h"
#include "aasm/error.h"
#include "coff.h"
static void
coff_directive_variant	(struct object_s	*obj,
			 const char		*param)
{
  if (obj->out_obj->flag & COFF_VARIANT_MASK)
    {
      error(ERRTYP_ERROR, "coff: format variant already defined", 0);
      return;
    }
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('g'):
   if (!strncmp("nu", param + 1, 2))
   {
      switch (param[3])
      {
       case ('\x20'):
       case ('\t'):
       case ('\0'):
        param += 3;
        {
           obj->out_obj->flag |= COFF_VARIANT_GNU;
          goto param_lex_end;
        }
      }
   }
  break;

 case ('m'):
   if (!strncmp("icrosoft", param + 1, 8))
   {
                  switch (param[9])
                  {
                   case ('\x20'):
                   case ('\t'):
                   case ('\0'):
                    param += 9;
                    {
                       obj->out_obj->flag |= COFF_VARIANT_MS;
                      goto param_lex_end;
                    }
                  }
   }
  break;

}
   error(ERRTYP_ERROR, "coff: unknown format variant", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
void
out_coff_directive	(struct object_s	*obj,
			 const char		*param)
{
param += strspn(param, "\x20\t\0");
  /** WLEX begin **/

switch (param[0])
{
 case ('n'):
   if (!strncmp("otime", param + 1, 5))
   {
            switch (param[6])
            {
             case ('\x20'):
             case ('\t'):
             case ('\0'):
              param += 6;
              {
                 obj->out_obj->flag |= COFF_NOTIME;
                goto param_lex_end;
              }
            }
   }
  break;

 case ('v'):
   if (!strncmp("ariant", param + 1, 6))
   {
              switch (param[7])
              {
               case ('\x20'):
               case ('\t'):
               case ('\0'):
                param += 7;
                {
                   coff_directive_variant(obj, param);
                  goto param_lex_end;
                }
              }
   }
  break;

}
   error(ERRTYP_ERROR, "coff: unknown .mod_out directive", 0);
param_lex_end:

  /** WLEX end **/

   return;
}
