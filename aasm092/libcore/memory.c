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
**    Copyright Cedric Bail, 2002
**    contact : cedric.bail@epita.fr
*/

#define DEBUG	1

#include <stdio.h>
#include <string.h>

#include "aasm/alloc.h"
#include "aasm/memory.h"
#include "aasm/types.h"

/*
** Fixed memory alloc
*/

#ifdef DEBUG

void		mem_disp_data (void* ptr, struct mem_s *memory)
{
  fprintf (stderr, "%s:%p: {", memory->desc->name, ptr);
  if (memory->desc->data_disp)
    memory->desc->data_disp (ptr);
  fputs("}\n", stderr);
}

#endif

void		mem_push(void* ptr, struct mem_s *memory)
{
  if (memory->free_used >= memory->free_max) 
    {
      /* expand freed memory block list when full */
      memory->free_max += MEM_REALLOC_SIZE;
      memory->free_list = xrealloc (memory->free_list,
				       sizeof (void*) * memory->free_max);
    }

  /* store freed block in memory block list */
  memory->free_list[memory->free_used] = ptr;
  memory->free_used++;

#ifdef DEBUG
  /* prevent use of freed data */
  /*  memset(ptr, 0xA5, memory->sizeof_struct);*/
#endif
}

void		*mem_pop (struct mem_s *memory)
{
  if (memory->free_used > 0)
    {
      /* return previously freed block if available  */
      memory->free_used--;
      return (memory->free_list[memory->free_used]);
    }

  if (!memory->list || (memory->entry_used >= MEM_REALLOC_SIZE))
    {
      /* no more space left in memory block */

      if (memory->list_used >= memory->list_max)
	{
	  /* expand memory block list to store more block addrs */

	  memory->list_max += MEM_REALLOC_SIZE;
	  memory->list = xrealloc (memory->list, sizeof (void*)
				     * memory->list_max);
	  
	  memset (memory->list + (memory->list_max - MEM_REALLOC_SIZE)
		  * sizeof (void*), 0, MEM_REALLOC_SIZE * sizeof (void*));
	}

      /* allocate memory block */
      memory->list[memory->list_used] = xmalloc (MEM_REALLOC_SIZE
						   * memory->sizeof_struct);
#ifdef DEBUG
      /* prevent use of freed data */
      /*      memset(memory->list[memory->list_used], 0xA5,
	      MEM_REALLOC_SIZE * memory->sizeof_struct);*/
#endif

      memory->list_used++;
      memory->entry_used = 0;
    }

  return (void*) ((aasm_u8_t*)(memory->list[memory->list_used - 1])
		  + (memory->entry_used++ * memory->sizeof_struct));
}

void		mem_clean (struct mem_s *memory)
{
  if (memory->free_list)
    free (memory->free_list);

  if (memory->list)
    {
      unsigned int i;

      for (i = 0; i < memory->list_used; ++i)
	free (memory->list[i]);
      free (memory->list);
    }

  memset (memory, 0, sizeof (struct mem_s));
}

void
mem_init		(struct mem_s			*memory,
			 unsigned int			size,
			 const struct mem_desc_s	*mem_desc)
{
  memset (memory, 0, sizeof (struct mem_s));

  /* Must preserve the alignement of allocated structure */
  ALIGN(size);

  memory->sizeof_struct = size;
  memory->desc = mem_desc;
}

/*
** Variable size malloc
*/

struct resize_s
{
  unsigned int	size;
  void*		ptr;
};

void		mem_resize_push (void* ptr,
				 struct resize_mem_s *memory)
{
  unsigned int size;
  struct resize_s* tmp = (struct resize_s*) ptr;

  if (tmp == NULL)
    return ;

  tmp--;
  size = tmp->size;

  if (memory->free_used + 1 > memory->free_max) 
    {
      memory->free_max += MEM_REALLOC_SIZE;
      memory->free_list = xrealloc (memory->free_list, sizeof (void*) * memory->free_max);
      memory->freed_size = xrealloc (memory->freed_size, sizeof (unsigned int) * memory->free_max);
    }

  memory->freed_size[memory->free_used] = size;
  memory->free_list[memory->free_used] = tmp;
  memory->free_used++;
}

void		*mem_resize_pop (struct resize_mem_s *memory,
				 unsigned int size)
{
  struct resize_s* tmp;
  unsigned int i;
  unsigned int nearest = MEM_REALLOC_SIZE * MEM_REALLOC_SIZE;
  int index = -1;

  if (size == 0 && memory->sizeof_struct == 0)
    return NULL;

  size += memory->sizeof_struct;

  /* Must preserve alignement of data */
  ALIGN(size);

  if (memory->free_used != 0)
    for (i = 0; i != memory->free_used; ++i)
      if (memory->freed_size[i] == size)
	{
	  memory->free_used--;
	  memset (memory->free_list[memory->free_used], 0, memory->sizeof_struct);
	  tmp = memory->free_list[memory->free_used];
	  tmp->size = size;
	  tmp->ptr = tmp + 1;

	  return (tmp->ptr);
	}

  for (i = 0; i != memory->list_used; ++i)
    {
      if (size == memory->allowed[i])
	{
	  memory->allowed[i] = 0;
	  memset((aasm_u8_t*)memory->list[i] - size, 0, size);
	  tmp = (struct resize_s*) ((aasm_u8_t*)memory->list[i] - size);
	  tmp->size = size;
	  tmp->ptr = tmp + 1;

	  return tmp;
	}
      /* FIXME : Howto to find the best size */
      if (memory->allowed[i] < nearest &&
	  memory->allowed[i] > size)
	{
	  nearest = memory->allowed[i];
	  index = i;
	}
    }

  if (index == -1)
    {
      if (memory->list_used + 1 > memory->list_max)
	{
	  unsigned int saved_size = memory->list_max;

	  memory->list_max += MEM_REALLOC_SIZE;
	  memory->list = xrealloc (memory->list, sizeof (void*) * memory->list_max);
	  memory->allowed = xrealloc (memory->allowed, sizeof (unsigned int) * memory->list_max);
	  memset (&(memory->list[saved_size]), 0, sizeof (void*) * MEM_REALLOC_SIZE);
	  memset (&(memory->allowed[saved_size]), 0, sizeof (unsigned int) * MEM_REALLOC_SIZE);
	}
      memory->list[memory->list_used] = xzalloc (MEM_REALLOC_SIZE * MEM_REALLOC_SIZE);
      memory->allowed[memory->list_used] = MEM_REALLOC_SIZE * MEM_REALLOC_SIZE - size;
      tmp = memory->list[memory->list_used++];
      tmp->size = size;
      tmp->ptr = tmp + 1;
    }
  else
    {
      unsigned int saved_index = memory->allowed[index];

      memory->allowed[index] -= size;
      tmp = (struct resize_s*) ((aasm_u8_t*) memory->list[index] + (MEM_REALLOC_SIZE * MEM_REALLOC_SIZE - saved_index));
      tmp->size = size;
      tmp->ptr = tmp + 1;
    }
  return (void*) tmp->ptr;
}

void		mem_resize_clean (struct resize_mem_s *memory)
{
  if (memory->free_list)
    free (memory->free_list);

  if (memory->list)
    {
      unsigned int i;
      for (i = 0; i != memory->list_used; ++i)
	free (*(memory->list + i));
      free (memory->list);
      free (memory->allowed);
      free (memory->freed_size);
    }

  memset (memory, 0, sizeof (struct resize_mem_s));
}

void		mem_resize_init (struct resize_mem_s *memory,
				 unsigned int size)
{
  memset (memory, 0, sizeof (struct resize_mem_s));
  size += sizeof (struct resize_s);

  /* Must preserve the alignement of allocated structure */
  ALIGN(size);

  memory->sizeof_struct = size;
}
