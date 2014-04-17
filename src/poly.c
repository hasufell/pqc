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

#include "context.h"
#include "err.h"
#include "poly.h"

#include <stdbool.h>
#include <stdio.h>
#include <tompoly.h>
#include <tommath.h>
#include <stdbool.h>

/**
 * Initialize a mp_int and check if this was successful, the
 * caller must free new_int with mp_clear().
 *
 * @param new_int a pointer to the mp_int you want to initialize
 */
void init_integer(mp_int *new_int)
{
	int result;
	if ((result = mp_init(new_int)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initialize a Polynom with a pb_poly and a mp_int as characteristic.
 * Checks if everything went fine. The caller must free new_poly
 * with pb_clear().
 *
 * @param new_poly the pb_poly you want to initialize
 * @param chara the characteristic
 */
void init_polynom(pb_poly *new_poly, mp_int *chara)
{
	int result;
	if ((result = pb_init(new_poly, chara)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initialize a Polynom with a pb_poly and an mp_int as characteristic
 * with size. Checks if everything went fine. The caller must free
 * new_poly with pb_clear().
 *
 * @param new_poly the pb_poly you want to initialize
 * @param chara the characteristic
 * @param size the size of the polynomial
 */
void init_polynom_size(pb_poly *new_poly, mp_int *chara, size_t size)
{
	int result;
	if ((result = pb_init_size(new_poly, chara, size)) != MP_OKAY) {
		NTRU_ABORT("Error initializing the number. %s",
				mp_error_to_string(result));
	}
}

/**
 * Initializes and builds a polynomial with the
 * coefficient values of c[] of size len within NTRU
 * context ctx and returns a newly allocated polynomial
 * pointer which is not clamped.
 *
 * If you want to fill a polyonmial of length 11 with zeros,
 * call build_polynom(NULL, 11, ctx).
 *
 * @param c array of polynomial coefficients, can be NULL
 * @param len size of the coefficient array, can be 0
 * @param ctx NTRU context
 * @return newly allocated polynomial pointer, must be freed
 * with delete_polynom()
 */
pb_poly *build_polynom(int const * const c,
		const size_t len,
		ntru_context *ctx)
{
	pb_poly *new_poly;
	mp_int chara;

	new_poly = malloc(sizeof(*new_poly));
	init_integer(&chara);
	init_polynom_size(new_poly, &chara, len);
	mp_clear(&chara);

	/* fill the polynom if c is not NULL */
	if (c) {
		for (unsigned int i = 0; i < len; i++) {
			bool sign = false;
			unsigned long unsigned_c;

			if (c[i] < 0) {
				unsigned_c = 0 - c[i];
				sign = true;
			} else {
				unsigned_c = c[i];
			}

			mp_set_int(&(new_poly->terms[i]), unsigned_c);

			if (sign == true)
				mp_neg(&(new_poly->terms[i]), &(new_poly->terms[i]));
		}
	} else { /* fill with zeros */
		for (unsigned int i = 0; i < len; i++)
			mp_set(&(new_poly->terms[i]), 0);
	}

	new_poly->used = len;

	return new_poly;
}

/**
 * Sets all the polynomial coefficients to +0.
 *
 * @param poly the polynomial
 * @param len the length of the polynomial
 */
void erase_polynom(pb_poly *poly, size_t len)
{
	for (unsigned int i = 0; i < len ; i++) {
		mp_set(&(poly->terms[i]), 0);
		mp_abs(&(poly->terms[i]), &(poly->terms[i]));
	}
}

/**
 * This deletes the internal structure of a polynomial,
 * and frees the pointer. Don't call this on stack variables,
 * this is intended for use after ntru_ functions, that
 * return a polynomial pointer.
 *
 * @param poly the polynomial to delete
 */
void delete_polynom(pb_poly *poly)
{
	pb_clear(poly);
	free(poly);
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
void pb_starmultiply(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		ntru_context *ctx,
		unsigned int modulus)
{
	pb_poly *a_tmp;
	mp_int mp_modulus;

	init_integer(&mp_modulus);
	mp_set_int(&mp_modulus, (unsigned long)(modulus));

	/* avoid side effects */
	a_tmp = build_polynom(NULL, ctx->N, ctx);
	PB_COPY(a, a_tmp);
	erase_polynom(c, ctx->N);

	for (int k = ctx->N - 1; k >= 0; k--) {
		int j;
		j = k + 1;

		for (int i = ctx->N - 1; i >= 0; i--) {
			if (j == (int)(ctx->N))
				j = 0;
			if (mp_cmp_d(&(a_tmp->terms[i]), 0) != MP_EQ &&
					mp_cmp_d(&(b->terms[j]), 0) != MP_EQ) {
				mp_int mp_tmp;

				init_integer(&mp_tmp);

				MP_MUL(&(a_tmp->terms[i]), &(b->terms[j]), &mp_tmp);
				MP_ADD(&(c->terms[k]), &mp_tmp, &(c->terms[k]));
				MP_DIV(&(c->terms[k]), &mp_modulus, NULL, &(c->terms[k]));

				mp_clear(&mp_tmp);
			}
			j++;
		}
	}
	mp_clear(&mp_modulus);
	delete_polynom(a_tmp);
}

/**
 * c = a XOR b
 *
 * @param a polynom (is allowed to be the same as param c)
 * @param b polynom
 * @param c polynom [out]
 * @param len max size of the polynoms, make sure all are
 * properly allocated
 */
void pb_xor(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		const size_t len)
{
	for (unsigned int i = 0; i < len; i++)
		MP_XOR(&(a->terms[i]), &(b->terms[i]), &(c->terms[i]));
}

/**
 * Invert the polynomial a modulo q.
 *
 * @param a polynomial to invert (is allowed to be the same as param Fq)
 * @param Fq polynomial [out]
 * @param ctx NTRU context
 * @return true/false for success/failure
 */
bool pb_inverse_poly_q(pb_poly * const a,
		pb_poly *Fq,
		ntru_context *ctx)
{
	int k = 0,
		j = 0,
		v = 2;
	int zero_poly_val = 1;
	pb_poly *a_tmp, *b, *c, *f, *g, *degree_zero_poly;

	degree_zero_poly = build_polynom(&zero_poly_val, 1, ctx);
	b = build_polynom(NULL, ctx->N, ctx);
	mp_set(&(b->terms[0]), 1);
	c = build_polynom(NULL, ctx->N, ctx);
	f = build_polynom(NULL, ctx->N, ctx);
	PB_COPY(a, f);
	a_tmp = build_polynom(NULL, ctx->N, ctx);
	PB_COPY(a, a_tmp);
	g = build_polynom(NULL, ctx->N, ctx);
	mp_set(&(g->terms[0]), 1);
	mp_neg(&(g->terms[0]), &(g->terms[0]));
	mp_set(&(g->terms[ctx->N]), 1);

	while (1) {
		while (mp_cmp_d(&(f->terms[0]), 0) == MP_EQ) {
			for (unsigned int i = 1; i <= ctx->N; i++) {
				MP_COPY(&(f->terms[i]), &(f->terms[i - 1]));
				MP_COPY(&(c->terms[ctx->N - i]), &(c->terms[ctx->N + 1 - i]));
			}
			mp_set(&(f->terms[ctx->N]), 0);
			mp_set(&(c->terms[0]), 0);
			k++;
		}

		/* hope this does not blow up in our face */
		pb_clamp(degree_zero_poly);
		pb_clamp(f);
		if (pb_cmp(f, degree_zero_poly) == PB_DEG_EQ)
			goto OUT_OF_LOOP;

		pb_clamp(g);
		if (pb_cmp(f, g) == PB_DEG_LT) {
			pb_exch(f, g);
			pb_exch(b, c);
		}

		/* draw_polynom(f); */
		/* draw_polynom(b); */
		pb_xor(f, g, f, ctx->N);
		pb_xor(b, c, b, ctx->N);
		/* draw_polynom(f); */
		/* draw_polynom(b); */
	}

OUT_OF_LOOP:
	k = k % ctx->N;

	for (int i = ctx->N - 1; i >= 0; i--) {
		j = i - k;
		if (j < 0)
			j = j + ctx->N;
		MP_COPY(&(b->terms[i]), &(Fq->terms[j]));
	}
	draw_polynom(Fq);

	while (v < (int)(ctx->q)) {
		pb_poly *pb_tmp,
				*pb_tmp2;
		mp_int tmp_v;
		pb_tmp = build_polynom(NULL, ctx->N, ctx);
		v = v * 2;
		init_integer(&tmp_v);
		mp_set_int(&tmp_v, v);
		pb_tmp2 = build_polynom(NULL, ctx->N, ctx);
		mp_set_int(&(pb_tmp2->terms[0]), 2);

		/* hope this does not blow up in our face */
		pb_starmultiply(a_tmp, Fq, pb_tmp, ctx, v);
		PB_SUB(pb_tmp2, pb_tmp, pb_tmp);
		PB_MOD(pb_tmp, &tmp_v, pb_tmp, ctx);
		pb_starmultiply(Fq, pb_tmp, Fq, ctx, v);

		mp_clear(&tmp_v);
		delete_polynom(pb_tmp);
		delete_polynom(pb_tmp2);
	}

	for (int i = ctx->N - 1; i >= 0; i--)
		if (mp_cmp_d(&(Fq->terms[i]), 0) == MP_LT) {
			mp_int mp_tmp;
			init_integer(&mp_tmp);
			mp_set_int(&mp_tmp, ctx->q);
			MP_ADD(&(Fq->terms[i]), &mp_tmp, &(Fq->terms[i]));
			mp_clear(&mp_tmp);
		}

	delete_polynom(a_tmp);
	delete_polynom(b);
	delete_polynom(c);
	delete_polynom(f);
	delete_polynom(g);
	delete_polynom(degree_zero_poly);

	/* TODO: check if the f * Fq = 1 (mod p) condition holds true */

	return true;
}
 * Print the polynomial in a human readable format to stdout.
 *
 * @param poly to draw
 */
void draw_polynom(pb_poly * const poly)
{
	int x;
	char buf[8192];

	if (poly->used == 0) {
		printf("0");
	} else {
		for (x = poly->used - 1; x >= 0; x--) {
			if (mp_iszero(&(poly->terms[x])) == MP_YES)
				continue;
			mp_toradix(&(poly->terms[x]), buf, 10);
			if ((x != poly->used - 1) && poly->terms[x].sign == MP_ZPOS) {
				printf("+");
			}
			printf(" %sx^%d ", buf, x);
		}
	}
	if (mp_iszero(&(poly->characteristic)) == MP_NO) {
		mp_toradix(&(poly->characteristic), buf, 10);
		printf(" (mod %s)", buf);
	}
	printf("\n");
}
