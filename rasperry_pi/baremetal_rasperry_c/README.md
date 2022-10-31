# Bare Metal Register Programming with the Rasperry Pi

## Linker File
+ The `SECTION` part of the linker file defines the arrangement of the executable code within the created executable
    - In order to place a specific part of the compiled code by its symboles to the top of the executable, we need to define a section for it
    - This is done by `.section ".text.boot"` within the arm assembly code and is placed at the top of the linkerfile

## ARM Assembly code
+ The arm assembly code is just glue code in order to go (in ARM terms "branch") to the starting adress, where the `main` symbol is stored
    - If we don't do this, we will end up with an arbitary symbol at the top of the executable binary, since we do not link the libc (because we do bare metal programming and have no access to a linux library if we do not use linux at all)
    - To not link accidentaly the libc, we need to use the `-nostdlib` flag on the gcc cross compiler!

## Reference:
+ https://www.youtube.com/watch?v=mshVdGlGwBs&t=9s
+ https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
+ https://github.com/lowlevellearning/raspberry-pi-baremetal-c