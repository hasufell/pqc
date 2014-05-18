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

#include "mem.h"
#include "poly.h"
#include "ascii_poly.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tompoly.h>
#include <tommath.h>

#define ASCII_DIGIETS 7

/**
 * Converts a string into a pb_poly of the size strlen(to_poly) * 7.
 * 7 bit per ASCII symbol.
 *
 * @param to_poly the string
 * @return the newly allocated polynom.
 */
pb_poly *ascii_to_poly(char *to_poly)
{
	size_t length = (strlen(to_poly) * ASCII_DIGIETS);
	char *tmp_ptr = to_poly;
	u_int8_t quotient,
			 i,
			 k,
			 binary_Number[ASCII_DIGIETS + 1];

	mp_int *chara = ntru_malloc(sizeof(mp_int));
	init_integer(chara);

	pb_poly *poly = ntru_malloc(sizeof(pb_poly));
	init_polynom_size(poly, chara, length);

	for (u_int32_t j = 0; j < strlen(to_poly); j++) {
		quotient = (u_int8_t) *tmp_ptr++;
		k = ASCII_DIGIETS;
		for (i = 1; i <= ASCII_DIGIETS; i++) {
			binary_Number[k--] = quotient % 2;
			quotient >>= 1;
		}
		for (i = 1; i <= ASCII_DIGIETS; i++) {
			mp_set(&(poly->terms[((i - 1) + (j * ASCII_DIGIETS))]),
					binary_Number[i]);
			binary_Number[i] = 0;
			poly->terms[i].sign = 0;
		}
	}
	poly->used = (int) length;
	mp_clear(chara);
	return poly;
}