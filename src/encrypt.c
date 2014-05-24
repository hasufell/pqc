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

#include "encrypt.h"

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * encrypt the msg, using the math:
 * e = (h ∗ r) + m (mod q)
 *
 * e = the encrypted poly
 * h = the public key
 * r = the random poly
 * m = the message poly
 * q = large mod
 *
 * @param msg pb_poly* 		the message to encrypt
 * @param pub_key pb_poly* 	the public key
 * @param rnd pb_poly*   	the random poly
 * @param out pb_poly* 		the output poly [out]
 * @param ctx ntru_context* the ntru context
 */
void ntru_encrypt_poly(
		fmpz_poly_t msg,
		fmpz_poly_t pub_key,
		fmpz_poly_t rnd,
		fmpz_poly_t out,
		ntru_context *ctx)
{
	fmpz_poly_zero(out);
	poly_starmultiply(pub_key, rnd, out, ctx, ctx->q);
	fmpz_poly_add(out, out, msg);
	fmpz_poly_mod_unsigned(out, ctx->q);
}
