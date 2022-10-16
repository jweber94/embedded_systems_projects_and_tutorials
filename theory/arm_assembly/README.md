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

### Comparisons in arm assembly
+ Comparisons (also known as condition with a branch associated with it) in arm assembly are the underlying principal of makeing `if`-statements in higher level languages!
+ In the background, there will be a `cmp` instruction, which subtracts the two given registers from each other!
    - `cmp r0, r1` does in the background calculate `subs r2, r0, r1`!
    - The meta-result (greater then, equals, less then, not equal then, ...) is stored in the cpsr register
+ We define what should be done by adding and defining labels for different outcomes of the `cmp` instruction!
    - The execution block is called a branch.
    - Common result triggers are:
        * `bgt greater`, where `greater` is the label, defined further down
            - In this case, the `greater` label is the branch! 
        * `blt less`
    - There is a `bal default`, which is the "be always executed" label, which will be invoked always after the cmp instruction (like the else case in higher level languages)
    - If there is no definition which label should be invoked, the assembly code will go on as if there were no cmp operation (except of the calculation time that was needed to evaluate it)
+ ***CAUTION***: If we do not use the `bal` label, the machine will going on to the next instruction within the file. This will end up, executing the defined modules further down, even if the `cmp` instruction has said that they should not be executed
    - This is the principal, how assembly code is executed!
    - We can avoid this by using the bla condition
    - ***CAUTION***: The `bla` label ***must*** always be at the end of the file to avoid executing anything that should not be executed
+ Keep in mind, that everything is invoked sequentially in assembly language! Even if a callback is invoked by the `cmp` operation, it will not go back to the place in the code where it left. It will continue executing the whole remaining program from the callback on!
    - See `./src/comparison.s` for details (and the youtube video at 1:18:00)

### Constants in arm assembly
+ Constants can be defined with the following syntax: 
    * `.equ name #0x00`
    
### Looping in ARM assembly
+ Loops can be constructed, using the before discussed condition and branching (aka `cmp` and branch modules that are invoked by `bal`, `blt`, ...) functions of arm assembly in a clever way. 
+ Important: 
    * *A loop is basically a logical condition with a _go to_ (aka branch) at the end that goes back to the start of the loop or that goes to the nexr line after the loop code*
    * ***That means, every loop could be broken down as a condition and a branch in a sequential program!***
+ To stop a loop, most of the time, a termination character is used. For example in C there is the _null termination_ to indicate that an array is finished!
    * The null termination is written as `0x00`
+ Example in `./src/loop.s`

### Conditional intructions
+ Are like if-else statements in arm assembly and syntactically like `cmp` with its associated program branches but in a more condensed form.
    * The code will be more alltogether
+ We can extend arithmetic instructions with logical comparison instructions to make this happen.
    * Example:
        ```
        cmp r0, r1
        addlt r2, #2
        ```
    * Other examples are:
        - `subgt`: subtract if cmp was *g*reater *t*hen
        - `movge`: move registers if cmp was *g*reater or *e*qual
+ See `./src/conditional_instructions.s` for more details

### Functions in arm assembly
+ We can use labels to define blocks of execution, but then we need to define the next label to go on or start the calling function over again to go back to the line in the code where we called the label.
    - To avoid this, we can use `bl`, which stands for _branch linked_
+ `bl <label>`:
    - This will cause the branch to execute and store the memory address of the ***next*** commmand of the calling text block/label in the `lr` register of the arm processor --> By this register, the processor knows where to go back after finishing the label/function
    - In order to let this work properly, we need to use a `bx lr` (which stands for branch back to the address that is stored in `lr`) at the end of the program
    - See `./src/functions_1.s` for a basic example. Tipp: try it out in the arm v7 emulator and take a careful look at the `lr` register!

### Using the stack in functions
+ This is the underlying principal if you call a function from a higher level language when you call a function: We execute the function in its own function execution context!
    - The parameters for a function are the registers that are set before calling the block of execution of the function block/label
    - To restore the register states of the calling context, the register values are stored temporarily on the stack!
+ Using the stack is only necessary, if we use must use registers that are also used by the calling execution block
    - e.g. if we need to do a syscall within the function or the amound of free registers are not enough
+ The stack is a FIFO queue from the point of view of the stack pointer (which is the pointer that points to the ***start infex*** of the stack)
+ Implementation is done by `push` and `pop` opcode!
    - See `./src/function_with_stack.s`
        * Be careful: if we do not do the `b end` (`b` for branch) to the end of the file, the function will get called again, since assembly is interpreted stricly sequentially, like it was said before

### Loading big numbers to registers could cause problems - workaround
+ ***CAUTION***: In arm v7, we can not directly write an immediate (aka a number) that is greater then 255 (=8 bits) to a register. This is caused by historical reasons.
    - What you can do instead is to declare a constant and then load the value of the address where the constant is stored to the targeted register.
    - Example:
    ```
    .equ large_num, #257
    ldr r0, =large_num
    ```
    , whereas 
    ```
    mov r0, #257
    ```
    does not work.
    - If we use a value that is greater then 256 within a `mov` instruction, the compiler will moan!

### Interaction with hardware
+ In the [arm emulator](https://cpulator.01xz.net/?sys=arm-de1soc) are hardware examples there to test.
    - For example, we can interact with the LEDs and the switches on the right side besides the code
+ To interact with the hardware, we need to read out or write to the memory addresses of the hardware on the processor. 
    - Since the hardware addresses are commonly pretty large (and always bigger then 257 in HEX), we first need to load the address to a register and then retrive the value behind the address from the loaded memory address.
+ Example for doing this by reading the state of a switch, see: `./src/hardware_interaction_read.s`
+ ***Switches*** in depth:
    - Basically, how a hardware signal needs to be interpreted as numbers in its register is documented on the processors manual.
    - In case of switches, the logic is the following:
        * The switch information address is `0xff000040`
        * The default endianness of arm v7 is little endian. Every switch is associated with one bit, starting with switch 0 as the least significant bit.
        * Depending on the pattern of the on and off taken switches, we will receive a number from the register which corresponds to the binary pattern interpreted by the endianness of the processor.
        * Example: 
            + Switches on: 0, 2
            + Binary pattern (little endian): 10100000 00000000 00000000 00000000
+ Writing to a hardware memory address at the example of the LEDs
    + Writing to the memory address functions quiet like reading from the switches: We need to write the binary pattern in a little endian manner the the hardware associated memory address
    + See `./src/hardware_interaction_read_write.s` for an example

### Datatypes in arm assembly
+ Besides the common `.word` datatype, which is used to store data in 32 bit (depends on the system) HEX values, there are two other datatypes, namely:
    * `.ascii`: To store plain text - not null terminated ~ this can cause iteration errors, etc on many systems. You can use this for some special cases or if you have to save just a single character
    * `.asciz`: Null terminalted plain text (there is an alias in arm assembly for asciz that is called `.string`)

## Arm cross compiling and development
+ Arm emulation under linux:
    - Emulation is used to:
        * Not need to use the device directly (and break it on purpose)
        * Can be automated with a CI pipeline
+ As an example: ***Emulation of raspbian - rasperry pi arm linux OS***
    - Steps:
        * Download the `.img` image: you can download the `.zip` and extract it from [here](https://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2017-04-10/) 
        * Download a kernel [here](https://github.com/dhruvvyas90/qemu-rpi-kernel/blob/master/kernel-qemu-4.4.34-jessie) 
        * Use `qemu-system` to simulate a complete operating system (instead of qemu-user which simulates the processor for the executable)
            * You can define, what hardware should be used to emulate a system. This will be defined by the command line argument. The main arguments to set system configurations for the emulation are:
                - `-cpu arm1176`: What CPU should be emulated
                - `-m`: The amound of ram that should be virtually available to the emulated system
                - `-M versatilepb`: Emulated machine type
                - `-serial stdio`: We want to work with the standard input output with our system
                - `--append "root=/dev/sda2 rootfstype=ext4 rw"`: Setup the filesystem that should be used by the emulated system (ext, btr, squashfs, ...)
            * Besides this, we can set the kernel as well as an image of the operating system (`.img` file, that contains everything about the operating system besides the kernel)
                - `-kernel /path/to/kernel/binary` to set the kernel
                - `-hda /path/to/os/image.img`: hda is an option to setup the hard drive as a(n image-) file
            * Furthermore, we can forward ports from our emulated machine to our host machine, using the `-nic` command
                * This is useful, because sometimes the UI of qemu-system is not well forwarded, so we mostly interact with the emulated machine via ssh, telnet or something like this!
                * Forwarding the ssh-port (port 22): `-nic user,hostfwd=tcp::5022-:22`
        + Start the system emulation environment with:
            - `qemu-system-arm -kernel ~/path/to/downloaded/kernel/kernel-qemu-4.4.34-jessie -cpu arm1176 -m 256 -M versatilepb -serial stdio -append "root=/dev/sda2 rootfstype=ext4 rw" -hda ~/path/to/unzipped/files/2017-04-10-raspbian-jessie-lite.img -nic user,hostfwd=tcp::5022-:22 -no-reboot`
            - When the emulated system has started, we can login and enable ssh by opening up a terminal and then start the ssh systemd service by typing `sudo systemctl start ssh.service`
            - After this is done, we can login to the emulated machine via ssh with the command on the host: `ssh -Y pi@127.0.0.1 -p 5022`
        + The most internet connections will be forwareded by the qemu emulator to the host as a gateway
+ Important hints: 
    - ***If you develop your own linux distributions for a targeted device, you can test your linux images and kernels in a qemu environment beforehand to avoid damaging the physical system/chip!***
        * This can be done, see: https://unix.stackexchange.com/questions/632031/forward-network-packets-from-host-machine-to-qemu-image 
### Systemcalls in arm assembly
+ Systemcalls:
    * A syscall is a request to the linux kernel to do something, that we could not do in user mode
    * Systemcalls enable a standardized interface to kernel functionallities.
        - The main special thing about the kernel is, that it has read and *write* permissions to _ALL_ memoryaddresses! --> You can damage your system very badly, if you accidently overwrite an important memory address on your system. To avoid doing this, we are program applications in a lesser privileged "user-mode". Syscalls are an interface to have a defined behaviour while reading or writing those critical memory locations
+ How systemcalls can be invoked on a register level can be looked up in [this](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI) table!
    - The nice thing about developing with linux is thhat the kernel abi does not change very often!
+ Write systemcall in arm assembly:
    - We need to write the targeted data and the file descriptor where we want to write to into the registers that can be looked up in the aforementioned table
    - After doing this, we need to call the software interrupt 0 (`swi 0`) to tell the processor that we want to invoke a systemcall with the currently set registers
        * _This will cause a function invokation (like we already discussed in the "functions" section before) by jumping to a predefined memory address, where the systemcalls are stored and choosen!_
        * You can imagine that the software interrupt is the call to the operating system to take over the processing
+ Specials about linux arm systemcalls:
    - The register `r7` has the role of defining what systemcall should be invoked!
        * The first thing that will happen after the `swi 0` was called is that the OS checks what number is stored at `r7`
+ After a syscall is finished, the program executes further after the `swi` instruction
+ See `./src/hello_world_real_arm.s` for an example of the systemcall `write`!
+ ***Further reading:***
    - File descriptors:
        * A file descriptor is a unique integer that is defined _for a process_ on the linux OS (Every process has its own file descriptor table associated with it!). With it, we can identify the file. It works as a handle to the corresponding file!
        * Since "everything is a file in linux", we can address the stdin, stdout and stderr with the default file descriptors 0 (in), 1 (out) and 2 (err).

### Terminate a real arm assembly program with the `exit` system call
+ Since a user space program has no permissions to end processes, we need to do this via the `exit` system call!
+ In order to do this, we need to:
```
mov r7, #1
swi 0
```
  to call the process termination system call.
+ If you do not end your program in this way, the execution stops with a segfault after the last valid instruction was executed by the CPU

## Compilation on the target system (or within the emulated target system via qemu-system) - NO crosscompiler
+ To create the binary file from the assembly file, we use the linux terminal program `$ as` (GNU assembler)
    - Syntax: `$ as hello_world.s -o hello_world.o`
    - This will generate the binary file, called "object file" or "object code"!
    - This object file is ***not*** runnable!
+ After we generated the object code, we need to link the systems dynamically and statically standard libraries by using `ld` (GNU linker)
    - Syntax: `$ ld hello_world.o -o hello_world`
    - This will create the executable binary, named hello_world

## Debugging arm assembly on the linux host (or linux host emulation)
### GDB: GNU debugger
+ To run a program with gdb and stop at a breakpoint, we need to start the program with a `$ gdb hello_world` in front of it
    - This will open up gdb, but will ***not*** run the program!
    - To run the program, you need to type `run` into the gdb console!
+ You might want to stop the program at the entrypoint. Therefore, you need to set a breakpoint on the address, where the `_start` label is defined.
    - This can be done by: `$ break _start`
    - After this, we can run the program by typing `run` to the gdb console
        * The program will halt at the instruction address, where the label `_start` is defined
+ To look at the instructions of the compiled code (with the real addresses of the program), we can type `(gdb) layout asm`
+ To look at the registers and their current values, we can either use another layout, or we can request the register info via the gdb terminal interface:
    - Terminal interface: `(gdb) info register r0` or `(gdb) info register` to show all registers
    - Layout: `(gdb) layout regs`
+ Navigating the layouts:
    - `ctrl` + `x` and then `o` (like tmux `ctrl` + `b` and then ...)
    - You can scroll the layouts by using the arrow keys
+ Step to next instruction: `(gdb) stepi`
+ Inspect memory addresses (addresses, not registers!): 
    - `(gdb) x/10x $r1`: Show in HEX
    - `(gdb) x/10d $r1`: Show in decimal
    - `(gdb) x/10c $r1`: Show in char/string
    - Here is $r1 the resolved value of the register `r1`, interpreted as an address (idealy, there is an address stored within)
