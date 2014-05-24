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

#ifndef NTRU_KEYPAIR_H
#define NTRU_KEYPAIR_H


#include "context.h"

#include <fmpz_poly.h>
#include <fmpz.h>
#include <stdbool.h>


typedef struct keypair keypair;


struct keypair {
	fmpz_poly_t priv;
	fmpz_poly_t priv_inv;
	fmpz_poly_t pub;
};


bool ntru_create_keypair(
		fmpz_poly_t f,
		fmpz_poly_t g,
		keypair *pair,
		ntru_context *ctx);


#endif /* NTRU_KEYPAIR_H */
