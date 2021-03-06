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
 * @file ntru_keypair.h
 * This file holds the public API of generating,
 * exporting and importing public and private keys
 * of the pqc NTRU implementation and is
 * meant to be installed on the client system.
 * @brief public API, key handling
 */

#ifndef PUBLIC_NTRU_KEYPAIR_H_
#define PUBLIC_NTRU_KEYPAIR_H_

#include <ntru.h>

#include <fmpz_poly.h>
#include <fmpz.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct keypair keypair;


/**
 * This struct holds the keypair,
 * both private and public components.
 */
struct keypair {
	/**
	 * First part of the private key,
	 * a random polynom.
	 */
	fmpz_poly_t priv;
	/**
	 * Second part of the private key,
	 * the priv polynom inverted.
	 */
	fmpz_poly_t priv_inv;
	/**
	 * The public key, computed as:
	 * h = p * (Fq * g) mod q
	 */
	fmpz_poly_t pub;
};


/**
 * Creates an NTRU key pair,
 * consisting of public and private
 * components.
 *
 * @param pair store private and public components here (the
 * polynomials inside the struct will be automatically
 * initialized) [out]
 * @param f a random ternary polynomial
 * @param g a random ternary polynomial
 * @param params the NTRU context
 * @return true for success, false if f or g are not invertible
 * (then the caller has to try different ones)
 */
bool
ntru_create_keypair(
		keypair *pair,
		const fmpz_poly_t f,
		const fmpz_poly_t g,
		const ntru_params *params);

/**
 * Export the public key to a file.
 *
 * @param filename the file to save the public key into
 * @param pub the public key
 * @param params the NTRU context
 * @return true for success, false if any of the file operations failed
 */
bool
export_public_key(char const * const filename,
		const fmpz_poly_t pub,
		const ntru_params *params);

/**
 * Export the private key to a file.
 *
 * @param filename the file to save the private key into
 * @param priv the private key
 * @param params the NTRU context
 * @return true for success, false if any of the file operations failed
 */
bool
export_priv_key(char const * const filename,
		const fmpz_poly_t priv,
		const ntru_params *params);

/**
 * Import the public key from a file.
 *
 * @param pub where to save the public key, must be initialized [out]
 * @param filename the file to get the public key from
 * @param params the NTRU context
 * @return true for success, false if any of the file operations failed
 */
bool
import_public_key(fmpz_poly_t pub,
		char const * const filename,
		const ntru_params *params);


/**
 * Import the private key from a file and store him
 * along with his inverse.
 *
 * @param priv where to save the private key, must be initialized [out]
 * @param priv_inv where to save the inverse of the private key,
 * must be initialized [out]
 * @param filename the file to get the private key from
 * @param params the NTRU context
 * @return true for success, false if any of the file operations failed
 */
bool
import_priv_key(fmpz_poly_t priv,
		fmpz_poly_t priv_inv,
		char const * const filename,
		const ntru_params *params);

/**
 * Used to free the inner structure
 * of a keypair. This will not call free()
 * on the pair itself.
 *
 * @param pair the pair to free the inner structure of
 */
void
ntru_delete_keypair(keypair *pair);


#endif /* PUBLIC_NTRU_KEYPAIR_H_ */
