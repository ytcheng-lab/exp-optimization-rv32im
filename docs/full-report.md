# Exponential Function Approximation for RV32IM Processors: Technical Report

## 1. Executive Summary

This document describes the software-level optimization of the exponential function for RISC-V RV32IM-class processors without hardware FPU support. The exponential function is a critical kernel in AI workloads such as softmax, sigmoid, and tanh. On minimal embedded cores lacking floating-point hardware, naïve implementations are prohibitively expensive. By analyzing standard library implementations and introducing approximation techniques such as Taylor series expansion, Horner's method, and lookup tables (LUT), this work demonstrates significant reductions in instruction-level cycle counts, making AI workloads more feasible on resource-constrained platforms.

---

## 2. Background and Motivation

### 2.1 Problem Context

* Many AI workloads require the exponential function, notably in softmax activation layers.
* RV32IM processors lack the F-extension (floating-point unit). All FP operations require software emulation.
* Exponential calculations are nonlinear, demanding precision but consuming high cycle counts on such cores.

### 2.2 Motivation

* Optimize exponential computation at the software level.
* Make AI inference on low-cost, low-power embedded systems.
* Provide insights for developers working on RISC-V-based AI inference.

---

## 3. Target Platform and Toolchain

* ISA: RISC-V RV32IM (no FPU)
* Toolchain: riscv-gnu-toolchain
* Simulator: Spike (for functional simulation)
* Cycle Measurement: Reading the mcycle CSR during execution to approximate dynamic cycle counts.

*Note:* Spike does not model microarchitectural stalls, caches, or pipelines. mcycle serves as a consistent proxy for dynamic instruction-level cost, enabling relative comparisons between optimization strategies.

---

## 4. Bottleneck Analysis of Existing Implementations

### 4.1 Study of GNU C Library (glibc) Implementation

* Uses range reduction via ln(2) scaling.
* Splits input into integer and fractional components.
* Integer component handled via bit-shifts and small LUT for precomputed powers of two.
* Fractional remainder approximated with low-degree Taylor expansion.
* Limitations on RV32IM:
  * Modular reductions and divisions are costly.
  * Type conversions between integer and floating-point are expensive.
  * LUT indexing incurs overhead.

### 4.2 Study of Cephes Implementation

* Uses Pade approximations for better precision.
* Performs range reduction similarly to glibc.
* Avoids LUTs in favor of more complex polynomial approximations.
* Limitations on RV32IM:
  * Division and floor operations required for range reduction.
  * Higher computational cost for polynomial evaluation.

---

## 5. Proposed Approximation Strategies

### 5.1 Taylor Series Approximation

* Approximates exp(x) over small intervals.
* Tunable degree allows trade-off between accuracy and cycle cost.
* Example used: (3rd-degree Taylor series approximation).
* On RV32IM, low-degree polynomials reduce multiplication count significantly.

### 5.2 Horner's Method

* Reduces polynomial evaluation complexity from quadratic to linear.
* Rewrites nested multiplications into a form with fewer operations.
* Improves performance on simple in-order pipelines.

### 5.3 Lookup Table (LUT)-Based Decomposition

* Decomposes x into integer and fractional parts (x = n + f).
* Uses LUT for precomputed exp(n) values.
* Evaluates exp(f) using low-degree Taylor approximation in the range \[0, 1).
* Reducing the required dynamic range of the approximation.

---

## 6. Implementation Details

### 6.1 Data Preparation

* Fixed input dataset defined in an assembly (.S) file to ensure reproducibility.
* Input and output arrays aligned for efficient access.

### 6.2 Modular Code Structure

* exp.c / exp.h: Contains multiple approximation methods.
  * `softmax_golden`: Calls standard exp() from math.h and computes softmax as baseline.
  * `softmax_taylor`: Uses 3rd-degree Taylor approximation with Horner's method.
  * `softmax_taylor_LUT`: Uses LUT plus Taylor approximation for higher precision.
  * `expth3`: Core function used in `softmax_taylor`, computes exp(x) using 3rd-degree Taylor approximation.
  * `expth3_LUT`: Core function used in `softmax_taylor_LUT`, computes exp(x) using LUT decomposition.

* main.c: Benchmark driver.
  * Loads input data.
  * Measures cycle counts using mcycle CSR.
  * Computes and prints maximum error compared to golden output.

* Makefile:
  * Cross-compiles for RISC-V target.
  * Supports flags to enable SPEEDUP or CHECK modes.

---

## 7. Experiment Design

* Input size: 512 single-precision floats.
* Baseline measurement:
  * Standard glibc exp() function called on all inputs.
  * Measures mcycle before and after for total dynamic cycle count.
* Optimized measurement:
  * 3rd-degree Taylor approximation with Horner's method.
  * LUT plus Taylor combination.
  * Same cycle measurement strategy.
* Error checking:
  * Optional compile flag to compute maximum absolute error between golden and approximated outputs.

*Cycle counts obtained via reading the mcycle CSR in code.*
*Spike simulation provides an instruction-level proxy without microarchitectural effects.*

---

## 8. Benchmark Results

| Method                | Cycle Count | Time Reduction | Max Absolute Error |
| --------------------- | ----------- | -------------- | -----------------  |
| glibc expf (baseline) | 1,265,689   | 0%             | 0                  |
| Taylor3 (no LUT)      | 386,938     | 69.43%         | 0.0132             |
| Taylor3 + LUT         | 533,376     | 57.86%         | 0.0003             |

* Inputs: 512 float32 uniformly distributed in range \[0, 10)
* All results obtained via Spike simulation (RV32IM, no FPU)
* Measurements based on mcycle CSR

---

## 9. Discussion

* Portability: Strategies work on any RV32IM-class core without FPU.
* Reproducibility: Fixed dataset ensures fair comparison.
* Limitations: 
    * Spike is an instruction-level proxy; does not capture cache, branch prediction, or pipeline stalls.
    * The proposed method is primarily effective for small input range. Extending to a wider input range would require a larger lookup table, which may increase memory usage.
* Future Work:
  * Extending range reduction schemes.
  * Evaluating higher-degree approximations.
  * Explore alternative softmax algorithms, such as log-sum-exp methods that use logarithms to replace divisions.



---

## 10. Conclusion

By analyzing standard exponential implementations and proposing approximation strategies tailored for RV32IM-class processors, we demonstrate that significant cycle-count reductions are achievable purely in software. Taylor series approximation with Horner's method and LUT decomposition enable practical AI workloads on minimal embedded cores without FPU, making softmax and similar operations viable even in constrained environments.

---

## 11. References

* [MobileBERT](https://arxiv.org/abs/2004.02984)
* [RISC-V Specifications](https://lf-riscv.atlassian.net/wiki/spaces/HOME/pages/16154769/RISC-V+Technical+Specifications)
* [GNU C Library](https://codebrowser.dev/glibc/glibc/sysdeps/ieee754/flt-32/e_expf.c.html) — Source code of expf(), IEEE-754 single-precision implementation
* [Cephes Math Library](https://www.netlib.org/cephes/)
* [RISCV toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)
* [Spike RISC-V ISA Simulator](https://github.com/riscv-software-src/riscv-isa-sim)
* [riscv-gnu-toolchain install guide](https://hackmd.io/Ouj3SnvZTQ-1iiaWfvpPMQ)
