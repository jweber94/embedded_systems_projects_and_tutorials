# Introduction to assambly programming with ARM and the rasperry pi

## Needed software
+ To cross-compile arm assembly on a x86 processor, we need/could use the gcc cross compiling build chain. Therefore, we need to install the following:
    + `sudo apt install gcc-5-arm-linux-gnueabihf-base`
    + You might need to add the following apt-remote to the `/etc/apt/sources.list`: 
        - `deb http://us.archive.ubuntu.com/ubuntu/ precise universe`
+ To test-run a cross compiled executable, we commonly use qemu (Linux CPU emulator). In order to install it, we need to run:
    + `sudo apt install qemu-user`

## Compiling process
+ Create an object file (= Binary machine code without linking):
    - `$ arm-linux-gnuabi-as hello_world.asm -o hello_world.o`
    - We can inspect the machine code meta information with `$ file hello_world.o`
+ In order to get an executable, we need to link the machine code with the common system libraries via gcc:
    - `$ arm-linux-gnueabi-gcc hello_world.o -o hello_world.elf -nostdlib`
    - We can inspect this elf-file also with `file` or with a HEX-viewer like `xxd`
        * e.g.: `xxd hello_world.elf`

## Test run on a non arm build environment
+ We can use ***qemu-user*** (Linux CPU emulator) to execute a cross compiled binary blob with a x86 intel processor for a test run!
+ Example:
    - `$ qemu-arm hello_world.elf`

## Convenience Makefile:
+ To build the hello world program in assembly (assuming that all previously named software is installed), you can run 
    - `$ make build`
+ And to run the program with qemu, you can run
    - `$ make run`

## Main keywords and takeaways
+ `.global`:
    - Is needed to show off function names to the linker or request them by the linker, if not defined within the individual assembly file
    - Everything that is defined `.global` is written to the linker table within the object file
+ `_start:`
    - Is the default entry point of an assembly program
+ `.section`:
    - Defines continious blobs of memory where the code or data is stored
+ Assembly code is interpreted sequentially on register level. That means a register will not be changed until it is told to do it. During execution the CPU can access all registers (if wanted) with their latest data/entrys within them.

### Reference:
+ https://www.youtube.com/watch?v=FV6P5eRmMh8
+ https://teachcomputerscience.com/assembly-language/
+ https://www.electropages.com/blog/2021/03/arm-vs-risc-v
+ https://www.geeksforgeeks.org/difference-between-hardware-interrupt-and-software-interrupt/
+ https://www.youtube.com/watch?v=TFt8iMskKE4
+ https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md
+ http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/swi.pdf --> Example of reading and writing with arm assembly
+ https://www.ti.com/lit/ug/spnu118y/spnu118y.pdf?ts=1665207637098 --> To lookup keywords in arm assembly
+ https://stackoverflow.com/questions/17898989/what-is-global-start-in-assembly-language --> Important definitions in assembly (_start, .global)
+ https://azeria-labs.com/memory-instructions-load-and-store-part-4/ --> Extremly good intro to assembly programming
