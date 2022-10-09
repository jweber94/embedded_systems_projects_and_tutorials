# Introduction to arm assembly programming
+ Reference: [This free code camp course](https://www.youtube.com/watch?v=gfmRrPjnEw4)
+ Testing environment of this course:
    - (CPUlator for arm v7)[https://cpulator.01xz.net/?sys=arm]

## Important to know before
+ Each HEX-value has 4 bits (= 1/2 byte)
+ _opcode_ in assembly terminology = operation instruction for the CPU
    - These are CPU instructions like `mov`, `swi`, ...
    - opcodes are *not* case senitive
+ ARM is little endian by default
    - We can change the endianess of the processor on demand

## Build and run process
+ Building:
    - `$ mkdir build`
    - `$ arm-linux-gnueabi-as ./src/indirect_addressing.asm -o ./build/indirect_addressing.o`
    - `$ arm-linux-gnueabi-gcc-10 ./build/indirect_addressing.o -o ./build/indirect_addressing.elf -nostdlib`
    - ***OR*** Experiment with it in (CPUlator for arm v7)[https://cpulator.01xz.net/?sys=arm]

## Arm Architectur
+ Register:
    - Are areas in memory that are located very close to the CPU
    - Can accessed (read/write) very fast
    - The capacity is very low for memory
    - On arm v7, each register contains 8 HEX-values (with 4 bits each) --> overall one register contains 4*8 = 32 bit 
        * arm v7 is a 32 bit processor (in contrast to the newer intel machines, which run a 64 bit processor)
        * ==> The number of bits of a processor describes how many bits are contained within one register of the processor.
    - In the context of low level programming, one register is the amount of data that we can store at a single time. It is also called a "***Word***"
        * The phrase "We are changing a word worth of data" means we are changing 32 bits
        * A ***word*** is the max size of a register
    - Some registers have special purposes! I.e. r7 is commonly used as a systemcall indicator
    - We want to use registers as much as possible to be efficient
+ `r0` to `r7`:
    - data registers
+ `sp` register:
    - This is the register where the stack pointer is stored (The *address* of the next free memory on the stack)
    - Stack:
        * Is the RAM memory of a computer
+ `lr` register:
    - Is only needed within functions/function calls
    - This is the address to what a function should return back to
+ `pc` register:
    - Address of the next instruction that the cpu should execute
    - Keep in mind: All instructions that the computer should execute is stored anywhere in memory
+ `cpsr` register:
    - Used to store information about the program/about the previous operations
    - commonly it contains flags (coded in HEX) how a result of an instruction should be interpreted.
        - i.e. if we need to interpret the previous calculation as a complement (if it is a negative number)
+ `spsr` register:
    - like cpsr

## Arm assembly syntax
+ Label:
    - Is a synonym to a function in higher level languages
    - If you tell your processor to go to a label, it starts to execute the code that is underneath the label.
    - i.e. `_start:` label as an entry point (-function)
+ `.global _start`
    - `.global` writes the information of the labels that are written within the `.asm` file to the liker tabel of the object file
+ `.data` portion of an assembly file:
    - Is used to write data to the stack
    - Syntax:
        ```
        .data
        my_data:
            .word 5
        my_string:
            .ascii "Hello World!"
        ```
    - `.word` is used for all data (in assembly there are no int, float, ...)
+ `ldr` instruction
    - Load data from the stack to the `dst` register

### Addressing types
+ Addressing types are the realization how to store and/or retrive data from and to registers
+ *Immediate addressing*:
    - A program defined value is stored to a register
    - Example:
        `mov R0, #5` 
+ *Direct addressing*:
    - Write the data from one register to another
    - Example: Write the value from r0 to r1
        `mov r1, r0` 
+ *`ldr` direct addressing*
    - Example: `ldr r0, =my_data`
    - This loads the *location* (address) of the data on the _stack_ into the register `r0`
+ *`ldr` indirect addressing*
    - Example: `ldr r1, [R0]`
    - This will copy the value that is behind the given address/register that stores an address to the targeted register
    - ***CAUTION:*** We can *not* indirect address the stack directly. We *must* load the memory location of the data on the stack to a register and then address its data from there in order to write it to a register
    - Example: `./src/indirect_addressing.asm`
+ Indirect addressing with a pre-increment with `ldr`:
    - Example: `ldr r2, [r0,#4]!` _or_ `ldr r2, [r0], #4` (which is syntactically identical)
    - This will write the same data to r2 from the stack as before _but_ it will also increment the address in register `r0` by 4!
    - You can interpret this as incrementing the read-pointer, where the `r0` register is the read pointer (`r0` could be replaced by any other general purpose register)