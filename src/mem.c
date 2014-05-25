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
 * @file mem.c
 * This file provides functions for
 * memory management.
 * @brief memory management
 */

#include "mem.h"

#include <stdio.h>
#include <stdlib.h>


/**
 * Allocate memory of size and return
 * a void pointer.
 *
 * @param size of the memory to allocate in bytes
 * @return void pointer to the beginning of the allocated memory block
 */
void *ntru_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);

	if (size)
		if (!ptr) {
			fprintf(stderr, "failed to allocate memory, aborting!");
			abort();
		}

	return ptr;
}

/**
 * Allocate memory of size and return
 * a void pointer. The memory is zeroed.
 *
 * @param nmemb amount of blocks to allocate
 * @param size of the memory blocks to allocate in bytes
 * @return void pointer to the beginning of the allocated memory block
 */
void *ntru_calloc(size_t nmemb, size_t size)
{
	void *ptr;

	ptr = calloc(nmemb, size);

	if (size)
		if (!ptr) {
			fprintf(stderr, "failed to allocate memory, aborting!");
			abort();
		}

	return ptr;
}
