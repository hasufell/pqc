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

#ifndef PQC_ENCRYPT_H
#define PQC_ENCRYPT_H

#include <tommath.h>
#include <tompoly.h>
#include "context.h"
#include "poly.h"

void pb_encrypt(ntru_context *ctx, 
		pb_poly *rnd,
		pb_poly *msg,
		pb_poly *pubKey,
		pb_poly *out);

#endif /* PQC_ENCRYPT_H */
