.global _start

_start:
    mov r0, #0x00000044 ; equals 68 in decimal
    lsl r0, #2 ; left shift 2 is the same as multiplying by 2*2=4 - 68*4 = 272 = 0x00000110      
    mov r1, r0 ; copy the value from register 0 to register 1 (make a deep copy)
    lsr r1, #2 ; goes back to the initial value 68 = 0x00000044

    # copy and shift at the same time
    mov r0, #0x00000033
    mov r1, r0, lsl #2 ; copy and left shift (aka *4) of the 0x00000033 = 51 , which equals 204 = 0x000000cc
