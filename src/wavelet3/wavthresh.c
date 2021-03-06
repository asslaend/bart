/* Copyright 2014. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 *
 * Authors:
 * 2013 Frank Ong <uecker@eecs.berkeley.edu>
 * 2013-2014 Martin Uecker <uecker@eecs.berkeley.edu>
 */

#define _GNU_SOURCE
#include <complex.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include "misc/misc.h"

#include "num/multind.h"
#include "num/ops.h"

#include "wavelet3/wavelet.h"

#include "wavthresh.h"


struct wavelet3_thresh_s {

	unsigned int N;
	const long* dims;
	const long* minsize;
	unsigned int flags;
	float lambda;
	bool randshift;
	int rand_state;
};


static int rand_lim(unsigned int* state, int limit)
{
        int divisor = RAND_MAX / (limit + 1);
        int retval;

        do {
                retval = rand_r(state) / divisor;

        } while (retval > limit);

        return retval;
}


static void wavelet3_thresh_apply(const void* _data, float mu, complex float* out, const complex float* in)
{
	const struct wavelet3_thresh_s* data = _data;
	long shift[data->N];
	for (unsigned int i = 0; i < data->N; i++)
		shift[i] = 0;

	if (data->randshift) {

		int levels = wavelet_num_levels(data->N, data->flags, data->dims, data->minsize, 4);

		for (unsigned int i = 0; i < data->N; i++)
			if (MD_IS_SET(data->flags, i))
				shift[i] = rand_lim((unsigned int*)&data->rand_state, 1 << levels);
	}

	wavelet3_thresh(data->N, data->lambda * mu, data->flags, shift, data->dims,
		out, in, data->minsize, 4, wavelet3_dau2);
}

static void wavelet3_thresh_del(const void* _data)
{
	const struct wavelet3_thresh_s* data = _data;
	free((void*)data->dims);
	free((void*)data->minsize);
	free((void*)data);
}


/**
 * Proximal operator for l1-norm with Wavelet transform: f(x) = lambda || W x ||_1
 *
 * @param N number of dimensions
 * @param dims dimensions of x
 * @param flags bitmask for Wavelet transform
 * @param minsize minimium size of coarse Wavelet scale
 * @param lambda threshold parameter
 * @param randshift random shifting
 */
const struct operator_p_s* prox_wavelet3_thresh_create(unsigned int N, const long dims[N], unsigned int flags, const long minsize[N], float lambda, bool randshift)
{
	struct wavelet3_thresh_s* data = xmalloc(sizeof(struct wavelet3_thresh_s));

	data->N = N;

	long* ndims = xmalloc(N * sizeof(long));
	md_copy_dims(N, ndims, dims);
	data->dims = ndims;

	long* nminsize = xmalloc(N * sizeof(long));
	md_copy_dims(N, nminsize, minsize);
	data->minsize = nminsize;

	data->flags = flags;
	data->lambda = lambda;
	data->randshift = randshift;
	data->rand_state = 1;

	return operator_p_create(N, dims, N, dims, data, wavelet3_thresh_apply, wavelet3_thresh_del);
}



