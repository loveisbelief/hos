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

#ifndef SCAN_H_
# define SCAN_H_

# include "aasm/types.h"

# define MAX_LINE_LEN		512

typedef int			auto_state_t		[256];

extern const auto_state_t	automata_dec		[1];
extern const auto_state_t	automata_num		[6];
extern const auto_state_t	automata_idf		[2];
extern const auto_state_t	automata_section	[2];
extern const auto_state_t	automata_operand	[3];
extern const auto_state_t	automata_singlechar	[2];
extern const auto_state_t	automata_line		[1];
extern const auto_state_t	automata_word		[1];
extern const auto_state_t	automata_spacecomment	[2];
extern const auto_state_t	automata_space		[1];
extern const auto_state_t	automata_string		[4];

unsigned int	scan_store	(const char		*in,
				 const char		**end,
				 const char		*format,
				 ...);

const char	*scan_skip	(const char		*str,
				 const auto_state_t	*automata);

unsigned int	scan_get_len	(const char		*str,
				 const auto_state_t	*automata);

const char	*scan_get_end	(const char		*str,
				 const auto_state_t	*automata,
				 const char		**end,
				 char			*buffer);

const char	*scan_get	(const char		*str,
				 const auto_state_t	*automata,
				 char			*buffer);

unsigned int	scan_hash	(const char		*str,
				 const auto_state_t	*automata);

unsigned int	scan_string_escape(const aasm_u8_t	*in,
				   aasm_u8_t		*out,
				   unsigned		end_char);

#endif

