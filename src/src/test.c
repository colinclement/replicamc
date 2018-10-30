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

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, 42u, 54u); // seed deterministically

    int L=4;
    int Nb=2;
    float b[] = {1.5, 1.};
    int *s = (int *)malloc(Nb*L*L*sizeof(int));
    randomize(s, Nb, L, &rng);

    float *J = (float *)malloc(2*L*L*sizeof(float));
    for (int i=0; i < 2*L*L; i++)
        J[i] = pcgrandn(&rng);

    for (int t=0; t < 1000; t++){
        mhsweep(s, b, Nb, J, L, &rng);
        swstep(s, b, Nb, J, L, &rng);
    }

    free(s);
    free(J);

    return 0;
}
