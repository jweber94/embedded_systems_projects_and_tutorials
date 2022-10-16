.global _start

.equ SWITCHES, 0xff200040
.equ LEDS, 0xff2000000

_start:
    # reading out the switches
    ldr r0, =SWITCHES
    ldr r1, [r0]

    # writing to the leds
    ldr r0, =LEDS
    str r1, [r0]
