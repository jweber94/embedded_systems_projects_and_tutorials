.global _start

.equ end_list #aaaaaaaa ; constant that indicates uninitialized memory

# The loop is used to sum up all elements in the list

_start:
    ldr r0, =list ; get address of the list to r0
    ldr r3, =end_list
    ldr r1, [r0] ; assess the value and store it from r0 to r1
    add r2, r2, r1

loop:
    ldr r1, [r0,#4]! ; increment 4 bits
    cmp r1, r3 ; check termination condition
    beq exit
    add r2, r2, r1
    bal loop ; go back to the loop

exit:
    ldr r4, #0x666

.data
    list:
        .word 1,2,3,4,5,6,7 ; All elements are one word (aka 32 bit)