#ifndef UTIL_H
#define UTIL_H

float pcgrand(pcg32_random_t *rng);

float pcgrandn(pcg32_random_t *rng);

void printSpins(int *s, int L);

#endif
