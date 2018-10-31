#ifndef UTIL_H
#define UTIL_H

typedef struct init init;

struct init {
    int L;
    int t[3]; //burntime, run time, save period
    int Nb;
    float *b;  // Nb betas
};

void readInputFile(const char *fname, init *initvalues);

void destroyInit(init *initvalues);

float pcgrand(pcg32_random_t *rng);

float pcgrandn(pcg32_random_t *rng);

void printSpins(int *s, int L);

#endif
