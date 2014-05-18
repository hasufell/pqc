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

#include "context.h"
#include "err.h"
#include "mem.h"
#include "poly.h"
#include "ascii_poly.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tommath.h>
#include <tompoly.h>

#define ASCII_DIGITS 7

/**
 * Converts a string into a pb_poly of the size strlen(to_poly) * 7.
 * 7 bit per ASCII symbol.
 *
 * @param to_poly the string
 * @return the newly allocated polynom.
 */
pb_poly *ascii_to_poly(char *to_poly)
{
	size_t length = (strlen(to_poly) * ASCII_DIGITS);
	char *tmp_ptr = to_poly;
	u_int8_t binary_Number[ASCII_DIGITS + 1];

	if (!to_poly) {
		return NULL;
	}

	mp_int chara;
	init_integer(&chara);

	pb_poly *poly = ntru_malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, length);

	/* for every char */
	for (u_int32_t j = 0; j < strlen(to_poly); j++) {
		u_int8_t quotient = (u_int8_t) *tmp_ptr++;
		u_int8_t k = ASCII_DIGITS;
		for (u_int8_t i = 1; i <= ASCII_DIGITS; i++) {
			/* gets the least significant bit in an array*/
			binary_Number[k--] = quotient % 2;
			/* bitshift so the next bit becomes the lsb*/
			quotient >>= 1;
		}
		for (u_int8_t i = 1; i <= ASCII_DIGITS; i++) {
			/* the actual position of the bit in the polynom */
			u_int32_t coefficient = (i - 1) + (j * ASCII_DIGITS);
			MP_SET(&(poly->terms[coefficient]), binary_Number[i]);
			/* set the array to 0 so the next run is garbage free */
			binary_Number[i] = 0;
			poly->terms[i].sign = 0;
		}
	}
	poly->used = (int) length;
	mp_clear(&chara);
	return poly;
}

/**
 * Converts a polynom into a newly allocated string.
 *
 * @param to_ascii the polynom you want to make a string of.
 * @return a pointer to the string ore a NULL pointer in the error case
 */
char *polynom_to_ascii(pb_poly *to_ascii)
{
	if (!to_ascii) {
		return NULL;
	}

	size_t length_poly = (size_t) to_ascii->used;
	size_t length_string = (size_t) (length_poly / ASCII_DIGITS);
	char *string = (char*) ntru_malloc(length_string);
	char bit_buffer;
	char *tmp_ptr = string;
	u_int8_t ascii_value = 0;

	/* every char */
	for (u_int32_t i = 0; i < length_poly; i += ASCII_DIGITS) {
		/* every bit*/
		for (u_int32_t j = 0; j < ASCII_DIGITS; j++) {
			/* get the bit */
			if (mp_toradix(&(to_ascii->terms[i + j]), &bit_buffer, 2)) {
				return NULL;
			}
			/* bit as integer */
			u_int8_t bit = atoi(&bit_buffer);
			/* bitshift to the left */
			ascii_value <<= 1;
			/* set the new bit and keep the other */
			ascii_value |= bit;
		}
		/* char into string */
		*tmp_ptr++ = (char) ascii_value;
		/* reset for next char */
		ascii_value = 0;
	}
	return string;
}
