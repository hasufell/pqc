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
 * @file poly.h
 * Header for the internal API of poly.c.
 * @brief header for poly.c
 */

#ifndef NTRU_POLY_H
#define NTRU_POLY_H

#include "context.h"
#include "err.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <fmpz_poly.h>


/**
 * Initializes and builds a polynomial with the
 * coefficient values of c[] of size len within NTRU
 * context ctx and returns a newly allocated polynomial.
 * For an empty polynom, both parameters can be NULL/0.
 *
 * @param new_poly the polynomial to initialize and
 * fill with coefficients
 * @param c array of polynomial coefficients, can be NULL
 * @param len size of the coefficient array, can be 0
 * @return newly allocated polynomial pointer, must be freed
 * with fmpz_poly_clear()
 */
void
poly_new(fmpz_poly_t new_poly,
		int const * const c,
		const size_t len);

/**
 * This deletes the internal structure of a polynomial,
 * and frees the pointer.
 *
 * @param poly the polynomial to delete
 */
void
poly_delete(fmpz_poly_t poly);

/**
 * Delete the internal structure of a polynomial
 * array which must be NULL terminated. It is expected
 * that poly_array is not on the stack and was obtained
 * by a function like ascii_to_poly().
 *
 * @param poly_array the polynomial array
 */
void
poly_delete_array(fmpz_poly_t **poly_array);

/**
 * This deletes the internal structure of all polynomials,
 * and frees the pointers.
 * You must call this with NULL as last argument!
 *
 * @param poly the polynomial to delete
 * @param ... follow up polynomials
 */
void
poly_delete_all(fmpz_poly_t poly, ...);

/**
 * Calls fmpz_poly_get_nmod_poly() and
 * fmpz_poly_set_nmod_poly_unsigned() in a row,
 * so we don't have to deal with the intermediate
 * nmod_poly_t type if we don't need it.
 *
 * This also normalises the coefficients to the interval
 * 0 <= r < m.
 *
 * @param a the polynom to apply the modulus to
 * @param mod the modulus
 */
void
fmpz_poly_mod_unsigned(fmpz_poly_t a,
		uint32_t mod);

/**
 * Calls fmpz_poly_get_nmod_poly() and
 * fmpz_poly_set_nmod_poly() in a row,
 * so we don't have to deal with the intermediate
 * nmod_poly_t type if we don't need it.
 *
 * This also normalises the coefficients to the interval
 * -m/2 <= r < m/2.
 *
 * @param a the polynom to apply the modulus to
 * @param mod the modulus
 */
void
fmpz_poly_mod(fmpz_poly_t a,
		uint32_t mod);

/**
 * The same as fmpz_poly_set_coeff_fmpz() except that it
 * will take care of null-pointer coefficients and use
 * fmpz_poly_set_coeff_si() in that case.
 *
 * @param poly the polynom we want to change a coefficient of
 * @param n the coefficient we want to set
 * @param x the value to assign to the coefficient
 */
void
fmpz_poly_set_coeff_fmpz_n(fmpz_poly_t poly,
		slong n,
		const fmpz_t x);

/**
 * Wrapper around fmpz_invmod() where we convert
 * mod to an fmpz_t implicitly.
 *
 * @param f result [out]
 * @param g the inverse
 * @param mod the modulo
 */
int
fmpz_invmod_ui(fmpz_t f,
		const fmpz_t g,
		uint32_t mod);

/**
 * The same as fmpz_add() except that it handles NULL
 * pointer for g and h.
 */
void
fmpz_add_n(fmpz_t f, const fmpz_t g, const fmpz_t h);

/**
 * Starmultiplication, as follows:
 * c = a * b mod (x^N − 1)
 *
 * @param a polynom to multiply (can be the same as c)
 * @param b polynom to multiply
 * @param c polynom [out]
 * @param ctx NTRU context
 * @param modulus whether we use p or q
 */
void
poly_starmultiply(fmpz_poly_t a,
		fmpz_poly_t b,
		fmpz_poly_t c,
		ntru_context *ctx,
		uint32_t modulus);

/**
 * Compute the inverse of a polynomial in modulo a power of 2,
 * which is q. This is based off the pseudo-code for "Inversion
 * in (Z/2Z)[X](X^N - 1)" and "Inversion in (Z/p^r Z)[X](X^N - 1)".
 * See NTRU Cryptosystems Tech Report #014 "Almost Inverses
 * and Fast NTRU Key Creation."
 *
 * @param a polynomial to invert (is allowed to be the same as param Fq)
 * @param Fq polynomial [out]
 * @param ctx NTRU context
 * @return true if invertible, false if not
 */
bool
poly_inverse_poly_q(fmpz_poly_t a,
		fmpz_poly_t Fq,
		ntru_context *ctx);

/**
 * Compute the inverse of a polynomial in (Z/pZ)[X]/(X^N - 1).
 * See NTRU Cryptosystems Tech Report #014 "Almost Inverses
 * and Fast NTRU Key Creation."
 *
 * @param a polynomial to invert
 * @param Fp polynomial [out]
 * @param ctx NTRU context
 */
bool
poly_inverse_poly_p(fmpz_poly_t a,
		fmpz_poly_t Fp,
		ntru_context *ctx);

/**
 * Draws a polynomial to stdout.
 *
 * @param poly draw this
 */
void
poly_draw(fmpz_poly_t poly);

/**
 * Draws a polynomial to stdout,
 * in pretty format.
 *
 * @param poly draw this
 */
void
poly_draw_pretty(fmpz_poly_t poly);


#endif /* NTRU_POLY_H */
