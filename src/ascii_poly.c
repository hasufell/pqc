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
 * @file ascii_poly.c
 * This file allows to convert between ascii strings
 * and polynomials.
 * @brief asci->poly and poly->ascii
 */

#include "common.h"
#include "context.h"
#include "mem.h"
#include "poly.h"
#include "ascii_poly.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Convert an integer to it's binary representation
 * as a string and return it.
 *
 * @param value the integer to convert
 * @return the binary representation as a newly allocated string
 */
static char *
get_int_to_bin_str(uint8_t value);

/**
 * Converts a binary representation of multiple concatenated
 * integers to the corresponding array of ascii chars, which
 * is NULL-terminated.
 *
 * @param binary_rep the binary representation of multiple
 * integers concatenated
 * @return NULL-terminated array of corresponding ascii-chars,
 * newly allocated
 */
static char *
get_bin_arr_to_ascii(char *binary_rep);


static char *
get_int_to_bin_str(uint8_t value)
{
    int i;
	const size_t bin_string_size = ASCII_BITS + 1;
	char *bin_string = ntru_malloc(sizeof(*bin_string) *
			(bin_string_size)); /* account for trailing null-byte */

	/* terminate properly */
    bin_string[bin_string_size - 1] = '\0';

    for (i = ASCII_BITS - 1; i >= 0; --i, value >>= 1)
        bin_string[i] = (value & 1) + '0';

	return bin_string;
}

static char *
get_bin_arr_to_ascii(char *binary_rep)
{
	const size_t int_arr_size = strlen(binary_rep) / ASCII_BITS;
	uint8_t int_arr[int_arr_size];
	char *tmp_string = binary_rep;
	uint32_t i = 0;
	char *int_string;

	while (*tmp_string) {
		int_arr[i] = 0;
		for (uint32_t j = 0; j < ASCII_BITS; j++) {
			if (*tmp_string == '1')
				int_arr[i] = int_arr[i] * 2 + 1;
			else if (*tmp_string == '0')
				int_arr[i] *= 2;
			tmp_string++;
		}
		i++;
	}

	int_string = ntru_calloc(1, CHAR_SIZE * (i + 1));

	for (uint32_t j = 0; j < i; j++)
		int_string[j] = (char) int_arr[j];

	return int_string;
}

fmpz_poly_t **
ascii_to_tern_poly(char *to_poly, ntru_context *ctx)
{
	uint32_t i = 0,
			 polyc = 0;
	char *cur = to_poly;
	size_t out_size = CHAR_SIZE * (strlen(to_poly) * ASCII_BITS + 1);
	char *out = ntru_malloc(out_size);
	fmpz_poly_t **poly_array;

	*out = '\0';

	while (*cur) {
		char *tmp_string = get_int_to_bin_str((int)(*cur));
		strcat(out, tmp_string);
		cur++;
		free(tmp_string);
	}

	poly_array = ntru_malloc(sizeof(**poly_array) * (strlen(out) / ctx->N));

	while (out[i]) {
		uint32_t j = 0;
		fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

		fmpz_poly_init(*new_poly);
		poly_array[polyc] = new_poly;

		while (out[i] && j < ctx->N) {
			fmpz_poly_set_coeff_si(*new_poly,
					j,
					(out[i] == '0') ? -1 : 1);
			i++;
			j++;
		}

		/* fill the last poly with 2 */
		for (uint32_t i = j; i < ctx->N; i++) {
			fmpz_poly_set_coeff_si(*new_poly,
					i,
					2);
		}

		polyc++;
	}

	free(out);

	poly_array[polyc] = NULL;

	return poly_array;
}

fmpz_poly_t **
ascii_to_poly(string *to_poly, ntru_context *ctx)
{
	uint32_t i = 0,
			 polyc = 0;
	fmpz_poly_t **poly_array;

	poly_array = ntru_malloc(sizeof(**poly_array) *
			(strlen(to_poly->ptr) / ctx->N));

	while (i < to_poly->len) {
		uint32_t j = 0;
		fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

		fmpz_poly_init(*new_poly);
		poly_array[polyc] = new_poly;

		while (i < to_poly->len && j < ctx->N) {
			fmpz_poly_set_coeff_si(*new_poly,
					j,
					(uint8_t)(to_poly->ptr[i]));
			i++;
			j++;
		}

		/* fill the last poly with q (which is a non-standard
		 * coefficient) */
		for (uint32_t i = j; i < ctx->N; i++) {
			fmpz_poly_set_coeff_si(*new_poly,
					i,
					ctx->q);
		}

		polyc++;

	}

	poly_array[polyc] = NULL;

	return poly_array;
}

char *
tern_poly_to_ascii(fmpz_poly_t **tern_poly_arr, ntru_context *ctx)
{
	fmpz_poly_t *ascii_poly;
	char *binary_rep = NULL;
	char *ascii_string;
	uint32_t i = 0;
	size_t old_length = 0,
		   new_length;

	/*
	 * parse the polynomial coefficients into a string
	 */
	binary_rep = ntru_malloc(CHAR_SIZE * (ctx->N + 1));
	while ((ascii_poly = *tern_poly_arr++)) {
		new_length = CHAR_SIZE * (ctx->N + 1);

		REALLOC(binary_rep,
				old_length +
				new_length +
				1); /* trailing null byte */

		old_length += new_length;

		for (uint32_t j = 0; j < ctx->N; j++) {
			fmpz *coeff = fmpz_poly_get_coeff_ptr(*ascii_poly, j);

			if (coeff) {
				if (fmpz_cmp_si(coeff, 1))
					binary_rep[i] = '0';
				else if (fmpz_cmp_si(coeff, -1))
					binary_rep[i] = '1';
				else if (fmpz_cmp_si(coeff, 2))
					binary_rep[i] = '0';
			}
			i++;
		}
	}

	binary_rep[i] = '\0';

	ascii_string = get_bin_arr_to_ascii(binary_rep);
	free(binary_rep);

	return ascii_string;
}

string *
poly_to_ascii(fmpz_poly_t **poly_array,
		ntru_context *ctx)
{
	fmpz_poly_t *ascii_poly;
	char *string_rep = NULL;
	string *result_string = ntru_malloc(sizeof(*result_string));
	uint32_t i = 0;
	size_t old_length = 0,
		   new_length;

	/*
	 * parse the polynomial coefficients into a string
	 */
	string_rep = ntru_malloc(CHAR_SIZE * (ctx->N + 1));
	while ((ascii_poly = *poly_array++)) {
		new_length = CHAR_SIZE * (ctx->N + 1);

		REALLOC(string_rep,
				old_length +
				new_length +
				1); /* trailing null byte */

		old_length += new_length;

		for (uint32_t j = 0; j < ctx->N; j++) {
			uint8_t coeff = fmpz_poly_get_coeff_ui(*ascii_poly, j);
			if (coeff == ctx->q)
				string_rep[i] = '\0';
			else
				string_rep[i] = (char)coeff;
			i++;
		}
	}

	string_rep[i] = '\0';

	result_string->ptr = string_rep;
	result_string->len = i;

	return result_string;
}
