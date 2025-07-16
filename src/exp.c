#include <stdio.h>
#include <math.h>
#include "exp.h"

#define C -0.693147180 //ln2 for fast log2
#define T3 (0.16666666666666666666666666666667f)
#define T2 (0.5f)
#define T1 (1.0f)

void exp_golden(float* in, float* out, int N) {
    for (int i = 0; i < N; i++) {
        out[i] = exp(in[i]);
	#ifdef DEBUG
	    printf("exp(%f) = %f\n", in[i], out[i]);
        #endif
    }
    return;
}

void exp_taylor3(float* in, float* out, int N){
    for (int i = 0; i < N; i++) {
        out[i] = expth3(in[i]);
        #ifdef DEBUG
            printf("exp(%f) = %f\n", in[i], out[i]);
        #endif
    }
    return;
}

float expth3 (float x) {
    float ans = x*T3 + T2;
    ans = x*ans + T1;
    ans = x*ans + 1;
    return ans;
}

