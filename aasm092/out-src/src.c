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
**    Copyright Cedric BAIL, 2002
**    contact : cedric.bail@epita.fr
*/

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "aasm/error.h"
#include "aasm/object.h"
#include "aasm/section.h"
#include "aasm/symbol.h"
#include "aasm/info.h"
#include "aasm/disp.h"
#include "aasm/expr.h"
#include "aasm/comments.h"
#include "aasm/macro.h"
#include "aasm/proc.h"
#include "aasm/mod_out_import.h"

#include "src.h"

# define	MAX_TAB 5

/* Generate enougth tabulation for curent scope */
static void
src_indent(unsigned int		n,
	   FILE			*file)
{
  unsigned int	i;

  for (i = 0; i < n; i++)
    fputc ('\t', file);
}

/* Display a label and his attribute */
static void
src_disp_unique_label (unsigned int		tab,
		       const struct symbol_s	*label,
		       FILE			*file)
{
  static const char*	qualifier[17] =
    {
      "", "taken", "nottaken", "", "loop", "", "", "",
      "call", "", "", "", "", "", "", "", "data"
    };

  src_indent(tab, file);
  
  fprintf (file, ".label %s %s\n", label->name,
	   qualifier[(label->flag & SYMBOL_LABEL_MASK) >> SYMBOL_LABEL_SHIFT]);
}

/* Display comment (One added comment => One line of comment) */
static void
src_write_comments (unsigned int		tab,
		    const struct comment_s	*comments,
		    FILE			*file)
{
  const struct comment_s*	j;

  for (j = comments; j != NULL; j = j->next)
    {
      src_indent(tab, file);
      fputc (';', file);
      
      fprintf (file, "%s\n", j->str);
    }  
}

/* 
   Loop into the list of all labels of the current instruction
   and display labels
 */
static void
src_disp_label (unsigned int		tab,
		const struct instr_s	*instr,
		FILE			*file)
{
  const struct symbol_s	*label = instr->label;

  do
    {
      if (label->flag & (SYMBOL_LABEL_LOOP | SYMBOL_LABEL_NOTTAKEN) && instr->len)
	fputc ('\n', file);

      src_disp_unique_label (tab, label, file);

      label = label->sibling;
    }
  while (label && (label != instr->label));
}

/*
  Loop into the list of all labels of the current information,
  display labels except the first one who has it's attribute set
  to data and return it to the caller function
 */
static const char*	src_disp_label_string (unsigned int		tab,
					       const struct instr_s	*instr,
					       FILE			*file)
{
  const struct symbol_s	*label = instr->label;
  const char*		save = NULL;

  do
    {
      if (label->flag & (SYMBOL_LABEL_LOOP | SYMBOL_LABEL_NOTTAKEN) && instr->len)
	fputc ('\n', file);

      if (label->flag | SYMBOL_LABEL_DATA && save == NULL)
	save = label->name;
      else
	{
	  src_disp_unique_label (tab, label, file);
	  
	  label = label->sibling;
	}
    }
  while (label && (label != instr->label));

  return save;
}

/* Display an instruction */
static void
src_disp_instr (unsigned int		tab,
		const struct instr_s	*instr,
		const struct section_s	*sec,
		FILE			*file,
		const struct object_s	*obj)
{
  unsigned int	i;

  /* 
     If debug flag is set we display some other information such as
     address and binary form
   */
  if (obj->flag & OBJECT_DEBUG)
    {
      src_indent(tab, file);
      fputc (';', file);

      if (instr->flag & INSTR_ADDR)
	fprintf (file, " address: 0x%08x", instr->offset);
      fprintf (file, " instruction binary form: ");

      disp_hex(instr->val.bin, instr->len, file);

      fputc ('\n', file);

      src_write_comments (tab, instr->comments, file);
    }

  /* Display the instruction and is attached operand */
  src_indent(tab, file);

  fprintf (file, "%s ", obj->msc.cl.nm.asm_->call.asm_.instr_name (instr->info));
  for (i = 0; i != instr->op_count; ++i)
    {
      if (i)
	fprintf (file, ", ");
      expr_fout_sec (instr->operands[i], file, sec, obj);
    }

  if (obj->flag & OBJECT_DEBUG)
    fputc ('\n', file);
  fputc ('\n', file);

  /* If this instruction was asked to be aligned display this info */
  if (instr->flag & INSTR_ALIGN_MASK)
    {
      src_indent(tab, file);

      fputs(".align ", file);	

      if (instr->flag & INSTR_ALIGN_OFFSET)
	fputs("offset ", file);

      if (instr->flag & INSTR_ALIGN_BOUNDARY)
	fputs("boundary ", file);
      fprintf(file, "0x%x\n", instr->align);
    }
}

/*
  This function test if a data is a string or not, if it is,
  display it
 */
static int
src_disp_string (unsigned int		tab,
		 const struct instr_s	*instr,
		 FILE			*file)
{
  unsigned int		i;
  const char*		label = NULL;
  const struct instr_s	*label_instr = instr;

  instr = instr->next;

  for (i = 0; i != instr->len; ++i)
    if (isprint(instr->val.bin[i]) == 0
	&& isspace(instr->val.bin[i]) == 0
	&& instr->val.bin[i] != '\0')
      return -1;

  if (i == 0)
    return -1;

  if (label_instr->label &&
      (label = src_disp_label_string (tab, label_instr, file)) == NULL)
    return -1;

  src_indent(tab, file);
  
  /* If this string as a name */
  if (label)
    fprintf (file, ".string %s \"", label);
  else
    fprintf (file, ".string \"");

  /* For each binary test the value and display it */
  for (i = 0; i != instr->len; ++i)
    if (isprint(instr->val.bin[i]))
      fputc (instr->val.bin[i], file);
    else
      if (instr->val.bin[i])
	{
	  /* 
	     It was not a printable caractere, so display it with a
	     backslash and a normalized form
	   */
	  static const char	*str_esc1 = "\a\b\f\n\r\t\v\\\"";
	  static const char	*str_esc2 = "abfnrtv\\\"";

	  fputc ('\\', file);
	  fputc (str_esc2[strchr(str_esc1, instr->val.bin[i]) - str_esc1],
		 file);
	}
      else
	/* That's a stupid end of string caractere, don't forget display it */
	fputs ("\\0", file);

  fputs ("\"\n", file);
  return 0;
}

/* Display all byte of a instruction in hexadecimal form with .db directive */
static void
src_disp_binary (unsigned int		tab,
		 const struct instr_s	*instr,
		 FILE			*file)
{
  unsigned int	i;

  for (i = 0; i != instr->len; ++i)
    {
      if (!(i % 11))
	{
	  if (i != 0)
	    fputc ('\n', file);
	  src_indent(tab, file);
	  fputs (".db ", file);
	}
      else
	fputs (", ", file);
      fprintf (file, "0x%02x", instr->val.bin[i]);
    }
  fputc ('\n', file);
}

/* Display extern symbol */
static void
src_disp_extern (unsigned int		tab,
		 const struct symbol_s	*sym,
		 FILE			*file)
{
  src_indent(tab, file);
  fprintf (file, ".extern %s\n", sym->name);
}

/* Display exported symbol */
static void
src_disp_export (unsigned int		tab,
		 const struct symbol_s	*sym,
		 FILE			*file)
{
  src_indent(tab, file);
  fprintf (file, ".export %s\n", sym->name);
}

/* Display definition of a symbol */
static void
src_disp_define (unsigned int		tab,
		 const struct symbol_s	*sym,
		 const struct section_s	*sec,
		 FILE			*file,
		 const struct object_s	*obj)
{
  src_indent(tab, file);

  fprintf(file, ".define %s ", sym->name);
  expr_fout_sec(sym->val.expr, file, sec, obj);
  fputc('\n', file);
}

/* Display a symbol of a scope */
static void
write_process_scope_sym (const struct scope_s	*scope,
			 const struct section_s	*sec,
			 FILE			*file,
			 unsigned int		tab,
			 const struct object_s	*obj)
{
  unsigned int		i;
  unsigned int		ext = 0;

  /* display defines in debug mode */ 
  if (obj->flag & OBJECT_DEBUG)
    for (i = 0; i < scope->symbol_hash_size; i++)
      {
	const struct symbol_s	*sym;
	
	for (sym = scope->symbols[i]; sym; sym = sym->next)
	  if (sym->flag & SYMBOL_TYPE_EXPR)
	    {
	      src_disp_define(tab, sym, sec, file, obj);
	      ext = 1;
	    }
      }
  
  /* display extern symbols */
  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      const struct symbol_s	*sym;
      
      for (sym = scope->symbols[i]; sym; sym = sym->next)
	if (sym->flag & SYMBOL_TYPE_EXTERN)
	  {
	    src_disp_extern(tab, sym, file);
	    ext = 1;
	  }
    }

  /* display exported symbols */
  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      const struct symbol_s	*sym;
      
      for (sym = scope->symbols[i]; sym; sym = sym->next)
	if (sym->flag & SYMBOL_EXPORT)
	  {
	    src_disp_export(tab, sym, file);
	    ext = 1;
	  }
    }
  
  if (ext)
    fputc('\n', file);
}

/* Display instructions of a scope */
static void
write_process_scope_instr(const struct scope_s		*scope,
			  const struct section_s	*sec,
			  FILE				*file,
			  unsigned int			tab,
			  const struct object_s		*obj)
{
  const struct instr_s	*instr = &scope->first_instr;

  /* display instructions */
  while (instr && instr->next)
    {
      if (instr->label)
	src_disp_label (tab, instr, file);

      instr = instr->next;

      switch (instr->flag & INSTR_TYPE_MASK)
	{
	case (INSTR_TYPE_DATA):

	  if (src_disp_string (tab, instr, file))
	    src_disp_binary (tab, instr, file);
	  break;

	case (INSTR_TYPE_OPCODE):

	  src_disp_instr (tab, instr, sec, file, obj);
	  break;

	case (INSTR_TYPE_UNINIT):

	  src_indent(tab, file);
	  fprintf(file, ".reserve %u\n", instr->len);
	  break;

	default:
	  break;
	}
    }
}

/* Display a macro */
static void
src_disp_macro (const struct symbol_s	*sym,
		const struct macro_s	*macro,
		const struct section_s	*sec,
		FILE			*file,
		unsigned int		tab,
		const struct object_s	*obj)
{
  unsigned int		i;

  src_indent (tab, file);
  fprintf (file, ".macro %s", sym->name);

  /* Display params */ 
  for (i = 0; i != macro->param_count; ++i)
    if (macro->sym[i].flag & SYMBOL_TYPE_PARAM)
      fprintf (file, " %s", macro->sym[i].name);

  fputc ('\n', file);

  tab++;

  /* display instructions */
  write_process_scope_instr(&macro->scope, sec, file, tab, obj);

  tab--;

  src_indent (tab, file);
  fprintf (file, ".endm\n\n");
}

/* Display macro defined in a scope */
static void
write_process_scope_macro (const struct scope_s		*scope,
			   const struct section_s	*sec,
			   FILE				*file,
			   unsigned int			tab,
			   const struct object_s	*obj)
{
  unsigned int i;

  for (i = 0; i < scope->symbol_hash_size; i++)
    {
      const struct symbol_s	*sym;
      
      for (sym = scope->symbols[i]; sym; sym = sym->next)
	if (sym->flag & SYMBOL_TYPE_MACRO)
	  src_disp_macro(sym, sym->val.macro, sec, file, tab, obj);
    }
}

/* Display a section */
static void
write_process_section (const struct section_s	*sec,
		       FILE			*file,
		       const struct object_s	*obj)
{
  struct proc_s		*proc;

  src_write_comments (0, sec->scope.comments, file);

  if (sec->flag & SECTION_BASEADDR)
    fprintf (file, "\t.section_base 0x%08x\n", sec->address);

  write_process_scope_sym(&sec->scope, sec, file, 1, obj);
  write_process_scope_instr(&sec->scope, sec, file, 1, obj);
  write_process_scope_macro(&sec->scope, sec, file, 1, obj);

  for (proc = sec->proc_first; proc; proc = proc->next)
    {
      src_write_comments (1, proc->scope.comments, file);

      fprintf (file, "\t.proc %s\n\n", proc->sym->name);	  

      write_process_scope_sym(&proc->scope, sec, file, 2, obj);
      write_process_scope_instr(&proc->scope, sec, file, 2, obj);
      fprintf (file, "\t.endp\n\n");
    }
}

/* Entry in the pretty printer */
static void
src_write (const struct object_s	*obj,
	   FILE				*file)
{
  static const char	*type_s [] =
    {
      "notype", "wrcode", "data",   "", "bss", "", "", "",
      "notype", "code",   "rodata", "", "bss", "", "", ""
    };

  struct section_s	*sec;
  int			boolean = 0;

  time_t tmp = time (NULL);

  /* display current action */
  fprintf (file, 
	   "; Generated on %s"
	   "; by " AASM_PROJECT_NAME " - " AASM_PROJECT_DESC " " PACKAGE_VERSION "\n\n",
	   ctime (&tmp));

  fprintf (file,
	   "; Object             : %s\n"
	   "; Object source file : %s\n"
	   "; Copyright          : %s\n"
	   "; Description        : %s\n\n",
	   obj->name,
	   obj->srcfilename,
	   obj->copyright,
	   obj->description);

  /* display object directive */
  if (obj->name)
    {
      boolean = 1;
      fprintf (file, ".obj_name %s\n", obj->name);
    }

  if (obj->copyright && strcmp (obj->copyright, "none") != 0)
    {
      boolean = 1;
      fprintf (file, ".obj_copyright %s\n", obj->copyright);
    }

  if (obj->description && strcmp (obj->description, "none") != 0)
    {
      boolean = 1;
      fprintf (file, ".obj_desc %s\n", obj->description);
    }

  if (obj->flag & OBJECT_BASEADDR)
    {
      boolean = 1;
      fprintf (file, ".obj_base 0x%08x\n", obj->address);
    }

  if (boolean)
    fputc ('\n', file);

  /* If in debug mode */
  if (obj->flag & OBJECT_DEBUG)
      fprintf (file, ".debug\n\n");

  write_process_scope_sym(&obj->global, NULL, file, 0, obj);

  /* process all sections */
  for (sec = obj->section_first; sec; sec = sec->next)
    {
      fprintf (file, ".section %s %s\n\n",
	       type_s[sec->flag & SECTION_TYPE_MASK],
	       sec->name);
      write_process_section(sec, file, obj);
      fprintf (file, ".ends\n\n");
    }
}    

/* Open file and start to write data */
int
out_src_write (struct object_s	*obj)
{
  FILE		*file;
  char		filename[512];

  sprintf(filename, "%s_src.aam", obj->name);
  disp_info(stdout, "writing", filename);

  if ((file = fopen(filename, "w+")))
    {
      src_write(obj, file);
      fclose(file);
      return (0);
    }

  error_submsg(error(ERRTYP_ERROR, "src: output error, can't write `%s'", NULL),
	       filename);
  return (-1);
}

/* No directive are supported by this module */
void
out_src_directive (struct object_s	*obj,
		   const char		*param)
{
  error(ERRTYP_ERROR, "src: unknown .out directive", 0);
}

