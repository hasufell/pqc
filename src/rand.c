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
#include <fcntl.h>
#include "rand.h"
#include <math.h>
#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tompoly.h>
#include <tommath.h>
#include <unistd.h>

/*
 * static declarations
 */
static mp_digit read_int_dev_random(void);
static mp_digit read_int_dev_urandom(void);

static mp_digit make_small_int(mp_digit random_int, int* sign);

static mp_int *make_big_int(mp_int *upper_bound, mp_int *lower_bound,
		mp_digit randim_int);
static int check_polynom(pb_poly *polynom);

/**
 * Reads a single mp_digit out of /dev/random and returns this mp_digit
 * 
 * @return the randomly chosen integer
 */
static mp_digit read_int_dev_random(void)
{
	int random_data;
	mp_digit random_int;
	size_t randomDataLen = 0;
	random_data = open("/dev/random", O_RDONLY);

	while (randomDataLen < sizeof(random_int)) {
		ssize_t result = read(random_data,
				((char*) &random_int) + randomDataLen,
				(sizeof(random_int)) - randomDataLen);
		if (result < 0) {
			NTRU_ABORT("Unable to read /dev/random.\n");
		}
		randomDataLen += result;
	}
	close(random_data);
	return random_int;
}

/**
 * Reads a single mp_digit out of /dev/urandom and returns this mp_digit
 *
 * @return the randomly chosen integer
 */
static mp_digit read_int_dev_urandom(void)
{
	int random_data;
	mp_digit random_int;
	ssize_t result;
	random_data = open("/dev/urandom", O_RDONLY);

	if ((result = read(random_data, &random_int, sizeof(random_int))) < 0)
		NTRU_ABORT("Unable to read /dev/urandom.\n");
	close(random_data);
	return random_int;
}

/**
 * Makes a small integer from the set {-1, 0, 1}
 * out of a randomly chosen integer.
 * A zero is signed positiv.
 *
 * @param random_int a randomly chosen mp_digit [out]
 * @param sign a integer to store the sign (1==positiv) [out]
 * @return random small integer from the set {-1, 0, 1}
 */
static mp_digit make_small_int(mp_digit random_int, int* sign)
{
	random_int = random_int % 3;

	if (random_int == 1) {
		*sign = 0;
	} else if (random_int == 2) {
		random_int = 1;
		*sign = 1;
	} else {
		*sign = 0;
	}
	return random_int;
}

/**
 * Makes a big integer from the borders of upper_bound
 * and lower_bound out of a randomly chosen integer.
 *
 * @param upper_bound the maximal upper border of the resulting mp_int [out]
 * @param lower_bound the minimal lower border of the resulting mp_int [out]
 * @param randim_int TODO
 * @return a mp_int with the random number
 */
static mp_int *make_big_int(mp_int *upper_bound, mp_int *lower_bound,
		mp_digit randim_int)
{
	mp_int result;
	init_integer(&result);

	//TODO

	return result;
}

/**
 * Checks if the coefficients of a polynom are less then
 * PERCENTAGE_OF_ZERO_ALLOWED zero
 *
 * @param polynom a pointer to the polynom you want to test [out]
 * @return 0 if the polynom zero coefficients are under
 *         PERCENTAGE_OF_ZERO_ALLOWED percent
 *         -1 if the polynom zero coefficients are over
 *         PERCENTAGE_OF_ZERO_ALLOWED percent
 */
static int check_polynom(pb_poly *polynom)
{
	int result = -1;
	//TODO
	return result;
}

///**
// * Makes a big integer from the borders of BIG_RAND_MAX
// * and BIG_RAND_MIN out of a randomly chosen integer.
// *
// * @param random_int a randomly chosen mp_digit [out]
// * @param sign a integer to store the sign (1==positiv) [out]
// * @return random big integer from the borders of BIG_RAND_MAX and BIG_RAND_MIN
// */
//static mp_digit make_big_int(mp_digit random_int, int* sign)
//{
//	random_int = random_int % abs(BIG_RAND_MAX - BIG_RAND_MIN);
//
//	if (random_int < BIG_RAND_MAX) {
//		*sign = 1;
//	} else if (random_int > BIG_RAND_MAX) {
//		*sign = 0;
//		random_int -= BIG_RAND_MAX;
//	} else if (random_int == BIG_RAND_MAX) {
//		random_int = abs(BIG_RAND_MIN);
//		*sign = 0;
//	} else {
//		NTRU_ABORT("Error while parsing big random Integer.\n");
//	}
//
//	return random_int;
//}

/**
 * Gets a random polynomial with coefficients
 * from the set {-1 ,0 ,1} using /dev/random.
 *
 * @param ctx the NTRU context [out]
 * @return newly allocated polynomial, must be freed with delete_polynom()
 */
pb_poly *ntru_get_rnd_poly_small(ntru_context *ctx)
{
	mp_int chara;
	init_integer(&chara);
	pb_poly *poly = malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, ctx->N);
	mp_clear(&chara);

	for (unsigned int i = 0; i < ctx->N; i++) {
		int sign;
		mp_digit c = read_int_dev_random();
		c = make_small_int(c, &sign);
		mp_set(&(poly->terms[i]), c);
		if (sign == 1)
			poly->terms[i].sign = 1;
	}
	poly->used = ctx->N;
	pb_clamp(poly);

	return poly;
}

/**
 * Gets a random polynomial with coefficients
 * from the set {-1 ,0 ,1} using /dev/urandom.
 *
 * @param ctx the NTRU context [out]
 * @return newly allocated polynomial, must be freed with delete_polynom()
 */
pb_poly *ntru_get_urnd_poly_small(ntru_context *ctx)
{
	mp_int chara;
	init_integer(&chara);
	pb_poly *poly = malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, ctx->N);
	mp_clear(&chara);

	for (unsigned int i = 0; i < ctx->N; i++) {
		int sign;
		mp_digit c = read_int_dev_urandom();
		c = make_small_int(c, &sign);
		mp_set(&(poly->terms[i]), c);

		if (sign == 1)
			poly->terms[i].sign = 1;
	}
	poly->used = ctx->N;
	pb_clamp(poly);

	return poly;
}

/**
 * Gets a random polynomial with coefficients
 * from the borders of BIG_RAND_MAX and
 * BIG_RAND_MIN using /dev/random.
 *
 * @param ctx the NTRU context [out]
 * @return newly allocated polynomial, must be freed with delete_polynom()
 */
pb_poly *ntru_get_rnd_poly_big(ntru_context *ctx)
{
	mp_int chara;
	init_integer(&chara);
	pb_poly *poly = malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, ctx->N);
	mp_clear(&chara);

	for (unsigned int i = 0; i < ctx->N; i++) {
		int sign;
		mp_digit c = read_int_dev_random();
		c = make_big_int(c, &sign);
		mp_set(&(poly->terms[i]), c);

		if (sign == 1)
			poly->terms[i].sign = 1;
	}
	poly->used = ctx->N;
	pb_clamp(poly);

	return poly;
}

/**
 * Gets a random polynomial with coefficients
 * from the borders of BIG_RAND_MAX and
 * BIG_RAND_MIN using /dev/urandom.
 *
 * @param ctx the NTRU context [out]
 * @return newly allocated polynomial, must be freed with delete_polynom()
 */
pb_poly *ntru_get_urnd_poly_big(ntru_context *ctx)
{
	mp_int chara;
	init_integer(&chara);
	pb_poly *poly = malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, ctx->N);
	mp_clear(&chara);

	for (unsigned int i = 0; i < ctx->N; i++) {
		int sign;
		mp_digit c = read_int_dev_urandom();
		c = make_big_int(c, &sign);
		mp_set(&(poly->terms[i]), c);

		if (sign == 1)
			poly->terms[i].sign = 1;
	}
	poly->used = ctx->N;
	pb_clamp(poly);

	return poly;
}
