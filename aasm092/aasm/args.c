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



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aasm/types.h"
#include "aasm/alloc.h"

#include "args.h"

char				**args_param_list_g;

/*
** command line arguments handling functions
*/

int		args_version(char	**dummy)
{
  printf("%s\n", args_title_g);
#ifdef BUILD_INFO
  printf("Build: %s\n", BUILD_INFO);
#endif
  return (1);
}

int		args_help(char		**dummy)
{
  unsigned int		i;

  printf("%s\n"
	 "%s\n\n"
	 "Usage: %s\n"
	 "List of available options: \n",
	 args_title_g, args_copyright_g, args_usage_g);

  for (i = 0; argslist_g[i].str_short; i++)
    {
      printf("    %-8s%-16s%s\n",
	     argslist_g[i].str_short, argslist_g[i].str_long,
	     argslist_g[i].explanation);
    }
  return (1);
}

int		args_parse(int		argc,
			   char		**argv)
{
  int			args_ok = 1;
  unsigned int		param_list_cnt = 0;
  unsigned int		exclude = 0;

  args_param_list_g = xzalloc((argc + 1) * sizeof (char **));

  while (argc)
    {
      unsigned int	i;

      /* look for possible option in args list */

      if (args_ok)
	{
	  if (!strcmp(*argv, "--"))
	    {
	      args_ok = 0;
	      argc--, argv++;
	      continue;
	    }
	  for (i = 0; argslist_g[i].str_short; i++)
	    if (**argv && (!strcmp(argslist_g[i].str_short, *argv)
		|| !strcmp(argslist_g[i].str_long, *argv)))
	      break;
	}

      if (args_ok && argslist_g[i].str_short)
	{
	  
	  if (argc < argslist_g[i].param_count + 1)
	    {
	      /* error, invalid option usage */
	      fprintf(stderr, "Invalid use of %s option, try --help.\n", *argv);
	      return (1);
	    }

	  /* test exclusion mask */
	  if (exclude & argslist_g[i].exclude_mask)
	    {
	      int	j;

	      /* display list of excluded options */
	      fprintf(stderr, "Use of %s not allowed with: ", *argv);
	      for (j = 0; argslist_g[j].str_short; j++)
		if (argslist_g[i].exclude_mask
		    & argslist_g[j].exclude_mask)
		  fprintf(stderr, "%s %s ", argslist_g[j].str_short,
			  argslist_g[j].str_long);
	      fprintf(stderr, "\n");
	      return (1);
	    }
	  else
	    /* set entry exclusion bits */
	    exclude |= argslist_g[i].exclude_mask;

	  /* determine action for this arg */
	  switch (argslist_g[i].action)
	    {
	    case args_varint_set:	/* int set */
	      if (argslist_g[i].param_count)
		*argslist_g[i].value_ptr.num = strtoul(argv[1], 0, 0);
	      else
		*argslist_g[i].value_ptr.num ^= argslist_g[i].value;
	      break;

	    case args_varstr_set:	/* string set */
	      *argslist_g[i].value_ptr.str = argv[1];
	      break;

	    case args_func_call:	/* function call */
	      if (argslist_g[i].value_ptr.func(argv + 1))
		return (1);
	      break;

	    case args_no_action:
	      ;
	    }

	  /* skip to next arg */
	  argc -= argslist_g[i].param_count + 1;
	  argv += argslist_g[i].param_count + 1;

	}
      else
	{
	  if (args_ok && (**argv == '-') && (*(*argv + 1) != 0))
	    {	  
	      /* error, invalid option */
	      fprintf(stderr, "Invalid option: %s, try --help.\n", *argv);
	      return (1);
	    }
	  else
	    /* add to param list */
	    args_param_list_g[param_list_cnt++] = *argv;
	  argc--, argv++;
	}

    }

  return (0);
}
