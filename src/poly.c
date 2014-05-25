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
 * @file poly.c
 * This files purpose is to handle polynomials
 * in general, allowing modification, arithmetic
 * and common algorithms like inverting them.
 * @brief operations on polynomials
 */

#include "context.h"
#include "err.h"
#include "mem.h"
#include "poly.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/*
 * static declarations
 */
static void poly_mod2_to_modq(fmpz_poly_t a,
		fmpz_poly_t Fq,
		ntru_context *ctx);


/**
 * Find the inverse polynomial modulo a power of 2,
 * which is q.
 *
 * @param a polynomial to invert
 * @param Fq polynomial [out]
 * @param ctx NTRU context
 */
static void poly_mod2_to_modq(fmpz_poly_t a,
		fmpz_poly_t Fq,
		ntru_context *ctx)
{
	int v = 2;
	fmpz_poly_t poly_tmp, two;

	fmpz_poly_init(poly_tmp);
	fmpz_poly_zero(poly_tmp);
	fmpz_poly_init(two);
	fmpz_poly_set_coeff_ui(two, 0, 2);

	while (v < (int)(ctx->q)) {
		v = v * 2;

		poly_starmultiply(a, Fq, poly_tmp, ctx, v);
		fmpz_poly_sub(poly_tmp, two, poly_tmp);
		fmpz_poly_mod_unsigned(poly_tmp, v);
		poly_starmultiply(Fq, poly_tmp, Fq, ctx, v);

	}

	fmpz_poly_clear(poly_tmp);
	fmpz_poly_clear(two);

}

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
void poly_new(fmpz_poly_t new_poly,
		int const * const c,
		const size_t len)
{
	fmpz_poly_init(new_poly);

	for (unsigned int i = 0; i < len; i++)
		fmpz_poly_set_coeff_si(new_poly, i, c[i]);
}

/**
 * This deletes the internal structure of a polynomial,
 * and frees the pointer.
 *
 * @param poly the polynomial to delete
 */
void poly_delete(fmpz_poly_t poly)
{
	fmpz_poly_clear(poly);
}

/**
 * Delete the internal structure of a polynomial
 * array which must be NULL terminated. It is expected
 * that poly_array is not on the stack and was obtained
 * by a function like ascii_to_poly().
 *
 * @param poly_array the polynomial array
 */
void poly_delete_array(fmpz_poly_t **poly_array)
{
	unsigned int i = 0;

	while(poly_array[i]) {
		poly_delete(*(poly_array[i]));
		free(*(poly_array[i]));
		i++;
	}
	free(poly_array);
}

/**
 * This deletes the internal structure of all polynomials,
 * and frees the pointers.
 * You must call this with NULL as last argument!
 *
 * @param poly the polynomial to delete
 * @param ... follow up polynomials
 */
void poly_delete_all(fmpz_poly_t poly, ...)
{
	fmpz_poly_struct *next_poly;
	va_list args;

	next_poly = poly;
	va_start(args, poly);
	while (next_poly != NULL) {
		poly_delete(next_poly);
		next_poly = va_arg(args, fmpz_poly_struct*);
	}
	va_end(args);
}

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
void fmpz_poly_mod_unsigned(fmpz_poly_t a,
		unsigned int mod)
{
	nmod_poly_t nmod_tmp;

	nmod_poly_init(nmod_tmp, mod);

	fmpz_poly_get_nmod_poly(nmod_tmp, a);
	fmpz_poly_set_nmod_poly_unsigned(a, nmod_tmp);

	nmod_poly_clear(nmod_tmp);
}

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
void fmpz_poly_mod(fmpz_poly_t a,
		unsigned int mod)
{
	nmod_poly_t nmod_tmp;

	nmod_poly_init(nmod_tmp, mod);

	fmpz_poly_get_nmod_poly(nmod_tmp, a);
	fmpz_poly_set_nmod_poly(a, nmod_tmp);

	nmod_poly_clear(nmod_tmp);
}

/**
 * The same as fmpz_poly_set_coeff_fmpz() except that it
 * will take care of null-pointer coefficients and use
 * fmpz_poly_set_coeff_si() in that case.
 *
 * @param poly the polynom we want to change a coefficient of
 * @param n the coefficient we want to set
 * @param x the value to assign to the coefficient
 */
void fmpz_poly_set_coeff_fmpz_n(fmpz_poly_t poly, slong n,
		const fmpz_t x)
{
	if (x)
		fmpz_poly_set_coeff_fmpz(poly, n, x);
	else
		fmpz_poly_set_coeff_si(poly, n, 0);
}

/**
 * Wrapper around fmpz_invmod() where we convert
 * mod to an fmpz_t implicitly.
 *
 * @param f result [out]
 * @param g the inverse
 * @param mod the modulo
 */
int fmpz_invmod_ui(fmpz_t f, const fmpz_t g, unsigned int mod)
{
	fmpz_t modulus;

	fmpz_init_set_ui(modulus, mod);

	return fmpz_invmod(f, g, modulus);
}

/**
 * The same as fmpz_add() except that it handles NULL
 * pointer for g and h.
 */
void fmpz_add_n(fmpz_t f, const fmpz_t g, const fmpz_t h)
{
	if (!g && !h) {
		fmpz_zero(f);
	} else {
		if (!g && h)
			fmpz_add_ui(f, h, 0);
		else if (g && !h)
			fmpz_add_ui(f, g, 0);
		else
			fmpz_add(f, g, h);
	}
}

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
void poly_starmultiply(fmpz_poly_t a,
		fmpz_poly_t b,
		fmpz_poly_t c,
		ntru_context *ctx,
		unsigned int modulus)
{
	fmpz_poly_t a_tmp;
	fmpz_t c_coeff_k;

	fmpz_poly_init(a_tmp);
	fmpz_init(c_coeff_k);

	/* avoid side effects */
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(c);

	for (int k = ctx->N - 1; k >= 0; k--) {
		int j;

		j = k + 1;

		fmpz_set_si(c_coeff_k, 0);

		for (int i = ctx->N - 1; i >= 0; i--) {
			fmpz *a_tmp_coeff_i,
				 *b_coeff_j;

			if (j == (int)(ctx->N))
				j = 0;

			a_tmp_coeff_i = fmpz_poly_get_coeff_ptr(a_tmp, i);
			b_coeff_j = fmpz_poly_get_coeff_ptr(b, j);

			if (a_tmp_coeff_i && fmpz_cmp_si(a_tmp_coeff_i, 0) &&
					b_coeff_j && fmpz_cmp_si(b_coeff_j, 0)) {
				fmpz_t fmpz_tmp;

				fmpz_init(fmpz_tmp);

				fmpz_mul(fmpz_tmp, a_tmp_coeff_i, b_coeff_j);
				fmpz_add(fmpz_tmp, fmpz_tmp, c_coeff_k);
				fmpz_mod_ui(c_coeff_k, fmpz_tmp, modulus);

				fmpz_poly_set_coeff_fmpz(c, k, c_coeff_k);

				fmpz_clear(fmpz_tmp);
			}
			j++;
		}
		fmpz_clear(c_coeff_k);
	}

	fmpz_poly_clear(a_tmp);
}

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
bool poly_inverse_poly_q(fmpz_poly_t a,
		fmpz_poly_t Fq,
		ntru_context *ctx)
{
	bool retval = true;
	int k = 0,
		j = 0;
	fmpz *b_last;
	fmpz_poly_t a_tmp,
				b,
				c,
				f,
				g;

	/* general initialization of temp variables */
	fmpz_poly_init(b);
	fmpz_poly_set_coeff_ui(b, 0, 1);
	fmpz_poly_init(c);
	fmpz_poly_init(f);
	fmpz_poly_set(f, a);

	/* set g(x) = x^N − 1 */
	fmpz_poly_init(g);
	fmpz_poly_set_coeff_si(g, 0, -1);
	fmpz_poly_set_coeff_si(g, ctx->N, 1);

	/* avoid side effects */
	fmpz_poly_init(a_tmp);
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(Fq);

	while (1) {
		while (fmpz_is_zero(fmpz_poly_get_coeff_ptr(f, 0))) {
			for (unsigned int i = 1; i <= ctx->N; i++) {
				fmpz *f_coeff = fmpz_poly_get_coeff_ptr(f, i);
				fmpz *c_coeff = fmpz_poly_get_coeff_ptr(c, ctx->N - i);

				/* f(x) = f(x) / x */
				fmpz_poly_set_coeff_fmpz_n(f, i - 1,
						f_coeff);

				/* c(x) = c(x) * x */
				fmpz_poly_set_coeff_fmpz_n(c, ctx->N + 1 - i,
						c_coeff);
			}

			fmpz_poly_set_coeff_si(f, ctx->N, 0);
			fmpz_poly_set_coeff_si(c, 0, 0);

			k++;

			if (fmpz_poly_degree(f) == -1) {
				retval = false;
				goto cleanup;
			}
		}

		if (fmpz_poly_degree(f) == 0)
			break;

		if (fmpz_poly_degree(f) < fmpz_poly_degree(g)) {
			fmpz_poly_swap(f, g);
			fmpz_poly_swap(b, c);
		}

		fmpz_poly_add(f, g, f);
		fmpz_poly_mod_unsigned(f, 2);

		fmpz_poly_add(b, c, b);
		fmpz_poly_mod_unsigned(b, 2);
	}

	k = k % ctx->N;

	b_last = fmpz_poly_get_coeff_ptr(b, ctx->N);
	if (b_last && fmpz_cmp_si(b_last, 0)) {
		retval = false;
		goto cleanup;
	}

	/* Fq(x) = x^(N-k) * b(x) */
	for (int i = ctx->N - 1; i >= 0; i--) {
		fmpz *b_i;

		j = i - k;

		if (j < 0)
			j = j + ctx->N;

		b_i = fmpz_poly_get_coeff_ptr(b, i);
		fmpz_poly_set_coeff_fmpz_n(Fq, j, b_i);
	}

	poly_mod2_to_modq(a_tmp, Fq, ctx);

	/* check if the f * Fq = 1 (mod p) condition holds true */
	fmpz_poly_set(a_tmp, a);
	poly_starmultiply(a_tmp, Fq, a_tmp, ctx, ctx->q);
	if (!fmpz_poly_is_one(a_tmp))
		retval = false;

cleanup:
	fmpz_poly_clear(a_tmp);
	fmpz_poly_clear(b);
	fmpz_poly_clear(c);
	fmpz_poly_clear(f);
	fmpz_poly_clear(g);

	if (!retval)
		fmpz_poly_zero(Fq);

	return retval;
}

/**
 * Compute the inverse of a polynomial in (Z/pZ)[X]/(X^N - 1).
 * See NTRU Cryptosystems Tech Report #014 "Almost Inverses
 * and Fast NTRU Key Creation."
 *
 * @param a polynomial to invert
 * @param Fp polynomial [out]
 * @param ctx NTRU context
 */
bool poly_inverse_poly_p(fmpz_poly_t a,
		fmpz_poly_t Fp,
		ntru_context *ctx)
{
	bool retval = true;
	int k = 0,
		j = 0;
	fmpz *b_last;
	fmpz_poly_t a_tmp,
				b,
				c,
				f,
				g;

	/* general initialization of temp variables */
	fmpz_poly_init(b);
	fmpz_poly_set_coeff_ui(b, 0, 1);
	fmpz_poly_init(c);
	fmpz_poly_init(f);
	fmpz_poly_set(f, a);

	/* set g(x) = x^N − 1 */
	fmpz_poly_init(g);
	fmpz_poly_set_coeff_si(g, 0, -1);
	fmpz_poly_set_coeff_si(g, ctx->N, 1);

	/* avoid side effects */
	fmpz_poly_init(a_tmp);
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(Fp);

	while (1) {
		while (fmpz_is_zero(fmpz_poly_get_coeff_ptr(f, 0))) {
			for (unsigned int i = 1; i <= ctx->N; i++) {
				fmpz *f_coeff_tmp = fmpz_poly_get_coeff_ptr(f, i);
				fmpz *c_coeff_tmp = fmpz_poly_get_coeff_ptr(c, ctx->N - i);

				/* f(x) = f(x) / x */
				fmpz_poly_set_coeff_fmpz_n(f, i - 1,
						f_coeff_tmp);

				/* c(x) = c(x) * x */
				fmpz_poly_set_coeff_fmpz_n(c, ctx->N + 1 - i,
						c_coeff_tmp);
			}

			fmpz_poly_set_coeff_si(f, ctx->N, 0);
			fmpz_poly_set_coeff_si(c, 0, 0);

			k++;

			if (fmpz_poly_degree(f) == -1) {
				retval = false;
				goto cleanup;
			}
		}

		if (fmpz_poly_degree(f) == 0)
			break;

		if (fmpz_poly_degree(f) < fmpz_poly_degree(g)) {
			/* exchange f and g and exchange b and c */
			fmpz_poly_swap(f, g);
			fmpz_poly_swap(b, c);
		}

		{
			fmpz_poly_t c_tmp,
						g_tmp;
			fmpz_t u,
				   mp_tmp;

			fmpz_init(u);
			fmpz_zero(u);

			fmpz_init_set(mp_tmp, fmpz_poly_get_coeff_ptr(f, 0));

			fmpz_poly_init(g_tmp);
			fmpz_poly_set(g_tmp, g);

			fmpz_poly_init(c_tmp);
			fmpz_poly_set(c_tmp, c);

			/* u = f[0] * g[0]^(-1) mod p */
			  /* = (f[0] mod p) * (g[0] inverse mod p) mod p */
			fmpz_invmod_ui(u,
					fmpz_poly_get_coeff_ptr(g, 0),
					ctx->p);
			fmpz_mod_ui(mp_tmp, mp_tmp, ctx->p);
			fmpz_mul(u, mp_tmp, u);
			fmpz_mod_ui(u, u, ctx->p);

			/* f = f - u * g mod p */
			fmpz_poly_scalar_mul_fmpz(g_tmp, g_tmp, u);
			fmpz_poly_sub(f, g_tmp, f);
			fmpz_poly_mod_unsigned(f, ctx->p);

			/* b = b - u * c mod p */
			fmpz_poly_scalar_mul_fmpz(c_tmp, c_tmp, u);
			fmpz_poly_sub(b, c_tmp, b);
			fmpz_poly_mod_unsigned(b, ctx->p);

			fmpz_clear(u);
			fmpz_poly_clear(g_tmp);
			fmpz_poly_clear(c_tmp);
		}
	}

	k = k % ctx->N;

	b_last = fmpz_poly_get_coeff_ptr(b, ctx->N);
	if (b_last && fmpz_cmp_si(b_last, 0)) {
		retval = false;
		goto cleanup;
	}

	/* Fp(x) = x^(N-k) * b(x) */
	for (int i = ctx->N - 1; i >= 0; i--) {
		fmpz *b_i;

		/* b(X) = f[0]^(-1) * b(X) (mod p) */
		{
			fmpz_t mp_tmp;

			fmpz_init(mp_tmp);

			fmpz_invmod_ui(mp_tmp,
					fmpz_poly_get_coeff_ptr(f, 0),
					ctx->p);

			if (fmpz_poly_get_coeff_ptr(b, i)) {
				fmpz_mul(fmpz_poly_get_coeff_ptr(b, i),
						fmpz_poly_get_coeff_ptr(b, i),
						mp_tmp);
				fmpz_mod_ui(fmpz_poly_get_coeff_ptr(b, i),
						fmpz_poly_get_coeff_ptr(b, i),
						ctx->p);
			}
		}

		j = i - k;
		if (j < 0)
			j = j + ctx->N;

		b_i = fmpz_poly_get_coeff_ptr(b, i);
		fmpz_poly_set_coeff_fmpz_n(Fp, j, b_i);
	}

	/* check if the f * Fp = 1 (mod p) condition holds true */
	fmpz_poly_set(a_tmp, a);
	poly_starmultiply(a_tmp, Fp, a_tmp, ctx, ctx->p);
	if (!fmpz_poly_is_one(a_tmp))
		retval = false;

cleanup:
	fmpz_poly_clear(a_tmp);
	fmpz_poly_clear(b);
	fmpz_poly_clear(c);
	fmpz_poly_clear(f);
	fmpz_poly_clear(g);

	if (!retval)
		fmpz_poly_zero(Fp);

	return retval;
}

/**
 * Draws a polynomial to stdout.
 *
 * @param poly draw this
 */
void poly_draw(fmpz_poly_t poly)
{
	fmpz_poly_print(poly);
	flint_printf("\n");
}

/**
 * Draws a polynomial to stdout,
 * in pretty format.
 *
 * @param poly draw this
 */
void poly_draw_pretty(fmpz_poly_t poly)
{
	fmpz_poly_print_pretty(poly, "x");
	flint_printf("\n");
}
