				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  count_negative_and_odd

count_negative_and_odd
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = VETT
				;r1 = N
				;bisogna contare i valori negativi e dispari
				
				mov r2, #4
				mul r1,r1,r2		;16bits integers, so i have to move using 2bytes
				add r2, r0, r1 		;find VETT ending address
				mov r5, #0			;count
loop
				ldrsh r4, [r0]		;get current value from vett, i'm using a signed half since, but an implementation using a 32bit + using AND with 0xf0000000 is possible to declare if a number is negative since a value THAT big is unrealistic for this use case
				cmp r4, #0
				bgt next_element
				andlt r4, r4, #0x00000001	;if r4 is lower than 0, so it's negative -> check if it's odd by doing AND with 0x01
				cmp r4,#1
				addeq r5,r5,#1			;if the AND returns 1 -> then add 1 to counter r5

next_element				
				add r0,r0,#4		;int should be saved as 16bits value, so i add 2 bytes to visit the next element
				cmp r0,r2
				bne	loop			;if current element address is lower, so different, than the ending address -> loop
				
				;setup value for R0 to return r5 which is the counter of number which are negative and odd at the same time
				MOV R0, R5
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;