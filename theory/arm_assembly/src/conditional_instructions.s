.global _start
_start:
	mov r0, #0x1
	mov r1, #0x2
	
	cmp r1, r0
	addgt r3, r1, r0
	
	cmp r1, r3
	moveq r4, #0x6
	cmp r3, r3
	moveq r4, #0x6
	
	mov r5, #0x7
	