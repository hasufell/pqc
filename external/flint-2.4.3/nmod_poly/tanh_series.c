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

    Copyright (C) 2011 Fredrik Johansson

******************************************************************************/

#include <gmp.h>
#include "flint.h"
#include "ulong_extras.h"
#include "nmod_vec.h"
#include "nmod_poly.h"

void
_nmod_poly_tanh_series(mp_ptr f, mp_srcptr h, slong n, nmod_t mod)
{
    mp_ptr t, u;

    t = _nmod_vec_init(n);
    u = _nmod_vec_init(n);

    _nmod_vec_add(t, h, h, n, mod);
    _nmod_poly_exp_series(u, t, n, mod);
    _nmod_vec_set(t, u, n);
    t[0] = UWORD(0);
    u[0] = UWORD(2);
    _nmod_poly_div_series(f, t, u, n, mod);

    _nmod_vec_clear(t);
    _nmod_vec_clear(u);
}

void
nmod_poly_tanh_series(nmod_poly_t g, const nmod_poly_t h, slong n)
{
    mp_ptr h_coeffs;
    slong h_len = h->length;

    if (h_len > 0 && h->coeffs[0] != UWORD(0))
    {
        flint_printf("Exception (nmod_poly_tanh_series). Constant term != 0.\n");
        abort();
    }

    if (h_len == 1 || n < 2)
    {
        nmod_poly_zero(g);
        return;
    }

    nmod_poly_fit_length(g, n);

    if (h_len < n)
    {
        h_coeffs = _nmod_vec_init(n);
        flint_mpn_copyi(h_coeffs, h->coeffs, h_len);
        flint_mpn_zero(h_coeffs + h_len, n - h_len);
    }
    else
        h_coeffs = h->coeffs;

    _nmod_poly_tanh_series(g->coeffs, h_coeffs, n, h->mod);

    if (h_len < n)
        _nmod_vec_clear(h_coeffs);

    g->length = n;
	_nmod_poly_normalise(g);
}
