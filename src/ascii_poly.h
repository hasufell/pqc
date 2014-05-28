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
 * @file ascii_poly.h
 * Header for the internal API of ascii_poly.c.
 * @brief header for ascii_poly.c
 */

#ifndef NTRU_ASCII_POLY_H_
#define NTRU_ASCII_POLY_H_


#include "common.h"
#include "context.h"
#include "ntru_string.h"

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Convert a "binary" ascii string to a binary polyomial.
 * The ascii string will be converted to a binary representation
 * and the following mapping will apply between binary -> poly:
 *
 * 1 => 1
 *
 * 0 => -1
 *
 * If the polynomial is of degree less than N -1, then it will
 * be filled with trailing 2's for later use in bin_poly_to_ascii().
 *
 * @param to_poly the string to get into binary polynomial format
 * @param ctx the NTRUEncrypt context
 * @return newly allocated array of binary polynomials
 */
fmpz_poly_t *
ascii_bin_to_bin_poly(char *to_poly, ntru_context *ctx);

/**
 * Convert an ascii string to an array of binary polyomials.
 * The ascii string will be converted to a binary representation
 * and the following mapping will apply between binary -> poly:
 *
 * 1 => 1
 *
 * 0 => -1
 *
 * If the last polynomial is of degree less than N -1, then it will
 * be filled with trailing 2's for later use in bin_poly_arr_to_ascii().
 *
 * @param to_poly the string to get into binary polynomial format
 * @param ctx the NTRUEncrypt context
 * @return newly allocated array of binary polynomials
 */
fmpz_poly_t **
ascii_to_bin_poly_arr(char *to_poly, ntru_context *ctx);

/**
 * Convert a single binary polynomial back to a real string.
 * The polynomial coefficients represent a binary format of the
 * ascii string with the following mapping:
 *
 * 1 => 1
 *
 * -1 => 0
 *
 *  2 => 0
 *
 * The 2's are only used for filling up the rest of the polynomial,
 * so they will just end up as '\0's at the end of the string and will
 * not confuse the result.
 *
 * @param poly the binary polynomial to convert
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
bin_poly_to_ascii(fmpz_poly_t poly,
		ntru_context *ctx);

/**
 * Convert an array of binary polynomials back to a real string.
 * The polynomial coefficients represent a binary format of the
 * ascii string with the following mapping:
 *
 * 1 => 1
 *
 * -1 => 0
 *
 *  2 => 0
 *
 * The 2's are only used for filling up the last polynomial, so they will
 * just end up as '\0's at the end of the string and will not confuse
 * the result.
 *
 * @param bin_poly_arr the array of polynomials
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
bin_poly_arr_to_ascii(fmpz_poly_t **bin_poly_arr, ntru_context *ctx);

/**
 * Convert a single polynom back to a real string which is
 * ascii encoded (full 256 C char spectrum).
 * The polynomial coefficients are expected to be in the range
 * [0, q-1] and will be casted back to chars without any mapping.
 *
 * Trailing q coefficients are only used for filling up the rest of
 * a polynomial with '\0', so they will not confuse the result.
 *
 * @param poly the polynomial to convert
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
poly_to_ascii(fmpz_poly_t poly,
		ntru_context *ctx);

/**
 * Convert an base64 encoded string to an array of polyomials with
 * coefficients which are expected to be in the range [0, q-1].
 * The chars will be converted (after decoding) to their integer
 * representation and directly put into the coefficients.
 *
 * If the last polynomial is of degree less than N -1, then it will
 * be filled with trailing q's for later user in poly_arr_to_base64().
 *
 * @param to_poly the string to get into polynomial format,
 * which is of type string, so we can iterate safely over it
 * (the string might have null-bytes in the middle of it)
 * @param ctx the NTRUEncrypt context
 * @return newly allocated array of polynomials
 */
fmpz_poly_t **
base64_to_poly_arr(string *to_poly, ntru_context *ctx);

/**
 * Convert a single polynom back to a real string which is
 * base64 encoded.
 * The polynomial coefficients are expected to be in the range
 * [0, q-1] and will be casted back to chars without any mapping.
 *
 * Trailing q coefficients are only used for filling up the rest of
 * a polynomial with '\0', so they will not confuse the result.
 *
 * @param poly the polynomial to convert
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
poly_to_base64(fmpz_poly_t poly,
		ntru_context *ctx);

/**
 * Convert an array of polynomials back to a real string which
 * is base64 encoded.
 * The polynomial coefficients are expected to be in the range
 * [0, q-1] and will be casted back to chars without any mapping.
 *
 * Trailing q coefficients are only used for filling up the rest of
 * a polynomial with '\0', so they will not confuse the result.
 *
 * @param poly_arr the array of polynomials
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
poly_arr_to_base64(fmpz_poly_t **poly_arr, ntru_context *ctx);


#endif /* NTRU_ASCII_POLY_H_ */
