# Fast Softmax on RV32IM Without FPU

This project demonstrates a fast and lightweight software implementation of the **softmax function** on RISC-V RV32IM platforms **without hardware floating-point support (F extension)**.  
It is designed to improve inference performance in edge AI workloads, particularly for systems that rely on integer-only or minimal floating-point capabilities.

## Motivation

The softmax function is widely used in classification tasks and transformer-based models (e.g., MobileBERT).  
However, its exponential component (`exp(x)`) is computationally expensive on RV32IM systems due to lack of native FPU.  
This project investigates exponential approximations and LUT-based techniques to speed up softmax evaluation, while maintaining numerical accuracy.

## Results

- Tested on 512 random `float32` values in range `[0, 10)`
- ~58% reduction in cycle count on RV32IM (measured with `mcycle`)
- Max absolute error: **0.0003**
- Platform: RISC-V `spike` + `pk`, using custom-built RV32IMZf toolchain

## Project Structure

softmax-acceleration-rv32im/
    - docs/ # Full technical report (full-report.md) and slides
    - src/ # Core implementation and benchmark code (main.c, algo.c)
    - run/ # Makefile and executable run results
    - README.md # This file

## Build & Run

To compile and simulate:

```bash
cd run
make all
```

## Documentation
- Full report in docs/full-report.md
- Concept slide in docs/softmax_rv32im_concept_slides.pdf

## License
MIT License
