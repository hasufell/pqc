/*
 * Copyright (C) 2014 FH Bielefeld
 *
 * This file is part of a FH Bielefeld project.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

/**
 * @file ntru_mem.h
 * Header for the internal API of ntru_mem.c.
 * @brief header for ntru_mem.c
 */

#ifndef NTRU_MEM_H
#define NTRU_MEM_H

#include <stdlib.h>


/**
 * Realloc macro which checks if reallocation
 * worked via a temporary pointer.
 */
#define REALLOC(ptr, size) \
{ \
	void *tmp_ptr = NULL; \
	tmp_ptr = realloc(ptr, size); \
	if (tmp_ptr == NULL) { \
		fprintf(stderr,"NULL Pointer in %s [%d]",__FILE__,__LINE__); \
		abort(); \
	} \
	ptr = tmp_ptr; \
}


/**
 * Allocate memory of size and return
 * a void pointer.
 *
 * @param size of the memory to allocate in bytes
 * @return void pointer to the beginning of the allocated memory block
 */
void *
ntru_malloc(size_t size);

/**
 * Allocate memory of size and return
 * a void pointer. The memory is zeroed.
 *
 * @param nmemb amount of blocks to allocate
 * @param size of the memory blocks to allocate in bytes
 * @return void pointer to the beginning of the allocated memory block
 */
void *
ntru_calloc(size_t nmemb, size_t size);


#endif /* NTRU_MEM_H */
