# Documentation: Exponential Function Optimization on RV32IM

This folder contains all documentation for the project on software-level optimization of the exponential function for RISC-V RV32IM-class processors without FPU support.

## Overview
The optimization work targets the exponential function as a critical bottleneck in AI workloads such as softmax. Techniques explored include:
- Taylor series approximation with tunable degree
- Horner's method for reduced complexity
- Lookup table (LUT) based range reduction

These methods aim to significantly reduce cycle counts on minimal RISC-V cores.

## Files
- **full-report.md** — Detailed technical report with background, analysis of existing libraries (glibc, Cephes), proposed optimization strategies, experiment results, and discussion.
- **slides.pdf** — Concise, illustrated summary of the project, designed for easy review.

## Recommended Reading Order
1. `slides.pdf` for a quick high-level overview
2. `full-report.md` for complete technical details

## License
MIT License
