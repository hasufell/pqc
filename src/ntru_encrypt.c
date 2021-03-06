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
 * @file ntru_encrypt.c
 * This file handles the NTRU encryption
 * algorithm.
 * @brief NTRU encryption
 */

#include "ntru_ascii_poly.h"
#include "ntru_encrypt.h"
#include "ntru_mem.h"
#include "ntru_params.h"
#include "ntru_poly.h"
#include "ntru_poly_ascii.h"
#include "ntru_string.h"

#include <lz4.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Compress a string and return it, newly allocated.
 *
 * @param str the string to compress
 * @return the compressed string, newly allocated
 */
static string *
get_compressed_str(const string *str);


/*------------------------------------------------------------------------*/

static string *
get_compressed_str(const string *str)
{
	int out_len = 0;
	string *compressed_str;
	uint32_t max_output_size;

	if (!str)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	max_output_size = str->len + 1;
	compressed_str = ntru_malloc(sizeof(string));
	compressed_str->ptr = ntru_malloc(
			sizeof(char) * max_output_size);
	out_len = LZ4_compress(
			(const char*) str->ptr,
			compressed_str->ptr,
			str->len);

	if (out_len > 0)
		compressed_str->len = out_len;
	else
		NTRU_ABORT_DEBUG("Failed compressing the message");

	return compressed_str;
}

/*------------------------------------------------------------------------*/

void
ntru_encrypt_poly(
		fmpz_poly_t out,
		const fmpz_poly_t msg_bin,
		const fmpz_poly_t pub_key,
		const fmpz_poly_t rnd,
		const ntru_params *params)
{
	fmpz_poly_t tmp_poly_msg;

	if (!msg_bin || !pub_key || !rnd || !out || !params)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	/* allow aliasing */
	fmpz_poly_init(tmp_poly_msg);
	fmpz_poly_set(tmp_poly_msg, msg_bin);

	fmpz_poly_zero(out);
	poly_starmultiply(out, pub_key, rnd, params, params->q);

	fmpz_poly_add(out, out, tmp_poly_msg);
	fmpz_poly_mod_unsigned(out, params->q);

	fmpz_poly_clear(tmp_poly_msg);
}

/*------------------------------------------------------------------------*/

string *
ntru_encrypt_string(
		const string *msg,
		const fmpz_poly_t pub_key,
		const fmpz_poly_t rnd,
		const ntru_params *params)
{
	uint32_t i = 0;
	string *enc_msg;
	fmpz_poly_t **poly_array;
	string *compressed_msg;

	if (!msg || !msg->len)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	compressed_msg = get_compressed_str(msg);

	poly_array = ascii_to_bin_poly_arr(compressed_msg, params);

	while (*poly_array[i]) {
		ntru_encrypt_poly(*poly_array[i],
				*poly_array[i],
				pub_key,
				rnd,
				params);
		i++;
	}

	enc_msg = poly_arr_to_base64((const fmpz_poly_t **)poly_array,
			i, params);

	poly_delete_array(poly_array);
	string_delete(compressed_msg);

	return enc_msg;
}

/*------------------------------------------------------------------------*/
