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

#include "err.h"

#include <stdio.h>
#include <tompoly.h>
#include <tommath.h>

/**
 * Initialize a mp_int and check if this was successful, the
 * caller must free new_int.
 *
 * @param new_int a pointer to the mp_int you want to initialize
 */
void init_integer(mp_int *new_int)
{
	int result;
	if ((result = mp_init(new_int)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initialize a Polynom with a pb_poly and a mp_int as characteristic.
 * Checks if everything went fine. The caller must free new_poly.
 *
 * @param new_poly the pb_poly you want to initialize
 * @param chara the characteristic
 */
void init_polynom(pb_poly *new_poly, mp_int *chara)
{
	int result;
	if ((result = pb_init(new_poly, chara)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initialize a Polynom with a pb_poly adn a mp_int as characteristic
 * with size. Checks if everything went fine. The caller must free
 * new_poly.
 *
 * @param new_poly the pb_poly you want to initialize
 * @param chara the characteristic
 * @param size the size of the polynomial
 */
void init_polynom_size(pb_poly *new_poly, mp_int *chara, int size)
{
	int result;
	if ((result = pb_init_size(new_poly, chara, size)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Print the polynomial in a human readable format to stdout.
 *
 * @param poly to draw
 */
void draw_polynom(pb_poly * const poly)
{
	int x;
	char buf[8192];

	if (poly->used == 0) {
		printf("0");
	} else {
		for (x = poly->used - 1; x >= 0; x--) {
			if (mp_iszero(&(poly->terms[x])) == MP_YES)
				continue;
			mp_toradix(&(poly->terms[x]), buf, 10);
			if ((x != poly->used - 1) && poly->terms[x].sign == MP_ZPOS) {
				printf("+");
			}
			printf(" %sx^%d ", buf, x);
		}
	}
	if (mp_iszero(&(poly->characteristic)) == MP_NO) {
		mp_toradix(&(poly->characteristic), buf, 10);
		printf(" (mod %s)", buf);
	}
	printf("\n");
}
