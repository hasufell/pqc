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
 * @file ntru_string.h
 * Header for the external API of ntru_string.c.
 * @brief header for ntru_string.c
 */

#ifndef NTRU_STRING_H
#define NTRU_STRING_H


#include <stdlib.h>


typedef struct string string;


/**
 * Represents a string.
 */
struct string {
	/**
	 * Pointer to the char array,
	 * holding the actual string.
	 */
	char *ptr;
	/**
	 * Length of the string.
	 */
	size_t len;
};


#endif /* NTRU_STRING_H */
