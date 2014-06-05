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
 * @file ntru_rnd.h
 * This file holds the public API of generating
 * random ternary polynomials of the pqc NTRU
 * implementation and is meant to be installed
 * on the client system.
 * @brief public API, random polynomials
 */

#ifndef PUBLIC_NTRU_RND_H_
#define PUBLIC_NTRU_RND_H_


#include "ntru.h"

#include <fmpz_poly.h>
#include <fmpz.h>
#include <stdint.h>


/**
 * Get a random integer from /dev/random.
 *
 * @return random integer
 */
int
get_rnd_int(void);

/**
 * Get a pseudo random integer from /dev/urandom.
 *
 * @return pseudo-random integer.
 */
int
get_urnd_int(void);

/**
 * Get a random ternary polynomial with specified numbers
 * of 1 coefficients and -1 coefficients.
 *
 * @param poly the resulting random polynomial [out]
 * @param params the NTRU context
 * @param num_ones the number of 1 coefficients
 * @param num_neg_ones the number of -1 coefficients
 * @param rnd_int function callback which should return
 * a random integer
 */
void
ntru_get_rnd_tern_poly_num(fmpz_poly_t poly,
		const ntru_params *params,
		uint32_t num_ones,
		uint32_t num_neg_ones,
		int (*rnd_int)(void));


#endif /* PUBLIC_NTRU_RND_H_ */
