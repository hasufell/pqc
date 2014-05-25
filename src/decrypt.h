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
 * Header for the external API of decrypt.c.
 * @brief header for decrypt.c
 */

#ifndef NTRU_DECRYPT_H
#define NTRU_DECRYPT_H

#include "poly.h"
#include "context.h"

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Decryption of the given Polynom with the private key, its inverse
 * and the fitting ntru_context
 *
 * @param encr_msg encrypted polynom with maximum length of N from
 * 		the given context
 * @param priv_key the polynom containing the private key to decrypt
 * 		the message
 * @param priv_key_inv the inverse polynome to the private key
 * @param out the result polynom is written in here [out]
 * @param ctx the ntru_context
 */
void
ntru_decrypt_poly(
		fmpz_poly_t encr_msg,
		fmpz_poly_t priv_key,
		fmpz_poly_t priv_key_inv,
		fmpz_poly_t out,
		ntru_context *ctx);


#endif /* NTRU_DECRYPT */
