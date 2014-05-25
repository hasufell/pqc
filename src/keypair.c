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
 * @file keypair.c
 * This file handles the creation of the
 * key pair and possibly common operations on them.
 * @brief key creation and operations
 */

#include "context.h"
#include "keypair.h"
#include "poly.h"

#include <fmpz_poly.h>
#include <fmpz.h>

#include <stdbool.h>


bool
ntru_create_keypair(
		fmpz_poly_t f,
		fmpz_poly_t g,
		keypair *pair,
		ntru_context *ctx)
{
	bool retval = true;
	fmpz_poly_t Fq,
				Fp,
				pub;

	fmpz_poly_init(Fq);
	fmpz_poly_init(Fp);
	fmpz_poly_init(pub);


	if (!poly_inverse_poly_q(f, Fq, ctx)) {
		retval = false;
		goto cleanup;
	}

	if (!poly_inverse_poly_p(f, Fp, ctx)) {
		retval = false;
		goto cleanup;
	}

	poly_starmultiply(Fq, g, pub, ctx, ctx->q);
	fmpz_poly_scalar_mul_ui(pub, pub, ctx->p);
	fmpz_poly_mod(pub, ctx->q);

	fmpz_poly_init(pair->priv);
	fmpz_poly_init(pair->priv_inv);
	fmpz_poly_init(pair->pub);

	fmpz_poly_set(pair->priv, f);
	fmpz_poly_set(pair->priv_inv, Fp);
	fmpz_poly_set(pair->pub, pub);

cleanup:
	fmpz_poly_clear(Fq);
	fmpz_poly_clear(Fp);
	fmpz_poly_clear(pub);
	return retval;
}

void
ntru_delete_keypair(keypair *pair)
{
	fmpz_poly_clear(pair->priv_inv);
	fmpz_poly_clear(pair->priv);
	fmpz_poly_clear(pair->pub);
}
