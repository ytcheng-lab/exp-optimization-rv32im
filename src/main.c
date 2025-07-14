#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "time_count.h"
#include "exp.h"

//extern int N;
extern float i[] __attribute((aligned(8)));
extern float o_g[] __attribute__((aligned(8))); // output matrix for golden
extern float o_t[] __attribute__((aligned(8))); // output matrix for test

#define THRESHOLD 0.001

int main()
{
    int N = 10;

    uint32_t cycles_baseline, cycles_speedup;
    printf("#######################################################\n");
    printf("Test require cycle counts for %d fp32 exp calculations\n", N);
    printf("#######################################################\n");
 
    // --- CMATH: Test baseline and golden value ---
    cycles_baseline = read_cycle();
    exp_golden(i, o_g, N);
    cycles_baseline = read_cycle() - cycles_baseline;
    printf("CMATH require cycles: %d\n", cycles_baseline);

    // --- SPEEDUP: Test speedup version ---
    //cycles_speedup = read_cycle();
    //exp_taylor3(i, o_t, N);
    //cycles_speedup = read_cycle() - cycles_speedup;
    printf("Speedup version: taylor order 3, require cycles: %d\n", cycles_speedup);

    // --- VERIFY: Compare Test with golden ---
    for (int k = 0; k < N; k++) {
        if(o_g[k] - o_t[k] > THRESHOLD)
		printf("exp(%f), golden: %f, speedup: %f\n", i[k], o_g[k], o_t[k]);
    }
   
    float result = cycles_speedup / 1.0 / cycles_baseline;
    printf("Time reduced: %.2f%%\n", (1.0 - result)*100.0f);
}
