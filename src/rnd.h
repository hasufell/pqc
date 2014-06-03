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
 * @file rnd.h
 * Header for the internal API of rnd.c.
 * @brief header for rnd.c
 */

#ifndef NTRU_RND_H
#define NTRU_RND_H

#include "context.h"

#include <stdlib.h>

#include <fmpz_poly.h>


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
 * @param ctx the NTRU context
 * @param num_ones the number of 1 coefficients
 * @param num_neg_ones the number of -1 coefficients
 */
void
ntru_get_rnd_tern_poly_num(fmpz_poly_t poly,
		const ntru_context *ctx,
		uint32_t num_ones,
		uint32_t num_neg_ones);


#endif /* NTRU_RND_H */
