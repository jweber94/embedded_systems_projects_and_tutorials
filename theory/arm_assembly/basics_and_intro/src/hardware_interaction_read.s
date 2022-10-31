.global _start

.equ SWITCH_START_ADDR, 0xff200040 ; this is writing the address hardly to the binary code so it can be retieved on runtime to a register

_start:
    ldr r0, =SWITCH_START_ADDR ; load the address to the register
    ldr r1, [r0] ; read out the value from the memory address 0xff200040 of the processor, which is associated with the switch 0
    