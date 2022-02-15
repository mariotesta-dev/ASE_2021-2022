				AREA ram_pool, DATA, READWRITE
				EXPORT BEST_3
BEST_3 SPACE 3 
				LTORG
				
				AREA rom_pool, DATA, READONLY
				EXPORT DATA_IN
				EXPORT N
DATA_IN	DCB -10, 20, 30, 40, 50
N DCB 5
				LTORG

				AREA 	asm_functions, CODE, READONLY
				EXPORT  find_best_3

find_best_3
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = DATA_IN
				;r1 = N
				;r2 = BEST_3
				;strategy: insert first 3 elements, then for each element starting from data_in|3| -> find the lowest and swap it

				;insert first 3 elements
				ldrsb r3, [r0]
				strb r3, [r2]
				ldrsb r3, [r0,#1]
				strb r3, [r2,#1]
				ldrsb r3, [r0,#2]
				strb r3, [r2,#2]
				
				add r3, r0, r1 		;ending of data_in
				add r0,r0, #3  		;starting of data_in from 4th element if exist
				add r8,r2, #3
loop
				cmp r0,r3		
				bge fine			;if starting pointer reached end or it's over the ending pointer, then end the function
				
				;else
				ldrsb r4, [r0]		;get element from data_in
				mov r6, #0			;index
				mov r7, #-1			;flag, if this flag is != -1 --> there is an minimum element inside best_3, so swap it
loop_int
				ldrsb r5, [r2]		;get element from best_3
				cmp r4,r5			
				movgt r4,r5			;if r4 > r5, save the minimum value between the two
				movgt r7, r6		;flag != -1 --> minimum value found, set r7 with the index of the minimum value
				add r6,r6,#1		;increment index
				add r2,r2,#1		;increment pointer address of best_3
				cmp r2,r8
				bne loop_int		;if end has not been reached, keep looping in internal loop
				
				cmp r7, #-1
				ldrsbne r4, [r0]	;get data_in value again
				ldrne r2, =BEST_3		;get best_3 address agaian
				strbne r4, [r2,r7]	;store data_value at index of minimum value, if it has been found -> r7 != -1
				
				add r0,r0,#1		;increment data_in pointer
				cmp r0,r3
				bne loop
				
				;setup value for R0 to return
fine
				MOV R0, R1
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;