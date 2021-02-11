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

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aasm/object.h"
#include "aasm/proc.h"
#include "aasm/expr.h"
#include "aasm/reloc.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/macro.h"
#include "aasm/comments.h"
#include "aasm/alloc.h"
#include "aasm/disp.h"
#include "aasm/error.h"
#include "aasm/modules.h"
#include "aasm/info.h"

#include "args.h"

static int
mod_use		(char		**name)
{
  if (module_load(&mod_gl_scope, *name))
    {
      error_disp(stdout);
      return (-1);
    }

  return (0);
}

static int
instr_help_	(char		**name)
{
  if (instr_help(*name))
    {
      disp_action_failed(stdout);
      error_disp(stdout);
    }
  else
    disp_action_end(stdout);

  return (-1);
}

static int
scope_meta_define	(char		**sym_name)
{
  struct symbol_s	*sym;

  /* FIXME check symbol name format here */

  sym = symbol_creat(*sym_name, &meta_scope_g);

  symbol_define(sym, expr_num_get_zero(), SYMBOL_TYPE_EXPR,
		&meta_scope_g, get_location());

  if (error_count_g)
    {
      disp_action_failed(stdout);
      error_disp(stdout);
      return (-1);
    }

  return (0);
}

static int
include_path_add	(char		**path)
{
  if (include_path_depth >= INCPATH_MAX)
    {
      error(ERRTYP_ERROR, "too many include paths specifyed on command line", NULL);
      disp_action_failed(stdout);
      error_disp(stdout);
      return (-1);
    }

  include_path[include_path_depth++] = *path;

  return (0);
}

const char		*args_title_g = AASM_PROJECT_NAME " - " AASM_PROJECT_DESC " " PACKAGE_VERSION;
const char		*args_copyright_g =  AASM_COPYRIGHT " " AASM_AUTHOR;
const char		*args_usage_g = "aasm [options] inputfile1 [inputfile2 ...]";

static unsigned int	object_flag_g = 0;

struct args_list_s	argslist_g[] =
{
  {
    "-h", "--help", "displays this help message",
    0, args_func_call,
    {(void*)&args_help}, 0, 0
  }, 
  {
    "-v", "--version", "displays version info",
    0, args_func_call,
    {(void*)&args_version}, 0, 0
  }, 
  {
    "", "--instr-help", "displays instruction help",
    1, args_func_call,
    {(void*)&instr_help_}, 0, 0x8
  },
  {
    "", "--color", "enables colored output\n",
    0, args_func_call,
    {(void*)&disp_color_set}, 0, 0
  },
  {
    "-Wmore", "--warning-more", "adds more warning messages",
    0, args_varint_set,
    {(void*)&error_mask_g}, ERRTYP_WARN_MORE, 0x40
  },
  {
    "-Wopt", "--warning-optim", "adds optimization warning messages",
    0, args_varint_set,
    {(void*)&error_mask_g}, ERRTYP_WARN_OPT, 0x10
  },
  {
    "-Wctrl", "--warning-ctrl", "adds control flow warning messages",
    0, args_varint_set,
    {(void*)&error_mask_g}, ERRTYP_WARN_CTRL, 0x20
  },
  {
    "-Wall", "--warning-all", "enables all warning messages",
    0, args_varint_set,
    {(void*)&error_mask_g}, ERRTYP_WARN_MORE | ERRTYP_WARN_OPT | ERRTYP_WARN_CTRL, 0x70
  },
  {
    "-Wnone", "--warning-none", "disables all warning messages",
    0, args_varint_set,
    {(void*)&error_mask_g}, ERRTYP_WARN, 0x70
  },
  {
    "-q", "--quiet", "disables process informations display\n",
    0, args_func_call,
    {(void*)&disp_quiet_set}, 0, 0
  },
  {
    "-D", "--define", "defines flag symbol",
    1, args_func_call,
    {(void*)&scope_meta_define}, 0, 0
  },
  {
    "-I", "--inc-path", "adds include path",
    1, args_func_call,
    {(void*)&include_path_add}, 0, 0
  },
  {
    "-g", "--debug", "enables debug features\n",
    0, args_varint_set,
    {(void*)&object_flag_g}, OBJECT_DEBUG, 0x100
  },
  {
    "-ml", "--module-list", "displays available modules list",
    0, args_func_call,
    {(void*)&module_list}, 0, 0
  },
  {
    "-m", "--module", "loads the specified module",
    1, args_func_call,
    {(void*)&mod_use}, 0, 0
  },
  {
    0, 0, 0,
    0, 0,
    {0}, 0
  }
};

int		main(int	argc,
		     char	**argv)

{
  int		res = 0;

  /* Initialize all memory structure */
  proc_init();
  expr_init();
  reloc_init();
  section_init();
  symbol_init();
  macro_init();
  location_init();
  error_init();
  comment_init();

  instr_init();
  string_init();

  /* init modules subsystem */
  modules_init();

  /* init meta scope */
  scope_init(&meta_scope_g, NULL, META_SYM_HASHSIZE);

  /* parse command line arguments */
  if (args_parse(argc - 1, argv + 1))
    return (0);

  /* check input file list */
  if (!*args_param_list_g)
    {
      fprintf(stderr, "No input files.\n");
      return (-1);
    }

  /* load input module */
  if (module_lock(&mod_gl_scope, MODULE_CLASS_IN))
    {
      disp_action_failed(stdout);
      error_disp(stdout);
      return (-1);
    }

  /* process each file */
  while (*args_param_list_g)
    {
      struct object_s	obj;

      /* init new object */
      object_init(&obj);
      obj.flag = object_flag_g;

      /* open input file */
      if (mod_gl_scope.cl.nm.in->call.in.open(*args_param_list_g))
	goto aasm_process_error;

      /* process input file */
      mod_gl_scope.cl.nm.in->call.in.process(&obj);

      /* close input file */
      mod_gl_scope.cl.nm.in->call.in.close();

      /* replace macro */
      if (!error_count_g)
	macro_replace(&obj);

      /* check object */
      object_check(&obj, *args_param_list_g);

      /* abort if error */
      if (error_count_g || module_lock(&obj.msc, MODULE_CLASS_ASM))
	goto aasm_process_error;

      /* assemble object file */
      disp_info(stdout, "assembling", obj.name);
      object_assemble(&obj);

      if (error_count_g || module_lock(&obj.msc, MODULE_CLASS_OUT))
	goto aasm_process_error;

      /* resolve adresses and relocations */
      object_reloc(&obj);

      /* write object file */
      res |= obj.msc.cl.nm.out->call.out.write(&obj);

      if (error_count_g)
	{
	aasm_process_error:
	  disp_action_failed(stdout);
	  error_disp(stdout);
	}
      else
	{
	  disp_action_end(stdout);
	  error_disp(stdout);
	}

      /* unlock asm module */
      module_unlock(&obj.msc, MODULE_CLASS_ASM);

      /* unlock output module & call clean function */
      if (!module_unlock(&obj.msc, MODULE_CLASS_OUT)
	  && (obj.msc.cl.nm.out->call.out.clean))
      	obj.msc.cl.nm.out->call.out.clean(&obj);

      if (error_count_g)
	res = 1;

      /* destroy error messages */
      error_destroy();

      /* free all memory used by object */
      object_free(&obj);

      /* prepare to process next input file */
      args_param_list_g++;
    }

  /* Clean permanent memory */
  expr_clean ();
  reloc_clean ();
  symbol_clean ();
  proc_clean ();
  section_clean ();
  macro_clean ();
  location_clean ();
  error_clean ();
  comment_clean ();

  instr_clean ();
  string_clean ();

  return (res);
}

