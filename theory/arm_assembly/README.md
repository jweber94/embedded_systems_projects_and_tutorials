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
+ Assembly file extension for linux/POSIX is `filename.s`

## Build and run process
+ Building:
    - `$ mkdir build`
    - `$ arm-linux-gnueabi-as ./src/indirect_addressing.s -o ./build/indirect_addressing.o`
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
    - `.global` writes the information of the labels that are written within the `.s` file to the liker tabel of the object file
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
    - Example: `./src/indirect_addressing.s`
+ Indirect addressing with a pre-increment with `ldr`:
    - Example: `ldr r2, [r0,#4]!` _or_ `ldr r2, [r0], #4` (which is syntactically identical)
    - This will write the same data to r2 from the stack as before _but_ it will also increment the address in register `r0` by 4!
    - You can interpret this as incrementing the read-pointer, where the `r0` register is the read pointer (`r0` could be replaced by any other general purpose register)
+ ***CAUTION - phänomänological explaination***: The `[]` operator loads the value behind the address of the given address/register
    * Example:
    ```
    mov r0, #0x00000044
    ldr r1, [r0] ; copies the 32 bit value at the address 0x00000044 to r1 
    ```
    * Copying values from another register to a targeted register has to be done with the `mov` operator:
    ```
    mov r0, #0x00000044
    mov r1, r0 ; copies the value 0x00000044 to r1
    ```

### Arithmetic operations
+ `mul`: Multiply
    - Syntax: `mul r2, r0, r1`, where `r2` is the destination and `r1` and `r0` are the _two_ sources for the arithmic operation
+ `add`: Addition
    - Syntax: `sub r2, r0, r1`, where `r2` is the destination and `r1` and `r0` are the _two_ sources for the arithmic operation
+ `sub`: Subtraction
    - Syntax: `sub r2, r0, r1`, where `r2` is the destination and `r1` and `r0` are the _two_ sources for the arithmic operation
+ *The problem of negative numbers in assembly*
    - Since all numbers are stored in 32 bit hex format and we can store positive as well as negative numbers, we can hardly know, if a number within a register is a large positive number or if it is a complementary negative number!
    - ***A small negative number has the same 32 bit pattern that a very large non-negative number has!***
    - To decide what it is, we can only determine what the inputted data to the arithmetic operation was
    - The CPU itself know that information by the time it is executing the calculation. This information can be stored in a dedicated register, the `cpsr` register
+ Arithmetic with flags:
    + `cpsr` register:
        - Can be used in order to determine if we use a complement number out of an arithmetic operation
    + In order to use this register, we need to add a `S` to the arithmetic operation. For example: `subs r2, r1, r0`
    + ***CAUTION:*** be aware that using the cpsr register needs an additional CPU execution. Therefore it is not as fast as the plain arithmetic operation, since it needs to write an additional register (the `cpsr` register)! ==> Only use the arithmetic operation with flags, if you are sure that negative numbers can occure! (Or if you do not know what the input values of the arithmetic operation are)
    + ***The indication of a negative number is an `8` on the left most HEX-bit (=4 byte)***
+ _carry_ flag on the `cpsr` register:
    - We will receive a carry flag on the cpsr register, if the result of an arithmetic operation could not fit into a 32 bit register!
    - We can catch it with the `S` flag (e.g. `adds`)
    - If we want to catch the carry, we need to use (for example in case of an addition) the `adc, dst, src1, src2` which writes the result of src1 + src2 and the carry to the register, so we can use the carry later on
+ See `./src/arithmetic_operations.s` for details

### Logical operators in arm assembly
+ All logical operators are used to perform *bitwise* (!) operations of 0 and 1 on the handed over registers and save the result to another register
+ `and`:
    * Example:
        ```
        mov r0, #0x1
        mov r1, #0x2
        and r3, r0, r1
        ```
    * `and` is mostly used to do binary masking defined bits - i.e. the and with the register itself with the binary mask `0x000000ff` allows only the last 8 bits (since HEX `f` is binary `1111`) in the register to exist further
        - Example: `and r0, r0, #0x000000ff`
+ `orr`:
    * Logical _or_
+ `eor`:
    * exclusive or
    * Gives the resulting bit within the register a 1 if eiter src1 _or_ src2 contains a 1, but returns 0 if both are 1 or both are 0
+ Negation in arm assembly
    * `mcn dst, r0`
    * In order to negate a logical operation, we need to do the evaluate the basic logical operation and then move-negate (`mvn`) the result to another register
+ See `./src/logical_operations.s` for details

### Logical shifts
+ ***LSL***: Logical shift to the left
    - If we shift a binary number one bit (for each setted bit) to the left, the resulting number is ***always the original number, multiplied by 2!***
    - The upcoming, free bits will be 0
    - This is the main purpose of a left-shift by 1
+ ***LSR***: Logical shift to the right
    - If we shift a binary number one bit (for each setted bit) to the right, the resulting number will be the calculation result of the ***original number, divided by 2***
    - The upcoming, free bits will be 0
+ The fact that a bit shift is equivalent to multipling or dividing by 2 holds true can be easily mathmatically derived if you keep in mind that the binary number system is based on 2 with a power of the place within the digit system. (german: Stellenwertsystem). 
+ Logical shifts are the only arm assembly operation that can be done with one CPU instruction!
    - See `./src/bit_shifting.s` for details

### Logical rotation
+ Mostly used by cryptocraphy and computer graphics
+ arm assembly only has rotation to the right
    - ***ROR*** - rotation to the right


