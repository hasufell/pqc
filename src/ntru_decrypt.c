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
 * @file ntru_decrypt.c
 * This file handles the NTRU decryption
 * algorithm.
 * @brief NTRU decryption
 */

#include "ntru_ascii_poly.h"
#include "ntru_decrypt.h"
#include "ntru_mem.h"
#include "ntru_params.h"
#include "ntru_poly.h"
#include "ntru_poly_ascii.h"
#include "ntru_string.h"

#include <lz4.h>
#include <stdbool.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Decompress a string and return it, newly allocated.
 *
 * @param compr_str the compressed string to decompress
 * @return the decompressed string, newly allocated
 */
static string *
get_decompressed_str(const string *compr_str);


/*------------------------------------------------------------------------*/

static string *
get_decompressed_str(const string *compr_str)
{
	int out_len = 0;
	const uint32_t max_lz4_ratio = 3;
	uint32_t max_out_len = 0;
	string *decompressed_msg = NULL;

	if (!compr_str)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	max_out_len = compr_str->len * max_lz4_ratio;
	decompressed_msg = ntru_malloc(sizeof(string));
	decompressed_msg->ptr = ntru_malloc(
			/* this is more than needed, but safe */
			sizeof(char) * max_out_len);

	out_len = LZ4_decompress_safe(
			(const char *)compr_str->ptr,
			decompressed_msg->ptr,
			compr_str->len,
			max_out_len);

	if (out_len > 0)
		decompressed_msg->len = out_len;
	else
		NTRU_ABORT_DEBUG("Failed decompressing the message");

	return decompressed_msg;
}

/*------------------------------------------------------------------------*/

void
ntru_decrypt_poly(
		fmpz_poly_t out_bin,
		const fmpz_poly_t encr_msg,
		const fmpz_poly_t priv_key,
		const fmpz_poly_t priv_key_inv,
		const ntru_params *params)
{
	fmpz_poly_t a,
				priv_key_tmp,
				priv_key_inv_tmp,
				encr_msg_tmp;

	if (!encr_msg || !priv_key || !priv_key_inv || !out_bin || !params)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	fmpz_poly_init(a);
	fmpz_poly_zero(a);

	/*
	 * make sure all are shifted to
	 * [-q/2, q/2]
	 */
	fmpz_poly_init(priv_key_tmp);
	fmpz_poly_init(priv_key_inv_tmp);
	fmpz_poly_init(encr_msg_tmp);
	fmpz_poly_set(priv_key_tmp, priv_key);
	fmpz_poly_set(priv_key_inv_tmp, priv_key_inv);
	fmpz_poly_set(encr_msg_tmp, encr_msg);
	fmpz_poly_mod(priv_key_tmp, params->q);
	fmpz_poly_mod(priv_key_inv_tmp, params->q);
	fmpz_poly_mod(encr_msg_tmp, params->q);

	poly_starmultiply(a, priv_key_tmp, encr_msg_tmp, params, params->q);
	fmpz_poly_mod(a, params->q);
	poly_starmultiply(out_bin, a, priv_key_inv_tmp, params, params->p);
	fmpz_poly_mod(out_bin, params->p);

	fmpz_poly_clear(a);
	fmpz_poly_clear(priv_key_tmp);
	fmpz_poly_clear(priv_key_inv_tmp);
	fmpz_poly_clear(encr_msg_tmp);
}

/*------------------------------------------------------------------------*/

string *
ntru_decrypt_string(
		const string *encr_msg,
		const fmpz_poly_t priv_key,
		const fmpz_poly_t priv_key_inv,
		const ntru_params *params)
{
	uint32_t i = 0;
	string *decr_msg;
	fmpz_poly_t **poly_array;
	string *decompressed_msg = NULL;

	if (!encr_msg || !encr_msg->len)
		NTRU_ABORT_DEBUG("Unexpected NULL parameters");

	poly_array = base64_to_poly_arr(encr_msg, params);

	while (*poly_array[i]) {
		ntru_decrypt_poly(*poly_array[i],
					*poly_array[i],
					priv_key,
					priv_key_inv,
					params);
		i++;
	}

	decr_msg = bin_poly_arr_to_ascii((const fmpz_poly_t **)poly_array,
			i, params);

	decompressed_msg = get_decompressed_str(decr_msg);

	poly_delete_array(poly_array);
	string_delete(decr_msg);

	return decompressed_msg;
}

/*------------------------------------------------------------------------*/
