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
 * @file ntru_string.c
 * Sets up a string type that does not suck
 * like C strings and provides operations on it.
 * @brief string type and operations
 */

#include "ntru_string.h"

#include <stdio.h>


/*------------------------------------------------------------------------*/

void
prints(string *print_string)
{
	for (size_t i = 0; i < print_string->len; i++)
		printf("%c", print_string->ptr[i]);
}


/*------------------------------------------------------------------------*/

void
string_delete(string *del_string)
{
	free(del_string->ptr);
	free(del_string);
}

/*------------------------------------------------------------------------*/
