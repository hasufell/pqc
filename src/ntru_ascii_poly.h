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
 * @file ntru_ascii_poly.h
 * Header for the internal API of ntru_ascii_poly.c.
 * @brief header for ntru_ascii_poly.c
 */

#ifndef NTRU_ASCII_POLY_H_
#define NTRU_ASCII_POLY_H_


#include "ntru_common.h"
#include "ntru_string.h"
#include "ntru_params.h"

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
 * @param to_poly the string to get into binary polynomial format
 * @param params the NTRUEncrypt context
 * @return newly allocated array of binary polynomials
 */
fmpz_poly_t *
ascii_bin_to_bin_poly(const char *to_poly, const ntru_params *params);

/**
 * Convert an ascii string to an array of binary polyomials.
 * The ascii string will be converted to a binary representation
 * and the following mapping will apply between binary -> poly:
 *
 * 1 => 1
 *
 * 0 => -1
 *
 * @param to_poly the string to get into binary polynomial format
 * @param params the NTRUEncrypt context
 * @return newly allocated array of binary polynomials
 */
fmpz_poly_t **
ascii_to_bin_poly_arr(const string *to_poly, const ntru_params *params);

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
 * @param params the NTRUEncrypt context
 * @return newly allocated array of polynomials
 */
fmpz_poly_t **
base64_to_poly_arr(const string *to_poly, const ntru_params *params);


#endif /* NTRU_ASCII_POLY_H_ */
