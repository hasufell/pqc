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
 * @file poly_ascii.h
 * Header for the internal API of poly_ascii.c.
 * @brief header for poly_ascii.c
 */

#ifndef NTRU_POLY_ASCII_H_
#define NTRU_POLY_ASCII_H_


#include "common.h"
#include "context.h"
#include "ntru_string.h"

#include <fmpz_poly.h>
#include <fmpz.h>


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
bin_poly_to_ascii(const fmpz_poly_t poly,
		const ntru_context *ctx);

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
bin_poly_arr_to_ascii(fmpz_poly_t **bin_poly_arr,
		const ntru_context *ctx);

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
poly_to_ascii(const fmpz_poly_t poly,
		const ntru_context *ctx);

/**
 * Convert an array of polynomials back to a real string.
 * The polynomial coefficients are expected to be in the range
 * [0, q-1] and will be casted back to chars without any mapping.
 *
 * Trailing q coefficients are only used for filling up the rest of
 * a polynomial with '\0', so they will not confuse the result.
 *
 * @param poly_array the array of polynomials
 * @param ctx the NTRUEncrypt context
 * @return the real string, newly allocated
 */
string *
poly_arr_to_ascii(fmpz_poly_t **poly_array,
		const ntru_context *ctx);

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
poly_to_base64(const fmpz_poly_t poly,
		const ntru_context *ctx);

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
poly_arr_to_base64(fmpz_poly_t **poly_arr, const ntru_context *ctx);


#endif /* NTRU_POLY_ASCII_H_ */
