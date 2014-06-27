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
 * @file ntru_poly.c
 * This files purpose is to handle polynomials
 * in general, allowing modification, arithmetic
 * and common algorithms like inverting them.
 * @brief operations on polynomials
 */

#include "ntru_err.h"
#include "ntru_mem.h"
#include "ntru_params.h"
#include "ntru_poly.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Find the inverse polynomial modulo a power of 2,
 * which is q.
 *
 * @param Fq polynomial, must be initialized [out]
 * @param a polynomial to invert
 * @param params NTRU parameters
 */
static
void poly_mod2_to_modq(fmpz_poly_t Fq,
		const fmpz_poly_t a,
		const ntru_params *params);


/*------------------------------------------------------------------------*/

static void
poly_mod2_to_modq(fmpz_poly_t Fq,
		const fmpz_poly_t a,
		const ntru_params *params)
{
	int v = 2;
	fmpz_poly_t poly_tmp, two;

	fmpz_poly_init(poly_tmp);
	fmpz_poly_zero(poly_tmp);
	fmpz_poly_init(two);
	fmpz_poly_set_coeff_ui(two, 0, 2);

	while (v < (int)(params->q)) {
		v = v * 2;

		poly_starmultiply(poly_tmp, a, Fq, params, v);
		fmpz_poly_sub(poly_tmp, two, poly_tmp);
		fmpz_poly_mod_unsigned(poly_tmp, v);
		poly_starmultiply(Fq, Fq, poly_tmp, params, v);

	}

	fmpz_poly_clear(poly_tmp);
	fmpz_poly_clear(two);

}

/*------------------------------------------------------------------------*/

int
fmpz_cmp_si_n(const fmpz_t f, slong g)
{
	fmpz_t f_cmp;

	fmpz_init(f_cmp);

	if (f)
		fmpz_set(f_cmp, f);
	else
		fmpz_set_si(f_cmp, 0);

	return fmpz_cmp_si(f_cmp, g);
}

/*------------------------------------------------------------------------*/

void
poly_new(fmpz_poly_t new_poly,
		int const * const c,
		const size_t len)
{
	if (!new_poly)
		NTRU_ABORT_DEBUG("Unexpected NULL parameter in");

	fmpz_poly_init(new_poly);

	for (uint32_t i = 0; i < len; i++)
		fmpz_poly_set_coeff_si(new_poly, i, c[i]);
}

/*------------------------------------------------------------------------*/

void
poly_delete(fmpz_poly_t poly)
{
	fmpz_poly_clear(poly);
}

/*------------------------------------------------------------------------*/

void
poly_delete_array(fmpz_poly_t **poly_array)
{
	uint32_t i = 0;

	while(poly_array[i]) {
		poly_delete(*(poly_array[i]));
		free(poly_array[i]);
		i++;
	}

	/* avoid double free */
	if (i > 1)
		free(poly_array);
}

/*------------------------------------------------------------------------*/

void
poly_delete_all(fmpz_poly_t poly, ...)
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

/*------------------------------------------------------------------------*/

void
fmpz_poly_mod_unsigned(fmpz_poly_t a,
		const uint32_t mod)
{
	nmod_poly_t nmod_tmp;

	nmod_poly_init(nmod_tmp, mod);

	fmpz_poly_get_nmod_poly(nmod_tmp, a);
	fmpz_poly_set_nmod_poly_unsigned(a, nmod_tmp);

	nmod_poly_clear(nmod_tmp);
}

/*------------------------------------------------------------------------*/

void
fmpz_poly_mod(fmpz_poly_t a,
		const uint32_t mod)
{
	nmod_poly_t nmod_tmp;

	nmod_poly_init(nmod_tmp, mod);

	fmpz_poly_get_nmod_poly(nmod_tmp, a);
	fmpz_poly_set_nmod_poly(a, nmod_tmp);

	nmod_poly_clear(nmod_tmp);
}

/*------------------------------------------------------------------------*/

void
fmpz_poly_set_coeff_fmpz_n(fmpz_poly_t poly, slong n,
		const fmpz_t x)
{
	if (x)
		fmpz_poly_set_coeff_fmpz(poly, n, x);
	else
		fmpz_poly_set_coeff_si(poly, n, 0);
}

/*------------------------------------------------------------------------*/

int
fmpz_invmod_ui(fmpz_t f, const fmpz_t g, const uint32_t mod)
{
	fmpz_t modulus;

	fmpz_init_set_ui(modulus, mod);

	return fmpz_invmod(f, g, modulus);
}

/*------------------------------------------------------------------------*/

void
fmpz_add_n(fmpz_t f, const fmpz_t g, const fmpz_t h)
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

/*------------------------------------------------------------------------*/

void
poly_starmultiply(fmpz_poly_t c,
		const fmpz_poly_t a,
		const fmpz_poly_t b,
		const ntru_params *params,
		uint32_t modulus)
{
	fmpz_poly_t a_tmp;
	fmpz_t c_coeff_k;

	fmpz_poly_init(a_tmp);
	fmpz_init(c_coeff_k);

	/* avoid side effects */
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(c);

	for (int k = params->N - 1; k >= 0; k--) {
		int j;

		j = k + 1;

		fmpz_set_si(c_coeff_k, 0);

		for (int i = params->N - 1; i >= 0; i--) {
			fmpz *a_tmp_coeff_i,
				 *b_coeff_j;

			if (j == (int)(params->N))
				j = 0;

			a_tmp_coeff_i = fmpz_poly_get_coeff_ptr(a_tmp, i);
			b_coeff_j = fmpz_poly_get_coeff_ptr(b, j);

			if (fmpz_cmp_si_n(a_tmp_coeff_i, 0) &&
					fmpz_cmp_si_n(b_coeff_j, 0)) {
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

/*------------------------------------------------------------------------*/

bool
poly_inverse_poly_q(fmpz_poly_t Fq,
		const fmpz_poly_t a,
		const ntru_params *params)
{
	bool retval = false;
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
	fmpz_poly_set_coeff_si(g, params->N, 1);

	/* avoid side effects */
	fmpz_poly_init(a_tmp);
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(Fq);

	while (1) {
		while (fmpz_is_zero(fmpz_poly_get_coeff_ptr(f, 0))) {
			for (uint32_t i = 1; i <= params->N; i++) {
				fmpz *f_coeff = fmpz_poly_get_coeff_ptr(f, i);
				fmpz *c_coeff = fmpz_poly_get_coeff_ptr(c, params->N - i);

				/* f(x) = f(x) / x */
				fmpz_poly_set_coeff_fmpz_n(f, i - 1,
						f_coeff);

				/* c(x) = c(x) * x */
				fmpz_poly_set_coeff_fmpz_n(c, params->N + 1 - i,
						c_coeff);
			}

			fmpz_poly_set_coeff_si(f, params->N, 0);
			fmpz_poly_set_coeff_si(c, 0, 0);

			k++;

			if (fmpz_poly_degree(f) == -1)
				goto cleanup;
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

	k = k % params->N;

	b_last = fmpz_poly_get_coeff_ptr(b, params->N);
	if (fmpz_cmp_si_n(b_last, 0))
		goto cleanup;

	/* Fq(x) = x^(N-k) * b(x) */
	for (int i = params->N - 1; i >= 0; i--) {
		fmpz *b_i;

		j = i - k;

		if (j < 0)
			j = j + params->N;

		b_i = fmpz_poly_get_coeff_ptr(b, i);
		fmpz_poly_set_coeff_fmpz_n(Fq, j, b_i);
	}

	poly_mod2_to_modq(Fq, a_tmp, params);

	/* check if the f * Fq = 1 (mod p) condition holds true */
	fmpz_poly_set(a_tmp, a);
	poly_starmultiply(a_tmp, a_tmp, Fq, params, params->q);
	if (fmpz_poly_is_one(a_tmp))
		retval = true;
	else
		fmpz_poly_zero(Fq);

cleanup:
	fmpz_poly_clear(a_tmp);
	fmpz_poly_clear(b);
	fmpz_poly_clear(c);
	fmpz_poly_clear(f);
	fmpz_poly_clear(g);

	return retval;
}

/*------------------------------------------------------------------------*/

bool
poly_inverse_poly_p(fmpz_poly_t Fp,
		const fmpz_poly_t a,
		const ntru_params *params)
{
	bool retval = false;
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
	fmpz_poly_set_coeff_si(g, params->N, 1);

	/* avoid side effects */
	fmpz_poly_init(a_tmp);
	fmpz_poly_set(a_tmp, a);
	fmpz_poly_zero(Fp);

	while (1) {
		while (fmpz_is_zero(fmpz_poly_get_coeff_ptr(f, 0))) {
			for (uint32_t i = 1; i <= params->N; i++) {
				fmpz *f_coeff = fmpz_poly_get_coeff_ptr(f, i);
				fmpz *c_coeff = fmpz_poly_get_coeff_ptr(c, params->N - i);

				/* f(x) = f(x) / x */
				fmpz_poly_set_coeff_fmpz_n(f, i - 1,
						f_coeff);

				/* c(x) = c(x) * x */
				fmpz_poly_set_coeff_fmpz_n(c, params->N + 1 - i,
						c_coeff);
			}

			fmpz_poly_set_coeff_si(f, params->N, 0);
			fmpz_poly_set_coeff_si(c, 0, 0);

			k++;

			if (fmpz_poly_degree(f) == -1)
				goto cleanup;
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
					params->p);
			fmpz_mod_ui(mp_tmp, mp_tmp, params->p);
			fmpz_mul(u, mp_tmp, u);
			fmpz_mod_ui(u, u, params->p);

			/* f = f - u * g mod p */
			fmpz_poly_scalar_mul_fmpz(g_tmp, g_tmp, u);
			fmpz_poly_sub(f, g_tmp, f);
			fmpz_poly_mod_unsigned(f, params->p);

			/* b = b - u * c mod p */
			fmpz_poly_scalar_mul_fmpz(c_tmp, c_tmp, u);
			fmpz_poly_sub(b, c_tmp, b);
			fmpz_poly_mod_unsigned(b, params->p);

			fmpz_clear(u);
			fmpz_poly_clear(g_tmp);
			fmpz_poly_clear(c_tmp);
		}
	}

	k = k % params->N;

	b_last = fmpz_poly_get_coeff_ptr(b, params->N);
	if (fmpz_cmp_si_n(b_last, 0))
		goto cleanup;

	/* Fp(x) = x^(N-k) * b(x) */
	for (int i = params->N - 1; i >= 0; i--) {
		fmpz *b_i;

		/* b(X) = f[0]^(-1) * b(X) (mod p) */
		{
			fmpz_t mp_tmp;

			fmpz_init(mp_tmp);

			fmpz_invmod_ui(mp_tmp,
					fmpz_poly_get_coeff_ptr(f, 0),
					params->p);

			if (fmpz_poly_get_coeff_ptr(b, i)) {
				fmpz_mul(fmpz_poly_get_coeff_ptr(b, i),
						fmpz_poly_get_coeff_ptr(b, i),
						mp_tmp);
				fmpz_mod_ui(fmpz_poly_get_coeff_ptr(b, i),
						fmpz_poly_get_coeff_ptr(b, i),
						params->p);
			}
		}

		j = i - k;
		if (j < 0)
			j = j + params->N;

		b_i = fmpz_poly_get_coeff_ptr(b, i);
		fmpz_poly_set_coeff_fmpz_n(Fp, j, b_i);
	}

	/* check if the f * Fp = 1 (mod p) condition holds true */
	fmpz_poly_set(a_tmp, a);
	poly_starmultiply(a_tmp, a_tmp, Fp, params, params->p);
	if (fmpz_poly_is_one(a_tmp))
		retval = true;
	else
		fmpz_poly_zero(Fp);

cleanup:
	fmpz_poly_clear(a_tmp);
	fmpz_poly_clear(b);
	fmpz_poly_clear(c);
	fmpz_poly_clear(f);
	fmpz_poly_clear(g);

	return retval;
}

/*------------------------------------------------------------------------*/

void
poly_draw(const fmpz_poly_t poly)
{
	fmpz_poly_print(poly);
	flint_printf("\n");
}

/*------------------------------------------------------------------------*/

void
poly_draw_pretty(const fmpz_poly_t poly)
{
	fmpz_poly_print_pretty(poly, "x");
	flint_printf("\n");
}

/*------------------------------------------------------------------------*/
