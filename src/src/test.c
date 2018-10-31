#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <cluster.h>
#include <pcg_basic.h>
#include <mc.h>
#include <ll.h>
#include <util.h>


int main(int argc, const char* argv[])
{
    if (argc != 2){
        printf("Usage: test input.txt\n");
        return 0;
    } else {
        init initvalues;
        readInputFile(argv[1], &initvalues);
        printf("L=%i\n", initvalues.L);
        printf("t=%i %i %i\n", initvalues.t[0], initvalues.t[1],
               initvalues.t[2]);
        printf("Nb=%i\n", initvalues.Nb);
        for (int i=0; i < initvalues.Nb; i++)
            printf("%f\n", initvalues.b[i]);
    }

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, 42u, 54u); // seed deterministically

    int L=4;
    int Nb=2;
    float b[] = {1.5, 1.};
    int *s = (int *)malloc(Nb*L*L*sizeof(int));
    int *label = (int *)malloc(L*L*sizeof(int));
    randomize(s, Nb, L, &rng);

    // fill random bonds
    float *J = (float *)malloc(2*L*L*sizeof(float));
    for (int i=0; i < 2*L*L; i++)
        J[i] = pcgrandn(&rng);

    for (int t=0; t < 1000; t++){
        mhsweep(s, b, Nb, J, L, &rng);
        swstep(s, b, Nb, J, label, L, &rng);
    }

    free(s);
    free(J);
    free(label);

    return 0;
}
