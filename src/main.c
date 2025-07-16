#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "time_count.h"
#include "exp.h"

//extern int N;
extern float i[] __attribute((aligned(8)));
extern float o_g[] __attribute__((aligned(8))); // output matrix for golden
extern float o_t[] __attribute__((aligned(8))); // output matrix for test

int main()
{
    int N = 10;
    float err = 0;
    float max_err = 0;

    uint32_t cycles_baseline, cycles_speedup;
    printf("#######################################################\n");
    printf("Test required cycle counts for %d fp32 exp calculations\n", N);
    printf("#######################################################\n");
 
    // --- CMATH: Test baseline and golden value ---
    cycles_baseline = read_cycle();
    exp_golden(i, o_g, N);
    cycles_baseline = read_cycle() - cycles_baseline;
    printf("cmath required cycles: %d\n", cycles_baseline);

    // --- SPEEDUP: Test speedup version ---
    cycles_speedup = read_cycle();
    exp_taylor3(i, o_t, N);
    cycles_speedup = read_cycle() - cycles_speedup;
    printf("Speedup version: taylor order 3\nrequired cycles: %d\n", cycles_speedup);

    // --- VERIFY: Compare Test with golden ---
    for (int k = 0; k < N; k++) {
	err = fabs(o_g[k] - o_t[k]);
        #ifdef DEBUG	
            printf("exp(%f), golden: %f, speedup: %f\n", i[k], o_g[k], o_t[k]);
        #endif
	if(err > max_err) {
		max_err = err; 
	}
    }
   
    float result = cycles_speedup / 1.0 / cycles_baseline;
    printf("#######################################################\n");
    printf("                         Result                        \n");
    printf("#######################################################\n"); 
    printf("Time reduced: %.2f%%\nMax error(abs): %.4f\n", (1.0 - result)*100.0f, max_err);
}
