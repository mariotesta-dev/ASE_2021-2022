				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  sort_vector

sort_vector
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = VETT
				;r1 = N
				
				mov r1, r1, LSL #2		;r1*4
				add r2, r0, r1 ;indirizzo fine vett
				add r3, r0, #0 ;j
				add r4, r0, #0 ;i
				add r7, r2, #-4
loop_ext
				add r3, r0, #0
loop_int		
				ldr r5, [r3]		;load vett|j|
				ldr r6, [r3,#4]		;load vett|j+1|
				cmp r5, r6
				;se vett|j| > vett|j+1| --> può diventare desc mettendo lt al posto di gt
				strgt r5, [r3,#4]	;salva vett|j| nella posizione successiva
				strgt r6, [r3]		;salva vett|j+1| nella posizione precedente
			
				add r3,r3, #4
				cmp r3, r7
				bne loop_int
				
				add r4,r4,#4
				cmp r4, r2
				bne loop_ext
				
				;setup value for R0 to return
				MOV R0, R6
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;