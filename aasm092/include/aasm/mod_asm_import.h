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

#ifndef MOD_ASM_IMPORT_H_
# define MOD_ASM_IMPORT_H_

#include <stdio.h>

# define CURRENT_ASM_API_VERSION	1

struct				register_s;
struct				object_s;
struct				instr_info_s;
struct				instr_s;
struct				section_s;

typedef struct instr_perf_s	*asm_get_instr_perf_t	(const char			*str,
							 unsigned int			len);

typedef struct register_s	*asm_get_reg_t		(const char			*str,
							 unsigned int			len);

typedef void			asm_process_t		(struct instr_s			*instr,
							 struct section_s		*sec);

typedef unsigned int		asm_reg_size_t		(const struct register_s	*reg);

typedef const char		*asm_reg_name_t		(const struct register_s	*reg);

typedef const char		*asm_instr_name_t	(const struct instr_info_s	*instr);

typedef unsigned int		asm_max_binary_size_t	(const struct instr_info_s	*instr);

typedef int			asm_instr_help_t	(const struct instr_info_s	*info,
							 FILE				*out);

typedef void			asm_directive_t		(struct object_s		*obj,
							 const char			*param);

typedef const unsigned int	*asm_endian_permut_t	(unsigned int			byte_size);

struct				module_asm_calls_s
{
  asm_get_instr_perf_t	*get_instr_perf;
  asm_get_reg_t		*get_reg;
  asm_process_t		*process;
  asm_reg_size_t	*reg_size;
  asm_reg_name_t	*reg_name;
  asm_instr_name_t	*instr_name;
  asm_max_binary_size_t	*max_binary_size;
  asm_instr_help_t	*instr_help;
  asm_directive_t	*directive;
  asm_endian_permut_t	*endian_permut;
};

# define MOD_ASM_LOAD_LIST	\
{\
  {"get_instr_perf", 0},\
  {"get_reg", 0},\
  {"process", 0},\
  {"reg_size", 0},\
  {"reg_name", 0},\
  {"instr_name", 0},\
  {"max_binary_size", 0},\
  {"instr_help", MODULE_LOAD_OPTIONAL},\
  {"directive", MODULE_LOAD_OPTIONAL},\
  {"endian_permut", 0},\
  {NULL, 0}\
}

# define MOD_ASM_LOAD_LIST_SIZE	10

#endif

