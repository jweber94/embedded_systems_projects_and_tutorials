
build:
	mkdir build
	arm-linux-gnueabi-as -o ./build/start.o ./src/start.s
	arm-linux-gnueabi-gcc-10 -c -o ./build/main.o ./src/main.c -I ./include
	arm-linux-gnueabi-gcc-10 -T linker.ld -o ./build/kernel7.elf ./build/start.o ./build/main.o -ffreestanding -O2 -nostdlib -lgcc
	arm-linux-gnueabi-objcopy ./build/kernel7.elf -O binary ./build/kernel7.img

clean:
	rm -rf ./build