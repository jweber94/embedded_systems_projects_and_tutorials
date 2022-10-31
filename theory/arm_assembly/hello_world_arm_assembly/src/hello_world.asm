# Hello World in ARM assambly language

.global _start
.section .text

_start:
    # write syscall
    mov r7, #0x4        
    # use file descriptor 1 (stdout) - in HEX
    mov r0, #0x1        
    # load register with the address where message is stored
    ldr r1, =message    
    # length in decimal
    mov r2, #13         
    # call the software interrupt
    swi 0               

    # exit the program with an exit syscall
    mov r7, #0x1
    mov r0, #0x0
    swi 0

.section .data
    message:
    .ascii "Hello World\n"
