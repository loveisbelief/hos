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

#ifndef PARSE_H_
# define PARSE_H_

#include "aasm/error.h"
#include "aasm/mod_in_import.h"

# define MAX_INCLUDE_DEPTH	16
# define MAX_ERROR_COUNT	25
# define MAX_OPERAND_COUNT	16

# define MAX_IFDEF_DEPTH	16

extern unsigned int		parse_ifdef_depth_g;
extern unsigned int		parse_ifdef_g[MAX_IFDEF_DEPTH];

# define COMMENT_CHAR		';'
# define DIRECTIV_1ST_CHAR	'.'
# define USE_MACRO_CHAR		'@'
# define SECTION_NAME_1ST_CHAR	'.'
# define SECTION_USE_CHAR	':'
# define OPERAND_SEP_CHAR	','
# define SPACE_CHARS		" \t\n\r"
# define STD_LABEL_CHAR		':'
# define REGISTER_1ST_CHAR	'%'
# define EXPR_PARAM_1ST_CHAR	'$'
# define SYM_PARAM_CHAR		'('

# define PARSER_ST_MAIN		0x1
# define PARSER_ST_SKIP		0x2
# define PARSER_ST_STRUCT	0x4

# define PARSER_NEED_ASMMOD	0x100 /* flag for directives depending on asm module  */
# define PARSER_NEED_OUTMOD	0x200 /* flag for directives depending on output module  */

extern unsigned int		parser_state_g;

struct				expr_s;
struct				object_s;
struct				error_locat_s;

int			parse_operands(const char		*param,
				       struct expr_s		**operands,
				       struct object_s		*obj,
				       struct error_locat_s	*location);

int			parse_open	(const char	*filename);

in_open_t		in_parser_open;
in_close_t		in_parser_close;
in_get_location_t	in_parser_get_location;
in_process_t		in_parser_process;

#endif
