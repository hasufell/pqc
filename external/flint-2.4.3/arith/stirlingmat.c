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

    Copyright (C) 2010, 2013 Fredrik Johansson

******************************************************************************/

#include "arith.h"

void arith_stirling_number_1u_vec_next(fmpz * row,
    const fmpz * prev, slong n, slong klen)
{
    slong k;

    if (klen > n) fmpz_one(row + n);
    if (n != 0 && klen != 0) fmpz_zero(row);

    for (k = FLINT_MIN(n, klen) - 1; k >= 1; k--)
    {
        fmpz_mul_ui(row + k, prev + k, n - UWORD(1));
        fmpz_add(row + k, prev + k - 1, row + k);
    }

    for (k = n + 1; k < klen; k++)
        fmpz_zero(row + k);
}

void arith_stirling_number_1_vec_next(fmpz * row,
    const fmpz * prev, slong n, slong klen)
{
    slong k;

    if (klen > n) fmpz_one(row + n);
    if (n != 0 && klen != 0) fmpz_zero(row);

    for (k = FLINT_MIN(n, klen) - 1; k >= 1; k--)
    {
        fmpz_mul_ui(row + k, prev + k, n - UWORD(1));
        fmpz_sub(row + k, prev + k - 1, row + k);
    }

    for (k = n + 1; k < klen; k++)
        fmpz_zero(row + k);
}

void arith_stirling_number_2_vec_next(fmpz * row,
    const fmpz * prev, slong n, slong klen)
{
    slong k;

    if (klen > n) fmpz_one(row + n);
    if (n != 0 && klen != 0) fmpz_zero(row);

    for (k = FLINT_MIN(n, klen) - 1; k >= 1; k--)
    {
        fmpz_mul_ui(row + k, prev + k, k);
        fmpz_add(row + k, prev + k - 1, row + k);
    }

    for (k = n + 1; k < klen; k++)
        fmpz_zero(row + k);
}

void
arith_stirling_matrix_1u(fmpz_mat_t mat)
{
    slong n;

    if (fmpz_mat_is_empty(mat))
        return;

    for (n = 0; n < mat->r; n++)
        arith_stirling_number_1u_vec_next(mat->rows[n],
            mat->rows[n - (n != 0)], n, mat->c);
}

void
arith_stirling_matrix_1(fmpz_mat_t mat)
{
    slong n;

    if (fmpz_mat_is_empty(mat))
        return;

    for (n = 0; n < mat->r; n++)
        arith_stirling_number_1_vec_next(mat->rows[n],
            mat->rows[n - (n != 0)], n, mat->c);
}

void
arith_stirling_matrix_2(fmpz_mat_t mat)
{
    slong n;

    if (fmpz_mat_is_empty(mat))
        return;

    for (n = 0; n < mat->r; n++)
        arith_stirling_number_2_vec_next(mat->rows[n],
            mat->rows[n - (n != 0)], n, mat->c);
}
