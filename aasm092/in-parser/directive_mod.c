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

#include "aasm/error.h"
#include "aasm/object.h"

#include "scan.h"
#include "directive.h"

void		directive_out(const char		*param,
			     struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%l#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .mod_out directive", 0);
      return;
    }

  if (!obj->msc.cl.nm.out->call.out.directive)
    {
      error(ERRTYP_ERROR, ".mod_out directive not supported by output module", 0);
      return;
    }

  obj->msc.cl.nm.out->call.out.directive(obj, str);

  return;
}

void		directive_asm(const char		*param,
			     struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%l#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .mod_asm directive", 0);
      return;
    }

  if (!obj->msc.cl.nm.asm_->call.asm_.directive)
    {
      error(ERRTYP_ERROR, ".mod_asm directive not supported by assembler module", 0);
      return;
    }

  obj->msc.cl.nm.asm_->call.asm_.directive(obj, str);

  return;
}

void		directive_mod_load(const char		*param,
				     struct object_s	*obj)
{
  char			str[MAX_LINE_LEN];
  const char		*end;

  if (!scan_store(param, &end, "#%w#", str) || *end)
    {
      error(ERRTYP_ERROR, "invalid use of .mod_load directive", 0);
      return;
    }

  module_load(&obj->msc, str);

  return;
}

