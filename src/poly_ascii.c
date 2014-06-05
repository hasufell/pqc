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
 * @file poly_ascii.c
 * This file allows to convert polynomials to
 * ascii strings, including base64 encoded.
 * @brief polynomials to acii
 */

#include "poly_ascii.h"
#include "common.h"
#include "mem.h"
#include "ntru_string.h"
#include "params.h"
#include "poly.h"

#include <glib.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Converts a binary representation of multiple concatenated
 * integers to the corresponding array of ascii chars, which
 * is NULL-terminated.
 *
 * It reads in 8bit chunks, as in:
 *
 * 10110101|00111100|01011001 => 90|60|89 => "Z<Y"
 *
 * Chunks that are 00000000 are stripped from the result.
 *
 * @param binary_rep the binary representation of multiple
 * integers concatenated
 * @return string of corresponding ascii-chars,
 * newly allocated
 */
static string *
get_bin_arr_to_ascii(const char *binary_rep);


/*------------------------------------------------------------------------*/

static string *
get_bin_arr_to_ascii(const char *binary_rep)
{
	size_t int_arr_size = 0;
	uint8_t *int_arr = NULL;
	uint32_t i = 0;
	char *int_string = NULL;
	string *result = ntru_malloc(sizeof(*result));

	if (!binary_rep || !*binary_rep)
		return NULL;

	int_arr_size = strlen(binary_rep) / ASCII_BITS + 1;
	int_arr = ntru_malloc(sizeof(*int_arr) * int_arr_size);

	while (*binary_rep) {
		int_arr[i] = 0;

		/* convert one binary integer to real integer */
		for (uint32_t j = 0; j < ASCII_BITS && *binary_rep; j++) {
			if (*binary_rep == '1')
				int_arr[i] = int_arr[i] * 2 + 1;
			else if (*binary_rep == '0')
				int_arr[i] *= 2;
			binary_rep++;
		}

		i++; /* amount of real integers */
	}

	int_string = ntru_malloc(CHAR_SIZE * (i + 1));

	for (uint32_t j = 0; j < i; j++)
		int_string[j] = (char) int_arr[j];

	result->ptr = int_string;
	result->len = i;

	free(int_arr);

	return result;
}

/*------------------------------------------------------------------------*/

string *
bin_poly_to_ascii(const fmpz_poly_t poly,
		const ntru_params *params)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	char *binary_rep = ntru_malloc(CHAR_SIZE * (params->N));
	uint32_t i = 0;

	for (uint32_t j = 0; j < params->N; j++) {
		fmpz *coeff = fmpz_poly_get_coeff_ptr(poly, j);

		if (coeff) {
			if (!fmpz_cmp_si(coeff, 1))
				binary_rep[i] = '1';
			else if (!fmpz_cmp_si(coeff, -1))
				binary_rep[i] = '0';
		} else {
			break;
		}

		i++;
	}

	result_string->ptr = binary_rep;
	result_string->len = i;

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
bin_poly_arr_to_ascii(fmpz_poly_t **bin_poly_arr,
		const ntru_params *params)
{
	fmpz_poly_t *ascii_poly;
	char *binary_rep = NULL;
	size_t string_len = 0;
	string *ascii_string = NULL;
	size_t old_length = 0,
		   new_length;

	/*
	 * parse the polynomial coefficients into a string
	 */
	binary_rep = ntru_calloc(1, CHAR_SIZE * (params->N + 1));
	while ((ascii_poly = (fmpz_poly_t *)*bin_poly_arr++)) {
		string *single_poly_string = NULL;

		new_length = CHAR_SIZE * (params->N);

		REALLOC(binary_rep,
				old_length +
				new_length +
				1); /* trailing null byte */

		old_length += new_length;

		single_poly_string = bin_poly_to_ascii(*ascii_poly, params);

		memcpy(binary_rep + string_len,
				single_poly_string->ptr,
				single_poly_string->len);

		string_len += single_poly_string->len;

		string_delete(single_poly_string);
	}
	binary_rep[string_len] = '\0';

	ascii_string = get_bin_arr_to_ascii(binary_rep);

	free(binary_rep);

	return ascii_string;
}

/*------------------------------------------------------------------------*/

string *
poly_to_ascii(const fmpz_poly_t poly,
		const ntru_params *params)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	char *string_rep = ntru_malloc(CHAR_SIZE * (params->N));

	for (uint32_t j = 0; j < params->N; j++) {
		uint8_t coeff = fmpz_poly_get_coeff_ui(poly, j);
		if (coeff == params->q)
			string_rep[j] = '\0';
		else
			string_rep[j] = (char)coeff;
	}

	result_string->ptr = string_rep;
	result_string->len = params->N;

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
poly_arr_to_ascii(fmpz_poly_t **poly_array,
		const ntru_params *params)
{
	fmpz_poly_t *ascii_poly;
	char *string_rep = NULL;
	size_t string_len = 0;
	string *result_string = ntru_malloc(sizeof(*result_string));
	size_t old_length = 0,
		   new_length;

	/*
	 * parse the polynomial coefficients into a string
	 */
	string_rep = ntru_calloc(1, CHAR_SIZE * (params->N + 1));
	while ((ascii_poly = *poly_array++)) {
		string *poly_str;

		poly_str = poly_to_ascii(*ascii_poly, params);

		new_length = CHAR_SIZE * poly_str->len;
		REALLOC(string_rep,
				old_length +
				new_length);
		old_length += new_length;

		memcpy(string_rep + string_len,
				poly_str->ptr,
				poly_str->len);
		string_len += poly_str->len;

		string_delete(poly_str);
	}

	result_string->ptr = string_rep;
	result_string->len = string_len;

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
poly_to_base64(const fmpz_poly_t poly,
		const ntru_params *params)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	string *string_rep = NULL;
	gchar *base64_string = NULL,
		  *tmp = NULL;

	string_rep = poly_to_ascii(poly, params);

	tmp = g_base64_encode((const guchar *)string_rep->ptr,
			string_rep->len);
	base64_string = g_base64_encode((const guchar *)tmp,
			strlen(tmp));

	result_string->ptr = base64_string;
	result_string->len = strlen(base64_string);

	string_delete(string_rep);
	free(tmp);

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
poly_arr_to_base64(fmpz_poly_t **poly_array,
		const ntru_params *params)
{
	string *string_rep;
	string *result_string = ntru_malloc(sizeof(*result_string));

	gchar *base64_string = NULL,
		  *tmp = NULL;

	string_rep = poly_arr_to_ascii(poly_array, params);

	tmp = g_base64_encode((const guchar *)string_rep->ptr, string_rep->len);
	base64_string = g_base64_encode((const guchar *)tmp,
			strlen(tmp));

	result_string->ptr = base64_string;
	result_string->len = strlen(base64_string);

	string_delete(string_rep);
	free(tmp);

	return result_string;
}

/*------------------------------------------------------------------------*/
