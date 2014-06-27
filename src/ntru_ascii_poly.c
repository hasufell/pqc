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
 * @file ntru_ascii_poly.c
 * This file allows to convert ascii strings, including
 * base64 encoded ones, to polynomials.
 * @brief ascii to polynomials
 */

#include "ntru_ascii_poly.h"
#include "ntru_common.h"
#include "ntru_mem.h"
#include "ntru_params.h"
#include "ntru_poly.h"
#include "ntru_string.h"

#include <glib.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Convert an integer to it's binary representation
 * as a char array and return it (not NULL terminated).
 *
 * As in: 90 => "10110101"
 *
 * @param value the integer to convert
 * @return the binary representation as a newly allocated char array
 * (not NULL terminated)
 */
static char *
get_int_to_bin_str(uint8_t value);


/*------------------------------------------------------------------------*/

static char *
get_int_to_bin_str(uint8_t value)
{
    int i;
	const size_t bin_string_size = ASCII_BITS;
	char *bin_string = ntru_malloc(sizeof(*bin_string) *
			(bin_string_size));

    for (i = ASCII_BITS - 1; i >= 0; --i, value >>= 1)
        bin_string[i] = (value & 1) + '0';

	return bin_string;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t *
ascii_bin_to_bin_poly(const char *to_poly, const ntru_params *params)
{
	uint32_t i = 0;
	fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

	fmpz_poly_init(*new_poly);

	while (to_poly[i] && i < params->N) {
		fmpz_poly_set_coeff_si(*new_poly,
				i,
				(to_poly[i] == '0') ? -1 : 1);
		i++;
	}

	return new_poly;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t **
ascii_to_bin_poly_arr(const string *to_poly, const ntru_params *params)
{
	char *cur = to_poly->ptr;
	char *out = ntru_malloc(CHAR_SIZE * (to_poly->len * ASCII_BITS + 1));
	uint32_t polyc = 0;
	size_t out_len = 0;
	fmpz_poly_t **poly_array;

	for (uint32_t i = 0; i < to_poly->len; i++) {
		char *tmp_string = get_int_to_bin_str((int)(*cur));
		memcpy(out + out_len, tmp_string, ASCII_BITS);
		out_len += ASCII_BITS;
		cur++;
		free(tmp_string);
	}
	out[out_len] = '\0';

	poly_array = ntru_malloc(sizeof(**poly_array) *
			(strlen(out) / params->N + 1));

	for (uint32_t i = 0; i < out_len; i += params->N) {
		char chunk[params->N + 1];
		size_t real_chunk_size;

		real_chunk_size =
			(out_len - i > params->N) ? params->N : out_len - i;

		memcpy(chunk, out + i, real_chunk_size);
		chunk[real_chunk_size] = '\0';

		poly_array[polyc] = ascii_bin_to_bin_poly(chunk, params);

		polyc++;
	}

	free(out);

	poly_array[polyc] = NULL;

	return poly_array;
}

/*------------------------------------------------------------------------*/

fmpz_poly_t **
base64_to_poly_arr(const string *to_poly, const ntru_params *params)
{
	uint32_t i = 0,
			 polyc = 0;
	gsize out_len;
	guchar *base64_decoded = NULL;
	string *new_string = ntru_malloc(sizeof(*new_string));
	fmpz_poly_t **poly_array;
	char *tmp = ntru_malloc(sizeof(char) * (to_poly->len + 1));

	/* g_base64_decode() needs it null-terminated */
	memcpy(tmp, to_poly->ptr, to_poly->len);
	tmp[to_poly->len] = '\0';

	base64_decoded = g_base64_decode((const gchar *)tmp, &out_len);

	new_string->ptr = (char *)base64_decoded;
	new_string->len = (unsigned long)(out_len);

	poly_array = ntru_malloc(sizeof(**poly_array) *
			(new_string->len / params->N));

	while (i < new_string->len) {
		uint32_t j = 0;
		fmpz_poly_t *new_poly = ntru_malloc(sizeof(*new_poly));

		fmpz_poly_init(*new_poly);

		while (j < params->N) {
			fmpz_poly_set_coeff_si(*new_poly,
					j,
					(uint8_t)(base64_decoded[i]));
			j++;
			i++;
		}

		/* fill the last poly with q (which is a non-standard
		 * coefficient) */
		for (uint32_t k = j; k < params->N; k++) {
			fmpz_poly_set_coeff_si(*new_poly,
					k,
					params->q);
		}

		poly_array[polyc] = new_poly;
		polyc++;
	}

	poly_array[polyc] = NULL;

	string_delete(new_string);
	free(tmp);

	return poly_array;
}
