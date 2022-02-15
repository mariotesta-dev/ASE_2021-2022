				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  get_and_sort

get_and_sort
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = VETT
				;r1 = VAL
				;r2 = i
				
				CMP r2, #0			;se è il primo valore che sto inserendo
				STRBEQ r1, [r0] 	;inseriscilo in prima posizione su vett
				BEQ fine
				;else           ---> almeno i>0, quindi ho almeno 2 elementi in vett
loop			
				ADD r3,r0,r2		;posizione dove inserire val
				ADD r4,r3,#-1		;posizione elemento precedente
				LDRB r5, [r4]		;prendo elemento precedente
				CMP r1, r5
				STRBLO r5, [r3] 	;se val è minore, salva r5 nella posizione indicata dall'indice
				STRBLO r1, [r4]
				STRBHI r1, [r3]		;se val è maggiore, salva val nella posizione indicata dall'indice
				BHI fine
				
				ADD r2,r2, #-1		;decremento indice
				CMP R2, #0
				BNE loop			;loop se i != 0
			
fine				
				;setup value for R0 to return VAL
				MOV R0, R1
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;