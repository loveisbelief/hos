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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "aasm/modules.h"
#include "aasm/error.h"
#include "aasm/alloc.h"
#include "aasm/disp.h"

unsigned int			include_path_depth = 2;
const char			*include_path[INCPATH_MAX + 1] =
  {
    ".",
    INCPATH_DEFAULT,
    NULL
  };

struct module_scope_s			mod_gl_scope =
  {
    {{NULL, NULL, NULL, NULL}}
  };

/*
** modules functions ptr
*/

#ifndef AASM_STATIC

static struct module_scope_s		mod_cache =
  {
    {{NULL, NULL, NULL, NULL}}
  };

static const char		*mod_dir[MOD_PATH_MAX] =
  {
    MOD_PATH_DEFAULT,
    NULL
  };

static const struct module_load_s	mod_in_symbols[MOD_IN_LOAD_LIST_SIZE + 1] = MOD_IN_LOAD_LIST;
static const struct module_load_s	mod_asm_symbols[MOD_ASM_LOAD_LIST_SIZE + 1] = MOD_ASM_LOAD_LIST;
static const struct module_load_s	mod_out_symbols[MOD_OUT_LOAD_LIST_SIZE + 1] = MOD_OUT_LOAD_LIST;

static int		module_load_symbols(struct module_s		*mod,
					    const struct module_load_s	*list,
					    unsigned int		api_version)
{
  unsigned int				i;

  if (mod->info->api_version != api_version)
    {
      error_submsg(error(ERRTYP_ERROR, "bad API version for `%s' module",
			 NULL), mod->info->name);
      return (-1);
    }

  for (i = 0; list[i].sym_suffix; i++)
    {
      char				symname[256];
      unsigned int			j;

      sprintf(symname, "%s_%s", mod->info->name, list[i].sym_suffix);

      /* non alpha and non digit char are set to _ */
      for (j = 0; symname[j]; j++)
	if (!isalnum(symname[j]))
	  symname[j] = '_';

      if (!(mod->call.table[i] = dlsym(mod->handle, symname))
	  && !(list[i].flags & MODULE_LOAD_OPTIONAL))
	{
	  error_submsg(error_submsg(error(ERRTYP_ERROR, "can't load `%s' module, symbol `%s' is missing",
					  NULL), mod->info->name), symname);
	  return (-1);
	}
    }

  return (0);
}

/*
static struct module_s	*module_find_name(const char		*name,
					  struct module_scope_s	*msc)
{
  struct module_s	*mod;
  unsigned int		i;

  for (i = 0; i < MOD_CLASS_COUNT; i++)
    for (mod = msc->cl.id[i]; mod; mod = mod->next)
      if (!strcmp(name, mod->info->name))
	return (mod);

  return (NULL);
}
*/

static struct module_s	*module_find_fid(unsigned int		file_id,
					  struct module_scope_s	*msc)
{
  struct module_s	*mod;
  unsigned int		i;

  for (i = 0; i < MOD_CLASS_COUNT; i++)
    for (mod = msc->cl.id[i]; mod; mod = mod->next)
      if (mod->file_id == file_id)
	return (mod);

  return (NULL);
}

static struct module_s	*module_preload(const char		*filename,
					struct module_scope_s	*mcache)
{
  unsigned int		i;

  for (i = 0; mod_dir[i]; i++)
    {
      struct module_s	*mod;
      char		pathname[512];
      struct stat	st;

      sprintf(pathname, "%s/aasm_%s.so", mod_dir[i], filename);

      if (stat(pathname, &st))
	continue;

      if ((mod = module_find_fid((unsigned)(st.st_dev ^ st.st_ino), mcache)))
	{
	  return (mod);
	}

      mod = xzalloc(sizeof (struct module_s));      
      mod->file_id = st.st_dev ^ st.st_ino;

      /* open module file */
      if ((mod->handle = dlopen(pathname, RTLD_GLOBAL | RTLD_NOW)))
	{
	  /* read module information structure */
	  if ((mod->info = dlsym(mod->handle, "module_info")))
	    {

	      /* load module symbols */
	      switch (mod->info->class)
		{
		case (MODULE_CLASS_IN):
		  if (module_load_symbols(mod, mod_in_symbols, CURRENT_IN_API_VERSION))
		    goto module_preload_err;

		  mod->next = mcache->cl.nm.in;
		  mcache->cl.nm.in = mod;

		  break;

		case (MODULE_CLASS_ASM):
		  if (module_load_symbols(mod, mod_asm_symbols, CURRENT_ASM_API_VERSION))
		    goto module_preload_err;

		  mod->next = mcache->cl.nm.asm_;
		  mcache->cl.nm.asm_ = mod;

		  break;

		case (MODULE_CLASS_OUT):	
		  if (module_load_symbols(mod, mod_out_symbols, CURRENT_OUT_API_VERSION))
		    goto module_preload_err;

		  mod->next = mcache->cl.nm.out;
		  mcache->cl.nm.out = mod;

		  break;

		case (MODULE_CLASS_FILTER):
		    goto module_preload_err;
		}

	      return (mod);
	    }

	module_preload_err:
      	  dlclose(mod->handle);
	  free(mod);
	}

      error_submsg(error_submsg(error(ERRTYP_ERROR, "can't load `%s' module: %s", NULL), filename), dlerror());
      return (NULL);
    }

  error_submsg(error(ERRTYP_ERROR, "can't find file for `%s' module", NULL), filename);

  return (NULL);
}

void			modules_init(void)
{
  unsigned int		i;
  char			*mod_env;

  if ((mod_env = xstrdup(getenv(MOD_PATH_ENV))))
    {
      for (i = 1; i < MOD_PATH_MAX - 1; i++)
	{
	  char		*mod_env_next;

	  mod_dir[i] = mod_env;
	  
	  if (!(mod_env_next = strchr(mod_env, ':')))
	    {
	      i++;
	      break;
	    }

	  *mod_env_next = 0;
	  mod_env = mod_env_next + 1;
	}

      mod_dir[i] = NULL;
    }
}

#else

#include "../static_list.h"

static struct module_s	*static_mod_list[STATIC_MOD_LIST_SIZE] = STATIC_MOD_LIST;

void			modules_init(void)
{
  return;
}

static struct module_s		*module_select(const char		*name)

{
  unsigned int			i;

  for (i = 0; i < STATIC_MOD_LIST_SIZE; i++)
    {
      if (!strcmp(name, static_mod_list[i]->info->name))
	return (static_mod_list[i]);
    }

  error_submsg(error(ERRTYP_ERROR, "can't find `%s' module statically linked with aasm", NULL), name);

  return (NULL);
}

#endif

int		module_load(struct module_scope_s	*mscope,
			    const char			*name)
{
  struct module_s	*mod;

#ifndef AASM_STATIC
  if ((mod = module_preload(name, &mod_cache)))
#else
  if ((mod = module_select(name)))
#endif
    {
      if (mscope->lock_flag & MODULE_LOCK(mod->info->class))
	{
	  error_submsg(error(ERRTYP_ERROR, "can't load module, `%s' locked", NULL),
		       mscope->cl.id[mod->info->class]->info->name);
	  return (-1);
	}

      mscope->cl.id[mod->info->class] = mod;
      return (0);
    }

  return (-1);
}

int		module_lock(struct module_scope_s	*mscope,
			    unsigned int		class)
{
  if (!mscope->cl.id[class])
    {

      if (mscope->parent && mscope->parent->cl.id[class])
	mscope->cl.id[class] = mscope->parent->cl.id[class];
      else
	{
	  struct module_s	*mod;
#ifndef AASM_STATIC
	  static const char	*defaults[MOD_CLASS_COUNT] =
	    {"default_in", "default_asm", "default_out"};

	  if (!(mod = module_preload(defaults[class], &mod_cache)))
#else
	  static const char	*defaults[MOD_CLASS_COUNT] = STATIC_MOD_DEFAULTS;

	  if (!(mod = module_select(defaults[class])))
#endif
	    return (-1);

	  if (mod->info->class != class)
	    {
	      error_submsg(error(ERRTYP_ERROR, "wrong class for `%s' module", NULL), mod->info->name);
	      return (-1);
	    }

	  mscope->cl.id[class] = mod;
	}
    }

  if (!(mscope->lock_flag & MODULE_LOCK(class)))
    {
      disp_info(stdout, "using", mscope->cl.id[class]->info->name);
      mscope->lock_flag |= MODULE_LOCK(class);
    }

  return (0);
}

int		module_unlock(struct module_scope_s	*mscope,
			      unsigned int		class)
{
  if (mscope->lock_flag & MODULE_LOCK(class))
    {
      mscope->lock_flag ^= MODULE_LOCK(class);
      return (0);
    }

  return (-1);
}

#define MODULE_LIST_FORMAT	"  %-22s %-14s %u.%u.%u   %-8s %s\n"

int		module_list(const char		**dummy)
{
  unsigned int		i;
  static const char	*mod_class[MOD_CLASS_COUNT] =
    {"input", "asm", "output", "filter"};

  printf("  File name              Module name    Version Class    Description\n");

#ifdef AASM_STATIC
      printf("\n> modules statically linked with aasm\n\n");

      for (i = 0; i < STATIC_MOD_LIST_SIZE; i++)
	{
	  const struct module_info_s	*info = static_mod_list[i]->info;

	  printf(MODULE_LIST_FORMAT,
		 "builtin", info->name,
		 info->version[0], info->version[1], info->version[2],
		 mod_class[info->class], info->info);
	}
#else
  for (i = 0; mod_dir[i]; i++)
    {
      DIR		*dir;
      struct dirent	*entry;
      void		*handle;

      if (!(dir = opendir(mod_dir[i])))
	{
	  printf("\n> Can't open `%s' directory\n", mod_dir[i]);
	  continue;
	}

      printf("\n> %s\n\n", mod_dir[i]);

      /* read directory file list */
      while ((entry = readdir(dir)))
	{
	  char		filename[512];
# ifdef MOD_DONT_LIST_LINK
	  struct stat	st;
# endif

	  /* skip file with bad prefix */
	  if (strncmp(entry->d_name, "aasm_", 5))
	    continue;

	  /* get full file path */
	  sprintf(filename, "%s/%s", mod_dir[i], entry->d_name);

# ifdef MOD_DONT_LIST_LINK
	  /* skip sym links */
	  if (lstat(filename, &st) || S_ISLNK(st.st_mode))
	    continue;
# endif

	  if ((handle = dlopen(filename, RTLD_NOW)))
	    {
	      const struct module_info_s	*info;

	      if ((info = dlsym(handle, "module_info")))
		{
		  printf(MODULE_LIST_FORMAT,
			 entry->d_name, info->name,
			 info->version[0], info->version[1], info->version[2],
			 mod_class[info->class], info->info);
		}

	      dlclose(handle);
	    }
	}

      closedir(dir);
    }

#endif

  return (1);
}

