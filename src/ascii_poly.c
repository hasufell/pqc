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

/* TODO: clean up some iterators */
/* TODO: ascii_to_bin_poly() should accept a string, not a c_str */

/**
 * @file ascii_poly.c
 * This file allows to convert between ascii strings
 * and polynomials.
 * @brief asci->poly and poly->ascii
 */

#include "ascii_poly.h"
#include "common.h"
#include "context.h"
#include "mem.h"
#include "ntru_string.h"
#include "poly.h"

#include <glib.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Convert an integer to it's binary representation
 * as a string and return it.
 *
 * As in: 90 => "10110101"
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
 * It reads in 8bit chunks, as in:
 *
 * 10110101|00111100|01011001 => 90|60|89 => "Z<Y"
 *
 * @param binary_rep the binary representation of multiple
 * integers concatenated
 * @return NULL-terminated array of corresponding ascii-chars,
 * newly allocated
 */
static char *
get_bin_arr_to_ascii(char *binary_rep);


/*------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------*/

static char *
get_bin_arr_to_ascii(char *binary_rep)
{
	size_t int_arr_size = 0;
	uint8_t *int_arr = NULL;
	uint32_t i = 0;
	char *int_string = NULL;

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

	int_string = ntru_calloc(1, CHAR_SIZE * (i + 1));

	for (uint32_t j = 0; j < i; j++)
		int_string[j] = (char) int_arr[j];

	free(int_arr);

	return int_string;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t *
ascii_bin_to_bin_poly(char *to_poly, ntru_context *ctx)
{
	uint32_t i = 0;
	fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

	fmpz_poly_init(*new_poly);

	while (to_poly[i] && i < ctx->N) {
		fmpz_poly_set_coeff_si(*new_poly,
				i,
				(to_poly[i] == '0') ? -1 : 1);
		i++;
	}

	/* fill the last poly with 2 */
	for (uint32_t j = i; j < ctx->N; j++) {
		fmpz_poly_set_coeff_si(*new_poly,
				i,
				2);
	}

	return new_poly;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t **
ascii_to_bin_poly_arr(char *to_poly, ntru_context *ctx)
{
	uint32_t polyc = 0;
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

	poly_array = ntru_malloc(sizeof(**poly_array) *
			(strlen(out) / ctx->N));

	for (uint32_t i = 0; i < strlen(out); i += ctx->N) {
		char chunk[ctx->N + 1];
		size_t real_chunk_size;

		real_chunk_size =
			(strlen(out + i) > ctx->N) ? ctx->N : strlen(out + i);

		memcpy(chunk, out + i, real_chunk_size);
		chunk[real_chunk_size] = '\0';

		poly_array[polyc] = ascii_bin_to_bin_poly(chunk, ctx);

		polyc++;
	}

	free(out);

	poly_array[polyc] = NULL;

	return poly_array;
}

/*------------------------------------------------------------------------*/

string *
bin_poly_to_ascii(fmpz_poly_t poly,
		ntru_context *ctx)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	char *binary_rep = ntru_malloc(CHAR_SIZE * (ctx->N));
	uint32_t i = 0;

	for (uint32_t j = 0; j < ctx->N; j++) {
		fmpz *coeff = fmpz_poly_get_coeff_ptr(poly, j);

		if (coeff) {
			if (fmpz_cmp_si(coeff, 1))
				binary_rep[i] = '0';
			else if (fmpz_cmp_si(coeff, -1))
				binary_rep[i] = '1';
			else if (fmpz_cmp_si(coeff, 2))
				binary_rep[i] = '0';
		} else {
			binary_rep[i] = '0';
		}
		i++;
	}

	result_string->ptr = binary_rep;
	result_string->len = i;

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
bin_poly_arr_to_ascii(fmpz_poly_t **bin_poly_arr, ntru_context *ctx)
{
	fmpz_poly_t *ascii_poly;
	char *binary_rep = NULL;
	size_t string_len = 0;
	char *ascii_string = NULL;
	string *result_string = ntru_malloc(sizeof(*result_string));
	size_t old_length = 0,
		   new_length;

	/*
	 * parse the polynomial coefficients into a string
	 */
	binary_rep = ntru_calloc(1, CHAR_SIZE * (ctx->N + 1));
	while ((ascii_poly = *bin_poly_arr++)) {
		string *single_poly_string = NULL;

		new_length = CHAR_SIZE * (ctx->N);

		REALLOC(binary_rep,
				old_length +
				new_length +
				1); /* trailing null byte */

		old_length += new_length;

		single_poly_string = bin_poly_to_ascii(*ascii_poly, ctx);

		memcpy(binary_rep + string_len,
				single_poly_string->ptr,
				single_poly_string->len);

		string_len += single_poly_string->len;

		string_delete(single_poly_string);
	}
	binary_rep[string_len] = '\0';

	ascii_string = get_bin_arr_to_ascii(binary_rep);

	result_string->ptr = ascii_string;
	result_string->len = strlen(ascii_string);

	free(binary_rep);

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
poly_to_ascii(fmpz_poly_t poly,
		ntru_context *ctx)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	char *string_rep = ntru_malloc(CHAR_SIZE * (ctx->N));
	uint32_t i = 0;

	for (uint32_t j = 0; j < ctx->N; j++) {
		uint8_t coeff = fmpz_poly_get_coeff_ui(poly, j);
		if (coeff == ctx->q)
			string_rep[i] = '\0';
		else
			string_rep[i] = (char)coeff;
		i++;
	}

	result_string->ptr = string_rep;
	result_string->len = i;

	return result_string;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t **
base64_to_poly_arr(string *to_poly, ntru_context *ctx)
{
	uint32_t i = 0,
			 polyc = 0;
	gsize out_len;
	guchar *base64_decoded = NULL,
		   *base_tmp = NULL;
	string *new_string = ntru_malloc(sizeof(*new_string));
	fmpz_poly_t **poly_array;
	char *tmp = ntru_malloc(sizeof(char) * (to_poly->len + 1));

	/* g_base64_decode() needs it null-terminated */
	memcpy(tmp, to_poly->ptr, to_poly->len);
	tmp[to_poly->len] = '\0';

	base_tmp = g_base64_decode((const gchar *)tmp, &out_len);

	/* g_base64_decode() needs it null-terminated */
	REALLOC(tmp, sizeof(char) * (out_len + 1));
	memcpy(tmp, base_tmp, out_len);
	tmp[out_len] = '\0';

	base64_decoded = g_base64_decode((const gchar *)tmp, &out_len);

	new_string->ptr = (char *)base64_decoded;
	new_string->len = (unsigned long)(out_len);

	poly_array = ntru_malloc(sizeof(**poly_array) *
			(strlen(new_string->ptr) / ctx->N));

	while (i < new_string->len) {
		uint32_t j = 0;
		fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

		fmpz_poly_init(*new_poly);

		while (j < ctx->N) {
			fmpz_poly_set_coeff_si(*new_poly,
					j,
					(uint8_t)(base64_decoded[i]));
			j++;
			i++;
		}

		/* fill the last poly with q (which is a non-standard
		 * coefficient) */
		for (uint32_t k = j; k < ctx->N; k++) {
			fmpz_poly_set_coeff_si(*new_poly,
					k,
					ctx->q);
		}

		poly_array[polyc] = new_poly;
		polyc++;
	}

	poly_array[polyc] = NULL;

	string_delete(new_string);
	free(base_tmp);
	free(tmp);

	return poly_array;
}

/*------------------------------------------------------------------------*/

string *
poly_to_base64(fmpz_poly_t poly,
		ntru_context *ctx)
{
	string *result_string = ntru_malloc(sizeof(*result_string));
	string *string_rep = NULL;
	gchar *base64_string = NULL,
		  *tmp = NULL;

	string_rep = poly_to_ascii(poly, ctx);

	tmp = g_base64_encode((const guchar *)string_rep->ptr,
			string_rep->len);

	base64_string = g_base64_encode((const guchar *)tmp,
			strlen(tmp));

	result_string->ptr = base64_string;
	result_string->len = strlen(base64_string);

	free(string_rep);
	free(tmp);

	return result_string;
}

/*------------------------------------------------------------------------*/

string *
poly_arr_to_base64(fmpz_poly_t **poly_array,
		ntru_context *ctx)
{
	fmpz_poly_t *ascii_poly;
	char *string_rep = NULL;
	size_t string_len = 0;
	string *result_string = ntru_malloc(sizeof(*result_string));
	size_t old_length = 0,
		   new_length;
	gchar *base64_string = NULL,
		  *tmp = NULL;

	/*
	 * parse the polynomial coefficients into a string
	 */
	string_rep = ntru_calloc(1, CHAR_SIZE * (ctx->N + 1));
	while ((ascii_poly = *poly_array++)) {
		string *poly_str;

		poly_str = poly_to_ascii(*ascii_poly, ctx);

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

	tmp = g_base64_encode((const guchar *)string_rep, string_len);
	base64_string = g_base64_encode((const guchar *)tmp,
			strlen(tmp));

	result_string->ptr = base64_string;
	result_string->len = strlen(base64_string);

	free(string_rep);
	free(tmp);

	return result_string;
}

/*------------------------------------------------------------------------*/
