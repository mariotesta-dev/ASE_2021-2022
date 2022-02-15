				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  divisible

divisible
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = e1
				;r1 = e2
				
				;divide e1/e2, if module==0 then the numbers are divisible and return 0, else return module
				UDIV R4, R0, R1
				MUL R4, R4, R1
				SUB R5, R0, R4
				
				;setup value for R0 to return
				MOV R0, R5
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;