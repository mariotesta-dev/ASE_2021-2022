				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  find_and_max

find_and_max
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = MAT
				;r1 = x1
				;r2 = y1
				;r3 = x2
				;ldr r4, |r12|... ldr r5, |r12, #4|, ecc...
				
				ldr r4, [r12]			;y2
				ldr r5, [r12, #4]		;M
				ldr r6, [r12, #8]		;N
				
				;X1, Y1
				;per riga moltiplico M*X1*4
				mov r7, #4
				mul r8, r6, r1				;N*X1
				mul r8, r8, r7				; *4 byte
				add r8, r0, r8				; mi sposto nella posizione in r0
				;per colonna sommo Y1*4
				mul r9, r2, r7				;Y1*4
				add r8, r8, r9				;sommo dove mi trovo nella posizione r0 + offset dato da Y1
				ldr r10, [r8]
				
				;X2, Y2
				;per riga moltiplico M*X2*4
				mov r7, #4
				mul r8, r6, r3				;N*X2
				mul r8, r8, r7				; *4 byte
				add r8, r0, r8				; mi sposto nella posizione in r0
				;per colonna moltiplico Y2*4
				mul r9, r4, r7				;Y2*4
				add r8, r8, r9				;sommo dove mi trovo nella posizione r0 + offset dato da Y1
				ldr r11, [r8]
				
				cmp r10, r11
				movge r0, r10
				movlt r0, r11
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;