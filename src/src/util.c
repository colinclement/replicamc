#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pcg_basic.h>
#include <util.h>

#define RAND32_MAX 4294967295 // 2**32 - 1

void readInputFile(const char *fname, init *initvalues){
    /* reads file fname and populates init struct
     *
     * expected file structure:
     * L
     * burntime runtime savetime
     * Nb
     * b0
     * b1
     * b2
     * ...
     *  b_(Nb-1)
     */
    FILE *fp = fopen(fname, "r");
    if (NULL == fp){
        fprintf(stderr, "Unable to open file %s", fname);
        exit(1);
    }

    fscanf(fp, "%i", &(initvalues->L));
    fscanf(fp, "%i %i %i", initvalues->t, initvalues->t+1, initvalues->t+2);
    fscanf(fp, "%i", &(initvalues->Nb));

    initvalues->b = (float *)malloc(initvalues->Nb * sizeof(float));
    int i=0;
    while (fscanf(fp, "%f", initvalues->b + i) != EOF){
        i++;
        //fgetc(fp);  // moves past newline
        if (i > initvalues->Nb){
             fprintf(stderr, "Found %i betas, expected %i\n", i, 
                    initvalues->Nb);
             exit(1);
        }
    }
    if (i < initvalues->Nb){
            fprintf(stderr, "Found %i betas, expected %i\n", i, 
                    initvalues->Nb);
            exit(1);
        }
     
    printf("%i\n", i);
    fclose(fp);
}

void destroyInit(init *initvalues){
    free(initvalues->b);
}

float pcgrand(pcg32_random_t *rng){
    assert(NULL != rng);
    return pcg32_random_r(rng) * (1.0 / RAND32_MAX);
}

float pcgrandn(pcg32_random_t *rng){
    assert(NULL != rng);

    float U1, U2, W=0, mult;
    static float X1, X2;  // save these values between calls!
    static int call = 0;

    if (call == 1){
        call = !call;
        return (float) X2;
    }

    while (W >= 1 || W == 0){
        U1 = 2*pcgrand(rng)-1;
        U2 = 2*pcgrand(rng)-1;
        W = U1*U1 + U2*U2;
    }

    mult = sqrt((-2 * log(W))/W);
    X1 = U1 * mult;
    X2 = U2 * mult;
    call = !call;
    return X1;
}

void printSpins(int *s, int L) {
    assert(NULL != s);
    assert(L > 0);

    for (int i=0; i < L; i++){
        for (int j=0; j < L; j++){
            printf("%i ", s[i*L + j]);
        }
        printf("\n");
    }
}
