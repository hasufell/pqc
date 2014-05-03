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

#include "ntru_decrypt.h"
#include "poly.h"

/*
 * Legend
 *
 * N : highest degree of the polynom
 * q : "is given" (... mod q)
 * p : "is given" (... mod p)
 * f : private key
 * Fp: inverse of "modulo p"
 * e : encrypted message
 * a : result of first multiplication (StarMultiply(f, e, a, N, q))
 * d : result of second multiplication (StarMultiply(a, Fp , d, N, p)), decrypted message
 * */

// Require: N , q, p, secret key f , inverse polynomial Fp , and encrypted message e.
int ntru_decrypt(char *encr_msg, pb_poly *private_key, ntru_context *context, char ** decr_msg){
	// toDo q = ?, p = ?, fp = ?

	//toDO StarMultiply(f, e, a, N, q)

	for(int i = 0, i < N, i++){
		if(a[i] < 0 ) {
			a[i] = a[i] + q; // Make all coefficients positive
		}
		if(a[i] > q/2) {
			a[i] = a[i] - q // Shift coefficients of a into range (−q/2, q/2)
		}
	}
	//toDo StarMultiply(a, Fp , d, N, p)

	// {Decode returns the decrypted message, d, through the argument list.}
	return d;
}
