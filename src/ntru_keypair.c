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
 * @file ntru_keypair.c
 * This file handles the creation of the
 * key pair and possibly common operations on them.
 * @brief key creation and operations
 */

#include "ntru_ascii_poly.h"
#include "ntru_file.h"
#include "ntru_keypair.h"
#include "ntru_params.h"
#include "ntru_poly.h"
#include "ntru_poly_ascii.h"
#include "ntru_string.h"

#include <fmpz_poly.h>
#include <fmpz.h>

#include <stdbool.h>
#include <string.h>


/*------------------------------------------------------------------------*/

bool
ntru_create_keypair(
		fmpz_poly_t f,
		fmpz_poly_t g,
		keypair *pair,
		ntru_params *params)
{
	bool retval = false;
	fmpz_poly_t Fq,
				Fp,
				pub;

	if (!f || !g || !params)
		goto _return;

	fmpz_poly_init(Fq);
	fmpz_poly_init(Fp);
	fmpz_poly_init(pub);

	if (!poly_inverse_poly_q(f, Fq, params))
		goto _cleanup;

	if (!poly_inverse_poly_p(f, Fp, params))
		goto _cleanup;

	poly_starmultiply(Fq, g, pub, params, params->q);
	fmpz_poly_scalar_mul_ui(pub, pub, params->p);
	fmpz_poly_mod_unsigned(pub, params->q);

	fmpz_poly_init(pair->priv);
	fmpz_poly_init(pair->priv_inv);
	fmpz_poly_init(pair->pub);

	fmpz_poly_set(pair->priv, f);
	fmpz_poly_set(pair->priv_inv, Fp);
	fmpz_poly_set(pair->pub, pub);

	retval = true;

_cleanup:
	fmpz_poly_clear(Fq);
	fmpz_poly_clear(Fp);
	fmpz_poly_clear(pub);
_return:
	return retval;
}

/*------------------------------------------------------------------------*/

void
export_public_key(char const * const filename,
		fmpz_poly_t pub,
		ntru_params *params)
{
	string *pub_string;

	pub_string = poly_to_base64(pub, params);
	write_file(pub_string, filename);

	string_delete(pub_string);
}

/*------------------------------------------------------------------------*/

void
export_priv_key(char const * const filename,
		fmpz_poly_t priv,
		ntru_params *params)
{
	string *priv_string;
	fmpz_poly_t priv_u;

	fmpz_poly_init(priv_u);
	fmpz_poly_set(priv_u, priv);
	fmpz_poly_mod_unsigned(priv_u, params->p);

	priv_string = poly_to_base64(priv_u, params);
	write_file(priv_string, filename);

	fmpz_poly_clear(priv_u);
	string_delete(priv_string);
}

/*------------------------------------------------------------------------*/

void
import_public_key(char const * const filename,
		fmpz_poly_t pub,
		ntru_params *params)
{
	string *pub_string;
	fmpz_poly_t **imported;

	pub_string = read_file(filename);
	imported = base64_to_poly_arr(pub_string, params);

	/* if the array exceeds one element, then something
	 * went horribly wrong */
	if (*imported[1])
		NTRU_ABORT_DEBUG("Failed importing public key!");

	fmpz_poly_set(pub, **imported);

	string_delete(pub_string);
	poly_delete_array(imported);
	free(imported);
}

/*------------------------------------------------------------------------*/

void
import_priv_key(char const * const filename,
		fmpz_poly_t priv,
		fmpz_poly_t priv_inv,
		ntru_params *params)
{
	string *pub_string;
	fmpz_poly_t **imported,
				Fp;

	fmpz_poly_init(Fp);

	pub_string = read_file(filename);

	imported = base64_to_poly_arr(pub_string, params);
	fmpz_poly_mod(**imported, params->p);

	/* if the array exceeds one element, then something
	 * went horribly wrong */
	if (*imported[1])
		NTRU_ABORT_DEBUG("Failed importing private key!");

	fmpz_poly_set(priv, **imported);

	if (!poly_inverse_poly_p(priv, Fp, params))
		goto cleanup;

	fmpz_poly_mod(Fp, params->p);

	fmpz_poly_set(priv_inv, Fp);
	fmpz_poly_clear(Fp);

cleanup:
	string_delete(pub_string);
	poly_delete_array(imported);
	free(imported);
}

/*------------------------------------------------------------------------*/

void
ntru_delete_keypair(keypair *pair)
{
	fmpz_poly_clear(pair->priv_inv);
	fmpz_poly_clear(pair->priv);
	fmpz_poly_clear(pair->pub);
}

/*------------------------------------------------------------------------*/
