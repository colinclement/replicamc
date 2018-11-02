#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pcg_basic.h>

#include <graph.h>
#include <cluster.h>
#include <mc.h>
#include <ll.h>
#include <util.h>

#define MAXLEN 256

#define docs "Input file format:\n"\
    "L\n"\
    "burntime runtime save period\n"\
    "Nbetas\n"\
    "beta1\n"\
    "beta2\n"\
    "...\n"\
    "beta_Nbetas\n"

int main(int argc, const char* argv[])
{
    init initvalues;
    
    // Random initial seeds set by time and memory address
    unsigned int s0=time(NULL) ^ (intptr_t)&printf, s1=(intptr_t)&initvalues;

    switch (argc) {
        case 2 :
            readInputFile(argv[1], &initvalues);
            break;
        case 4 :
            readInputFile(argv[1], &initvalues);
            s0 = strtoul(argv[2], NULL, 0);
            s1 = strtoul(argv[2], NULL, 0);
            break;
        default:
            printf("Requires 1 or 3 arguments.\nUsage: test inputfile "
                   "[seed0 seed1]\n\n%s", docs);
            return 0;
    }

    char outname[MAXLEN];
    makeOutputName(argv[1], outname, NULL);
    char bondname[MAXLEN];
    makeOutputName(argv[1], bondname, "-bonds");

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, s0, s1); // seed deterministically

    int L=initvalues.L;
    int Nb=initvalues.Nb;

    float *b = initvalues.b;
    int *s = (int *)malloc(Nb*L*L*sizeof(int));
    int *label = (int *)malloc(L*L*sizeof(int));
    randomize(s, Nb, L, &rng);

    // fill random bonds
    float *J = (float *)malloc(2*L*L*sizeof(float));
    for (int i=0; i < 2*L*L; i++)
        J[i] = pcgrandn(&rng);

    saveBonds(bondname, J, L);

    // Burn in (throw away sequences)
    for (int t=0; t < initvalues.t[0]; t++){
        mhsweep(s, b, Nb, J, L, &rng);
        swstep(s, b, Nb, J, label, L, &rng);
    }

    FILE *fp = fopen(outname, "a");
    for (int t=0; t < initvalues.t[1]; t++){
        mhsweep(s, b, Nb, J, L, &rng);
        swstep(s, b, Nb, J, label, L, &rng);
        if (t % initvalues.t[2] == 0)
            writeSpins(fp, s, L, Nb);
    }

    fclose(fp);
    free(s);
    free(J);
    free(label);
    destroyInit(&initvalues);

    return 0;
}
