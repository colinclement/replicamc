#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#define checkscan(val, N, r) if (N != (r = val)){\
    fprintf(stderr, "Expected %i values read %i\n", N, r);\
    exit(1);\
    }

typedef struct init init;

struct init {
    int L;
    int t[3]; //burntime, run time, save period
    int Nb;
    float *b;  // Nb betas
};

void readInputFile(const char *fname, init *initvalues);

void destroyInit(init *initvalues);

void makeOutputName(const char *fname, char *outname, char *toappend);

void writeSpins(FILE *fp, int *s, int L, int Nb);

void saveBonds(const char *filename, float *J, int L);

float pcgrand(pcg32_random_t *rng);

float pcgrandn(pcg32_random_t *rng);

void printSpins(int *s, int L);

#endif
