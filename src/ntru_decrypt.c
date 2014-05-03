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
#include "context.h"

/*
 * Legend
 *
 * N : maximal degree of the polynom
 * q : "is given" (... mod q)
 * p : "is given" (... mod p)
 * f : private key
 * Fp: inverse of "modulo p"
 * e : encrypted message
 * a : result of first multiplication (StarMultiply(f, e, a, N, q))
 * d : result of second multiplication (StarMultiply(a, Fp , d, N, p)), decrypted message
 * */

// Require: N , q, p, secret key f , inverse polynomial Fp , and encrypted message e.
pb_poly* ntru_decrypt(pb_poly *encr_msg, pb_poly *private_key, pb_poly *Fp, ntru_context *context, char ** decr_msg){

	unsigned int q = context->q;
	unsigned int p = context->p;
	unsigned int N = context->N;

	// StarMultiply(f, e, a, N, q)
	pb_poly *a = build_polynom(NULL, N, context);
	pb_starmultiply(private_key, encr_msg, a, context, q);

	mp_int mp_q;
	mp_int mp_qdiv2;
	mp_int zero;

	init_integer(&mp_q);
	init_integer(&mp_qdiv2);
	init_integer(&zero);

	MP_SET_INT(&mp_q, q);
	mp_div_2(&mp_q, mp_qdiv2);
	mp_zero(&zero);

	for(int i = 0, i < N, i++){
		if(mp_cmp(&(a->terms[i]),&zero) == MP_LT) {		// Make all coefficients positive
			//a->terms[i] = a->terms[i] + q;
			mp_add((&a->terms[i]),&mp_q,(&a->terms[i]));
		}
		if(mp_cmp(&(a->terms[i]), &mp_qdiv2) == MP_GT) {	// Shift coefficients of a into range (−q/2, q/2)
			//a->terms[i] = a->terms[i] - mp_q;
			mp_sub((&a->terms[i]),&mp_q,(&a->terms[i]));
		}
	}

	pb_poly *d = build_polynom(NULL, N, context);

	// StarMultiply(a, Fp , d, N, p)
	pb_starmultiply(a, Fp, d, N, p);

	// {Decode returns the decrypted message, d, through the argument list.}
	return d;
}
