	
				AREA 	asm_functions, CODE, READONLY
				EXPORT  search_in_pool
					
array	DCD 0x008260F3, 0x006370A3, 0x00942053, 0x00727043, 0x00921013
N		DCD 5
	
search_in_pool  
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr} 	;ro = VAR
				
				LDR R4, =N	;load N = 5
				LDR R4, [R4]
				LDR R6, =array ;load literal pool of 5 elements
				MOV R5, #0	 ;0 value
				MOV R8, #0   ;array index
				
loop			
				LDR R7, [R6] ;get value from array
				CMP R7, R0   ;cmp with VAR
				MOVEQ R0, R8 ;if they're equale -> var has been found, setup value for R0 to return the index
				BEQ fine	 ;end the program
				;else
				ADD R8,R8,#1 ;increment index
				ADD R6, R6, #4 ;increment pointer to next element of the array
				ADD R4, R4, #-1 ;decrease loop counter
				CMP R4, R0		;if !=0 keep looping, else loop ended
				BNE loop
				
				MOV R0, #-1 ;loop ended -> var hasn't been found, setup value for R0 to return -1
fine	
			
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;