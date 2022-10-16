.global _start

_start:
    # setup the registers for the write system call
    mov r0, #1
    ldr r1, =message
    ldr r2, =len
    mov r7, #4
    # invoke the write system call
    swi 0

    # terminate the program with the exit systemcall
    mov r7, #1
    swi 0

.data
message:
    # declaring the text
    .asciz "Hello World!\n"
# declare the length of the message variables - the syntax is looking for the nullterminator
len = .-message
