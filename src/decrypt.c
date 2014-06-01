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
 * @file decrypt.c
 * This file handles the NTRU decryption
 * algorithm.
 * @brief NTRU decryption
 */

#include "ascii_poly.h"
#include "decrypt.h"
#include "ntru_string.h"

#include <stdbool.h>
#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/*------------------------------------------------------------------------*/

bool
ntru_decrypt_poly(
		const fmpz_poly_t encr_msg,
		const fmpz_poly_t priv_key,
		const fmpz_poly_t priv_key_inv,
		fmpz_poly_t out_bin,
		const ntru_context *ctx)
{
	fmpz_poly_t a,
				priv_key_tmp,
				priv_key_inv_tmp,
				encr_msg_tmp;

	if (!encr_msg || !priv_key || !priv_key_inv || !out_bin || !ctx)
		return false;

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
	fmpz_poly_mod(priv_key_tmp, ctx->q);
	fmpz_poly_mod(priv_key_inv_tmp, ctx->q);
	fmpz_poly_mod(encr_msg_tmp, ctx->q);

	poly_starmultiply(priv_key_tmp, encr_msg_tmp, a, ctx, ctx->q);
	fmpz_poly_mod(a, ctx->q);
	poly_starmultiply(a, priv_key_inv_tmp, out_bin, ctx, ctx->p);
	fmpz_poly_mod(out_bin, ctx->p);

	fmpz_poly_clear(a);
	fmpz_poly_clear(priv_key_tmp);
	fmpz_poly_clear(priv_key_inv_tmp);
	fmpz_poly_clear(encr_msg_tmp);

	return true;
}

/*------------------------------------------------------------------------*/

string *
ntru_decrypt_string(
		const string *encr_msg,
		const fmpz_poly_t priv_key,
		const fmpz_poly_t priv_key_inv,
		const ntru_context *ctx)
{
	uint32_t i = 0;
	string *decr_msg;
	fmpz_poly_t **poly_array;

	if (!encr_msg || !encr_msg->len)
		return NULL;

	poly_array = base64_to_poly_arr(encr_msg, ctx);

	while (*poly_array[i]) {
		if (!ntru_decrypt_poly(*poly_array[i],
					priv_key,
					priv_key_inv,
					*poly_array[i],
					ctx))
			NTRU_ABORT("failed encrypting string!\n");
		i++;
	}

	decr_msg = bin_poly_arr_to_ascii(poly_array, ctx);

	poly_delete_array(poly_array);

	return decr_msg;
}

/*------------------------------------------------------------------------*/
