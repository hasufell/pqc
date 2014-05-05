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


/** 
 * Decryption of the given Polynom with the private key, its inverse
 * and the fitting ntru_context
 *
 * @param encr_msg encrypted polynom with maximum length of N from
 * 		the given context
 * @param priv_key the polynom containing the private key to decrypt
 * 		the message
 * @param priv_key_inv the inverse polynome to the private key
 * @param context the ntru_context 
 * @param decr_msg may contain the decrypted polynome at some point
 * @returns the decrypted polynome at the moment
 * 
 * 
 */
pb_poly* ntru_decrypt(pb_poly *encr_msg, pb_poly *priv_key, 
	pb_poly *priv_key_inv, ntru_context *context, char ** decr_msg){

	unsigned int q = context->q;
	unsigned int p = context->p;
	unsigned int N = context->N;
	unsigned int i;

	pb_poly *a = build_polynom(NULL, N, context);
	pb_starmultiply(priv_key, encr_msg, a, context, q);

	mp_int mp_q;
	mp_int mp_qdiv2;
	mp_int zero;

	init_integer(&mp_q);
	init_integer(&mp_qdiv2);
	init_integer(&zero);

	MP_SET_INT(&mp_q, q);
	mp_div_2(&mp_q, &mp_qdiv2);
	mp_zero(&zero);

	for(i = 0; i < N; i++){
		if(mp_cmp(&(a->terms[i]),&zero) == MP_LT) {
			mp_add((&a->terms[i]),&mp_q,(&a->terms[i]));
		}
		if(mp_cmp(&(a->terms[i]), &mp_qdiv2) == MP_GT) {
			mp_sub((&a->terms[i]),&mp_q,(&a->terms[i]));
		}
	}

	pb_poly *d = build_polynom(NULL, N, context);

	pb_starmultiply(a, priv_key_inv, d, context, p);
	
	return d;
}
