/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart

******************************************************************************/

#include <gmp.h>
#include "flint.h"
#include "ulong_extras.h"

int n_is_prime(mp_limb_t n)
{
    /* flint's "BPSW" checked against Feitma and Galway's database [1] 
       up to 2^64 by Dana Jacobsen.
       [1]  http://www.janfeitsma.nl/math/psp2/database
    */

#if !FLINT64
    return n_is_probabprime(n);
#else
    if (n < UWORD(10000000000000000))
        return n_is_probabprime(n);
    else 
        return n_is_probabprime_BPSW(n);
#endif
}
