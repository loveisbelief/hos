/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: gperf -N in_directive_set -L ANSI-C -k'*' -c -D -t directive.perf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "directive.perf"
 /* -*- c -*- */

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

/*
+options+-N in_directive_set -L ANSI-C -k* -c -D -t
*/

#include <string.h>

#include "directive.h"
#include "parse.h"

static unsigned int hash (const char	*str,
			  unsigned int	len);

struct directive_s *in_directive_set (const char		*str,
				    unsigned int	len);

#line 40 "directive.perf"
struct directive_s
;

#define TOTAL_KEYWORDS 35
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 88
/* maximum key range = 85, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89,  0, 89,  0, 10, 15,
       0,  0,  5, 10,  0,  0, 25, 89,  5, 20,
       0,  0, 10, 89,  0,  0,  5,  5, 15,  0,
      15,  0, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89, 89, 89, 89, 89,
      89, 89, 89, 89, 89, 89
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct directive_s *
in_directive_set (register const char *str, register unsigned int len)
{
  static struct directive_s wordlist[] =
    {
#line 58 "directive.perf"
      {"ends",		&directive_section_end,	PARSER_ST_MAIN},
#line 54 "directive.perf"
      {"error",		&directive_error,	PARSER_ST_MAIN},
#line 64 "directive.perf"
      {"if",		&directive_if,		PARSER_ST_MAIN | PARSER_ST_SKIP | PARSER_NEED_ASMMOD},
#line 67 "directive.perf"
      {"else",		&directive_else,	PARSER_ST_MAIN | PARSER_ST_SKIP},
#line 66 "directive.perf"
      {"endif",		&directive_endif,	PARSER_ST_MAIN | PARSER_ST_SKIP},
#line 47 "directive.perf"
      {"define",		&directive_define,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 69 "directive.perf"
      {"db",		&directive_db,		PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 71 "directive.perf"
      {"pad",		&directive_pad,		PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 61 "directive.perf"
      {"endp",		&directive_proc_end,	PARSER_ST_MAIN},
#line 63 "directive.perf"
      {"ifdef",		&directive_ifdef,	PARSER_ST_MAIN | PARSER_ST_SKIP},
#line 65 "directive.perf"
      {"ifndef",		&directive_ifndef,	PARSER_ST_MAIN | PARSER_ST_SKIP},
#line 53 "directive.perf"
      {"warning",	&directive_warning,	PARSER_ST_MAIN},
#line 70 "directive.perf"
      {"fill",		&directive_fill,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 48 "directive.perf"
      {"align",		&directive_align,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 68 "directive.perf"
      {"string",		&directive_string,	PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 72 "directive.perf"
      {"reserve",	&directive_reserve,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 62 "directive.perf"
      {"endm",		&directive_macro_end,	PARSER_ST_MAIN},
#line 49 "directive.perf"
      {"label",		&directive_label,	PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 50 "directive.perf"
      {"extern",		&directive_extern,	PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 55 "directive.perf"
      {"section",	&directive_section,	PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 59 "directive.perf"
      {"proc",		&directive_proc,	PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 77 "directive.perf"
      {"debug",		&directive_debug,	PARSER_ST_MAIN},
#line 52 "directive.perf"
      {"include",	&directive_include,	PARSER_ST_MAIN},
#line 76 "directive.perf"
      {"mod_load",	&directive_mod_load,	PARSER_ST_MAIN},
#line 51 "directive.perf"
      {"export",		&directive_export,	PARSER_ST_MAIN},
#line 74 "directive.perf"
      {"mod_out",	&directive_out,		PARSER_ST_MAIN | PARSER_NEED_OUTMOD},
#line 73 "directive.perf"
      {"dump",		&directive_dump,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 60 "directive.perf"
      {"macro",		&directive_macro,	PARSER_ST_MAIN},
#line 56 "directive.perf"
      {"section_base",	&directive_sectionbase,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 75 "directive.perf"
      {"mod_asm",	&directive_asm,		PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 57 "directive.perf"
      {"section_align",	&directive_sectionalign,PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 46 "directive.perf"
      {"obj_base",	&directive_objbase,	PARSER_ST_MAIN | PARSER_NEED_ASMMOD},
#line 44 "directive.perf"
      {"obj_desc",	&directive_objdesc,	PARSER_ST_MAIN},
#line 43 "directive.perf"
      {"obj_name",	&directive_objname,	PARSER_ST_MAIN},
#line 45 "directive.perf"
      {"obj_copyright",	&directive_objcopyright,PARSER_ST_MAIN}
    };

  static signed char lookup[] =
    {
      -1, -1, -1, -1,  0,  1, -1,  2, -1,  3,  4,  5,  6,  7,
       8,  9, 10, 11, -1, 12, 13, 14, 15, -1, 16, 17, 18, 19,
      -1, 20, 21, -1, 22, 23, -1, -1, 24, 25, -1, 26, 27, -1,
      28, -1, -1, -1, -1, 29, 30, -1, -1, -1, -1, 31, -1, -1,
      -1, -1, 32, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, 34
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist[index];
            }
        }
    }
  return 0;
}
#line 78 "directive.perf"


