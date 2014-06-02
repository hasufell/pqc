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
 * @file encrypt.c
 * This file handles the NTRU encryption
 * algorithm.
 * @brief NTRU encryption
 */

#include "ascii_poly.h"
#include "encrypt.h"
#include "mem.h"
#include "ntru_string.h"
#include "poly_ascii.h"

#include <string.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/*------------------------------------------------------------------------*/

bool
ntru_encrypt_poly(
		const fmpz_poly_t msg_bin,
		const fmpz_poly_t pub_key,
		const fmpz_poly_t rnd,
		fmpz_poly_t out,
		const ntru_context *ctx)
{
	fmpz_poly_t tmp_poly_msg;

	if (!msg_bin || !pub_key || !rnd || !out || !ctx)
		return false;

	/* allow aliasing */
	fmpz_poly_init(tmp_poly_msg);
	fmpz_poly_set(tmp_poly_msg, msg_bin);

	fmpz_poly_zero(out);
	poly_starmultiply(pub_key, rnd, out, ctx, ctx->q);

	/* FIXME: causes rare junk chars in some cases,
	 * which is perfectly reproducible, so this is not UB */
	fmpz_poly_add(out, out, tmp_poly_msg);
	fmpz_poly_mod_unsigned(out, ctx->q);

	fmpz_poly_clear(tmp_poly_msg);

	return true;
}

/*------------------------------------------------------------------------*/

string *
ntru_encrypt_string(
		const string *msg,
		const fmpz_poly_t pub_key,
		const fmpz_poly_t rnd,
		const ntru_context *ctx)
{
	uint32_t i = 0;
	string *enc_msg;
	fmpz_poly_t **poly_array;

	if (!msg || !msg->len)
		return NULL;

	poly_array = ascii_to_bin_poly_arr(msg, ctx);

	while (*poly_array[i]) {
		if (!ntru_encrypt_poly(*poly_array[i],
				pub_key,
				rnd,
				*poly_array[i],
				ctx))
			NTRU_ABORT("failed encrypting string!\n");
		i++;
	}

	enc_msg = poly_arr_to_base64(poly_array, ctx);

	poly_delete_array(poly_array);

	return enc_msg;
}

/*------------------------------------------------------------------------*/
