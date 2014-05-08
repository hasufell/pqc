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

#include "pcq_encrypt.h"

/*
 * encrypt the msg, using the math:
 * e = (h ∗ r) + m (mod q)
 * 
 * e = the encrypted poly
 * h = the public key
 * r = the random poly
 * m = the message poly
 * q = large mod
 * 
 * @param ctx ntru_context* the ntru context
 * @param rnd pb_poly*   	the random poly
 * @param msg pb_poly* 		the message to encrypt
 * @param pubKey pb_poly* 	the public key
 * @param out pb_poly* 		the output poly
 */
void pb_encrypt(ntru_context *ctx, 
		pb_poly *rnd,
		pb_poly *msg,
		pb_poly *pubKey,
		pb_poly *out) {

		mp_int *tmpOut;
		mp_int *tmpMsg;
		mp_int mp_mod;
		
		init_integer(&mp_mod);
		MP_SET_INT(&mp_mod,(unsigned long)ctx->q);
		
		pb_starmultiply(pubKey, rnd, out, ctx, ctx->q);
		
		tmpOut = out->terms;
		tmpMsg = msg->terms;
		
		for(unsigned int i = 0; i <= ctx->N-1; i++) {
			mp_add(tmpOut,tmpMsg,tmpOut);
			mp_mod(tmpOut,mp_mod,tmpOut);
			
			tmpOut++;
			tmpMsg++;
		}
}
