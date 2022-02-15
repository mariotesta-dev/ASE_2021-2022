				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  count_bits_to_1

count_bits_to_1
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = VAR1
				
				MOV R4, #32  ;loop count
				MOV R5, #0	 ;0 value
				MOV R6, #0   ;counter of bits set to 1 = 0
				MOV R8, #0x00000001
loop
				AND R9, R0, R8
				CMP R9,R8
				ADDEQ R6, R6, #1
				
				MOV R0, R0, LSR #1
			
				ADD R4, R4, #-1
				CMP R4, R0
				BNE loop
				
				;setup value for R0 to return
				MOV R0, R6
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;