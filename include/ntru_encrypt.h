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
 * @file ntru_encrypt.h
 * This file holds the public API of encryption
 * of the pqc NTRU implementation and is
 * meant to be installed on the client system.
 * @brief public API, encryption
 */

#ifndef PUBLIC_NTRU_ENCRYPT_H_
#define PUBLIC_NTRU_ENCRYPT_H_


#include <ntru.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Encrypt a message in the form of a null-terminated char array and
 * return a string.
 *
 * @param msg the message
 * @param pub_key the public key
 * @param rnd the random poly (should have relatively small
 * coefficients, but not restricted to {-1, 0, 1})
 * @param params ntru_params the ntru context
 * @return the newly allocated encrypted string, NULL on failure
 */
string *
ntru_encrypt_string(
		const string *msg,
		const fmpz_poly_t pub_key,
		const fmpz_poly_t rnd,
		const ntru_params *params);


#endif /* PUBLIC_NTRU_ENCRYPT_H_ */
