
build:
	riscv32-unknown-elf-gcc ../src/*.c ../src/*.S -I../src/*.h -lm -o exp-opt.exe

build_linux:
	gcc ../src/main_linux.c ../src/exp.c ../src/data.S -lm -o exp-opt.exe

run:
	spike --isa=rv32im_zicsr_zicntr pk exp-opt.exe
clean:
	rm -f exp-opt.exe
all: build run clean
