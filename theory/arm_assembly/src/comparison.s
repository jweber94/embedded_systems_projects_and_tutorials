.global _start

_start:
    mov r0, #3
    mov r1, #1

    cmp r0, r1 ; which calculates r0 - r1 = 3 - 1 = 2

    bgt greater_callback
    blt less_callback
    bal default ; jump to the end of the file
    
    mov r3, #3 ; will not be invoked since the bla statement jumps to the bottom of the file and assembly is always interpreted sequentially

greater_callback:
    mov r2, #1 ; after this is invoked, less_callback and default will be invoked as well, since assembly code is executed sequentially 

less_callback:
    mov r2, #2

default:
    mov r2, #42