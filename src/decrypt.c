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

#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/*------------------------------------------------------------------------*/

void
ntru_decrypt_poly(
		fmpz_poly_t encr_msg,
		fmpz_poly_t priv_key,
		fmpz_poly_t priv_key_inv,
		fmpz_poly_t out_bin,
		ntru_context *ctx)
{
	fmpz_poly_t a;

	fmpz_poly_init(a);
	fmpz_poly_zero(a);

	poly_starmultiply(priv_key, encr_msg, a, ctx, ctx->q);
	fmpz_poly_mod(a, ctx->q);
	poly_starmultiply(a, priv_key_inv, out_bin, ctx, ctx->p);
	fmpz_poly_mod(out_bin, ctx->p);

	fmpz_poly_clear(a);
}

/*------------------------------------------------------------------------*/

string *
ntru_decrypt_string(
		string *encr_msg,
		fmpz_poly_t priv_key,
		fmpz_poly_t priv_key_inv,
		ntru_context *ctx)
{
	uint32_t i = 0;
	string *decr_msg;
	fmpz_poly_t **poly_array;

	poly_array = base64_to_poly_arr(encr_msg, ctx);

	while (*poly_array[i]) {
		ntru_decrypt_poly(*poly_array[i], priv_key, priv_key_inv,
				*poly_array[i], ctx);
		i++;
	}

	decr_msg = bin_poly_arr_to_ascii(poly_array, ctx);

	poly_delete_array(poly_array);

	return decr_msg;
}

/*------------------------------------------------------------------------*/
