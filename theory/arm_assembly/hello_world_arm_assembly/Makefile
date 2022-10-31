# Makefile for building the project

.phony:
	build

clean:
	rm -rf ./build

build: clean
	mkdir build
	arm-linux-gnueabi-as ./src/hello_world.asm -o ./build/hello_world.o
	arm-linux-gnueabi-gcc-10 ./build/hello_world.o -o ./build/hello_world.elf -nostdlib

run:
	qemu-arm ./build/hello_world.elf
