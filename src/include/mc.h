#ifndef MC_H
#define MC_H

#include <pcg_basic.h>

void randomize(int *s, int Nb, int L, pcg32_random_t *rng);

void mhstep(int *s, float b, float *J, int L, pcg32_random_t *rng);

void mhsweep(int *s, float *b, int Nb, float *J, int L, pcg32_random_t *rng);

void swstep(int *s, float *b, int Nb, float *J, int *label, int L, pcg32_random_t *rng);

#endif
