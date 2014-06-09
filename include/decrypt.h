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
 * @file decrypt.h
 * This file holds the public API of decryption
 * of the pqc NTRU implementation and is
 * meant to be installed on the client system.
 * @brief public API, decryption
 */

#ifndef PUBLIC_NTRU_DECRYPT_H_
#define PUBLIC_NTRU_DECRYPT_H_


#include "ntru.h"

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Decryption of a given encrypted string.
 *
 * @param encr_msg the encrypted message in the form of a string
 * @param priv_key the polynomial containing the private key to decrypt
 * 		the message
 * @param priv_key_inv the inverse polynome to the private key
 * @param params the ntru_params
 * @return the decrypted string or NULL on failure
 */
string *
ntru_decrypt_string(
		const string *encr_msg,
		const fmpz_poly_t priv_key,
		const fmpz_poly_t priv_key_inv,
		const ntru_params *params);


#endif /* PUBLIC_NTRU_DECRYPT_H_ */
