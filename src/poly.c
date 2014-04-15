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

#include <stdbool.h>
#include <stdio.h>
#include <tompoly.h>
#include <tommath.h>

/**
 * Initialize a mp_int and check if this was successful, the
 * caller must free new_int with mp_clear().
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
 * Checks if everything went fine. The caller must free new_poly
 * with pb_clear().
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
 * Initialize a Polynom with a pb_poly and an mp_int as characteristic
 * with size. Checks if everything went fine. The caller must free
 * new_poly with pb_clear().
 *
 * @param new_poly the pb_poly you want to initialize
 * @param chara the characteristic
 * @param size the size of the polynomial
 */
void init_polynom_size(pb_poly *new_poly, mp_int *chara, size_t size)
{
	int result;
	if ((result = pb_init_size(new_poly, chara, size)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initializes and builds a polynomial with the
 * coefficient values of c[] of size len within NTRU
 * context ctx and returns a newly allocated polynomial
 * pointer which is not clamped.
 *
 * If you want to fill a polyonmial of length 11 with zeros,
 * call build_polynom(NULL, 11, ctx).
 *
 * @param c array of polynomial coefficients, can be NULL
 * @param len size of the coefficient array, can be 0
 * @param ctx NTRU context
 * @return newly allocated polynomial pointer, must be freed
 * with delete_polynom()
 */
pb_poly *build_polynom(int const * const c,
		const size_t len,
		ntru_context *ctx)
{
	pb_poly *new_poly;
	mp_int chara;

	new_poly = malloc(sizeof(*new_poly));
	init_integer(&chara);
	init_polynom_size(new_poly, &chara, len);
	mp_clear(&chara);

	/* fill the polynom if c is not NULL */
	if (c) {
		for (unsigned int i = 0; i < len; i++) {
			bool sign = false;
			unsigned long unsigned_c;

			if (c[i] < 0) {
				unsigned_c = 0 - c[i];
				sign = true;
			} else {
				unsigned_c = c[i];
			}

			mp_set_int(&(new_poly->terms[i]), unsigned_c);

			if (sign == true)
				new_poly->terms[i].sign = 1;
		}
	} else { /* fill with zeros */
		for (unsigned int i = 0; i < len; i++)
			mp_set(&(new_poly->terms[i]), 0);
	}

	new_poly->used = len;

	return new_poly;
}

/**
 * This deletes the internal structure of a polynomial,
 * and frees the pointer. Don't call this on stack variables,
 * this is intended for use after ntru_ functions, that
 * return a polynomial pointer.
 *
 * @param poly the polynomial to delete
 */
void delete_polynom(pb_poly *poly)
{
	pb_clear(poly);
	free(poly);
}

/**
 * Starmultiplication, as follows:
 * c = a * b mod x^(N − 1)
 *
 * @param a polynom to multiply
 * @param b polynom to multiply
 * @param c polynom [out]
 * @param ctx NTRU context
 * @param modulus whether we use p or q
 */
void pb_starmultiply(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		ntru_context *ctx,
		unsigned int modulus)
{
	for (int k = ctx->N - 1; k >= 0; k--) {
		int j;
		j = k + 1;

		for (int i = ctx->N - 1; i >= 0; i--) {
			if (j == (int)(ctx->N))
				j = 0;
			if (mp_cmp_d(&(a->terms[i]), (mp_digit)0) != MP_EQ &&
					mp_cmp_d(&(b->terms[j]), (mp_digit)0) != MP_EQ) {
				int result;
				mp_int mp_modulus;
				mp_int mp_tmp;

				init_integer(&mp_tmp);
				init_integer(&mp_modulus);
				mp_set_int(&mp_modulus, (unsigned long)(modulus));

				if ((result = mp_mul(&(a->terms[i]),
								&(b->terms[j]), &mp_tmp)) != MP_OKAY)
					NTRU_ABORT("Error multiplying terms. %s",
							mp_error_to_string(result));
				if ((result = mp_add(&(c->terms[k]),
								&mp_tmp, &(c->terms[k]))) != MP_OKAY)
					NTRU_ABORT("Error adding terms. %s",
							mp_error_to_string(result));
				if ((result = mp_mod(&(c->terms[k]),
								&mp_modulus, &(c->terms[k]))) != MP_OKAY)
					NTRU_ABORT("Error redrucing term by modulo. %s",
							mp_error_to_string(result));

				mp_clear(&mp_modulus);
				mp_clear(&mp_tmp);
			}
			j++;
		}
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
