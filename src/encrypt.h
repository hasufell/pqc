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
 * @file encrypt.h
 * Header for the internal API of encrypt.c.
 * @brief header for encrypt.c
 */

#ifndef PQC_ENCRYPT_H
#define PQC_ENCRYPT_H


#include "common.h"
#include "context.h"
#include "poly.h"

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * encrypt the msg, using the math:
 * e = (h ∗ r) + m (mod q)
 *
 * e = the encrypted poly
 *
 * h = the public key
 *
 * r = the random poly
 *
 * m = the message poly
 *
 * q = large mod
 *
 * @param msg_tern the message to encrypt, in ternary format
 * @param pub_key the public key
 * @param rnd the random poly (should have relatively small
 * coefficients, but not restricted to {-1, 0, 1})
 * @param out the output poly which is in the range {0, q-1}
 * (not ternary!) [out]
 * @param ctx ntru_context the ntru context
 */
void
ntru_encrypt_poly(
		fmpz_poly_t msg_tern,
		fmpz_poly_t pub_key,
		fmpz_poly_t rnd,
		fmpz_poly_t out,
		ntru_context *ctx);

/**
 * Encrypt a message int he form of a null-terminated char array and
 * return a string.
 *
 * @param msg the message
 * @param pub_key the public key
 * @param rnd the random poly (should have relatively small
 * coefficients, but not restricted to {-1, 0, 1})
 * @param ctx ntru_context the ntru context
 * @return the newly allocated encrypted string
 */
string *
ntru_encrypt_string(
		char *msg,
		fmpz_poly_t pub_key,
		fmpz_poly_t rnd,
		ntru_context *ctx);


#endif /* PQC_ENCRYPT_H */
