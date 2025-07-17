RV_TOOLCHAIN = /home/yutung/RISCV-TOOLS/RV32IMZf/bin
RV_GCC = $(RV_TOOLCHAIN)/riscv32-unknown-elf-gcc
PK = $(RV_TOOLCHAIN)/../riscv32-unknown-elf/bin/pk
SPIKE = /home/yutung/RISCV-TOOLS/spike/bin/spike
ISA = rv32im_zicsr_zicntr
RV_OPT = -march=$(ISA) -mabi=ilp32

build_debug:
	$(RV_GCC) $(RV_OPT) -DDEBUG ../src/main.c ../src/algo.c ../src/data.S -I../src -lm -o exp-opt.exe

build:
	$(RV_GCC) $(RV_OPT) ../src/main.c ../src/algo.c ../src/data.S -I../src -lm -o exp-opt.exe

run:
	$(SPIKE) --isa=$(ISA) $(PK) exp-opt.exe

clean:
	rm -f exp-opt.exe

all: clean build run

