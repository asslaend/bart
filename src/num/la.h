/* Copyright 2013-2014. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by 
 * a BSD-style license which can be found in the LICENSE file.
 */ 

#include <complex.h>

extern void mat_identity(int A, int B, complex float x[A][B]);
extern void mat_gaussian(int A, int B, complex float x[A][B]);
extern void mat_mul(int A, int B, int C, complex float x[A][C], const complex float y[A][B], const complex float z[B][C]);
extern void mat_transpose(int A, int B, complex float dst[B][A], const complex float src[A][B]);
extern void mat_adjoint(int A, int B, complex float dst[B][A], const complex float src[A][B]);
extern void mat_copy(int A, int B, complex float dst[A][B], const complex float src[A][B]);
// extern complex double vec_dot(int N, const complex float x[N], const complex float y[N]);
extern complex float vec_dot(int N, const complex float x[N], const complex float y[N]);
extern void vec_saxpy(int N, complex float x[N], complex float alpha, const complex float y[N]);
extern void gram_matrix(int N, complex float cov[N][N], int L, const complex float data[N][L]);
extern void gram_schmidt(int M, int N, float val[N], complex float vecs[M][N]);
extern void gram_matrix2(int N, complex float cov[N * (N + 1) / 2], int L, const complex float data[N][L]);
extern void pack_tri_matrix(int N, complex float cov[N * (N + 1) / 2], const complex float m[N][N]);
extern void unpack_tri_matrix(int N, complex float m[N][N], const complex float cov[N * (N + 1) / 2]);
extern void orthiter_noinit(int M, int N, int iter, float vals[M], complex float out[M][N], const complex float matrix[N][N]);
extern void orthiter(int M, int N, int iter, float vals[M], complex float out[M][N], const complex float matrix[N][N]);
extern void cholesky(int N, complex float A[N][N]);
extern void cholesky_solve(int N, complex float x[N], const complex float L[N][N], const complex float b[N]);
extern void cholesky_double(int N, complex double A[N][N]);
extern void cholesky_solve_double(int N, complex double x[N], const complex double L[N][N], const complex double b[N]);
extern complex float vec_mean(long D, const complex float src[D]);
extern void vec_axpy(long N, complex float x[N], complex float alpha, const complex float y[N]);
extern void vec_sadd(long D, complex float alpha, complex float dst[D], const complex float src[D]);

#if 1
#include "misc/pcaa.h"

#define mat_mul(A, B, C, x, y, z) \
	mat_mul(A, B, C, x, AR2D_CAST(complex float, A, B, y), AR2D_CAST(complex float, B, C, z))

#define mat_copy(A, B, x, y) \
	mat_copy(A, B, x, AR2D_CAST(complex float, A, B, y))

#define mat_transpose(A, B, x, y) \
	mat_transpose(A, B, x, AR2D_CAST(complex float, A, B, y))

#define mat_adjoint(A, B, x, y) \
	mat_adjoint(A, B, x, AR2D_CAST(complex float, A, B, y))

#define pack_tri_matrix(N, cov, m) \
	pack_tri_matrix(N, cov, AR2D_CAST(complex float, N, N, m))

#define orthiter(M, N, iter, vals, out, matrix) \
	orthiter(M, N, iter, vals, out, AR2D_CAST(complex float, N, N, matrix))

#define orthiter_noinit(M, N, iter, vals, out, matrix) \
	orthiter_noinit(M, N, iter, vals, out, AR2D_CAST(complex float, N, N, matrix))

#define gram_matrix2(N, cov, L, data) \
	gram_matrix2(N, cov, L, AR2D_CAST(complex float, N, L, data))
#endif


