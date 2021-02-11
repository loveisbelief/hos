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

#ifndef MODULES_H_
# define MODULES_H_

# ifdef HAVE_DLFCN_H
#  include <dlfcn.h>
# else

#  define RTLD_LAZY		0x00001 /* Lazy function call binding.  */
#  define RTLD_NOW		0x00002 /* Immediate function call binding.  */
#  define RTLD_BINDING_MASK	0x00003 /* Mask of binding time value.  */
#  define RTLD_NOLOAD		0x00004 /* Do not load the object.  */
#  define RTLD_GLOBAL		0x00100
#  define RTLD_LOCAL		0

void		*dlopen(const char	*,
			int		);
int		dlclose(void		*);
void		*dlsym(void		*,
		       const char	*);
const char	*dlerror(void);

# endif

# define MODULE_LOAD_OPTIONAL	1

struct				module_load_s
{
  const char			*sym_suffix;
  unsigned int			flags;
};

# include "mod_in_import.h"
# include "mod_asm_import.h"
# include "mod_out_import.h"

# ifndef AASM_PREFIX
#  define AASM_PREFIX		"/usr"
# endif

# define MOD_PATH_DEFAULT	AASM_PREFIX "/lib/aasm"

# define MOD_PATH_ENV		"AASM_MOD_PATH"
# define MOD_PATH_MAX		20

# define MODULE_CLASS_IN	0
# define MODULE_CLASS_ASM	1
# define MODULE_CLASS_OUT	2
# define MODULE_CLASS_FILTER	3
# define MODULE_CLASS_UNDEF	4

# define MOD_CLASS_COUNT	4

struct				module_info_s
{
  int				api_version; /* must be first field */

  const char			*name;
  const char			*copyright;
  const char			*info;
  int				version[3];

  unsigned int			arch_id;

  unsigned int			class;
  int				priority;
};

struct				module_s
{
  const struct module_info_s	*info;

  unsigned int			lock;
  unsigned int			file_id; /* file id based on inode and device */

  void				*handle;
  struct module_s		*next;

  union
  {
    void			*table[MOD_ASM_LOAD_LIST_SIZE];
    struct module_in_calls_s	in;
    struct module_asm_calls_s	asm_;
    struct module_out_calls_s	out;
  }				call;

};

# define MODULE_LOCK(n)		(1 << n)

struct				module_scope_s
{
  union
  {
    struct
    {
      struct module_s		*in;
      struct module_s		*asm_;
      struct module_s		*out;
      struct module_s		*filter;
    }				nm;
    struct module_s		*id[MOD_CLASS_COUNT];
  }				cl;

  unsigned int			lock_flag;

  struct module_scope_s		*parent;
};

extern struct module_scope_s	mod_gl_scope;

/* include paths are handled by module system */

# define INCPATH_MAX		16
# define INCPATH_DEFAULT	AASM_PREFIX "/include/aasm"

extern const char		*include_path[INCPATH_MAX + 1];
extern unsigned int		include_path_depth;

/*
** modules.c
*/

void		modules_init(void);

int		module_load(struct module_scope_s	*mscope,
			    const char			*name);

int		module_lock(struct module_scope_s	*mscope,
			    unsigned int		class);

int		module_unlock(struct module_scope_s	*mscope,
			      unsigned int		class);

int		module_list(const char		**dummy);

#endif

