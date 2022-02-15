				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  duty_cycle

duty_cycle
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = valore_iniziale
				;r1 = tempo_totale
				;r2 = tempi_variazione array
				;r3 = numero_variazioni
				
				mov r5, #4
				mul r5, r3, r5		;numero di byte per calcolare indirizzo finale
				add r4, r2, r5		;indirizzo finale del vettore tempi_variazione
				cmp r0, #0			;se valore iniziale è 0
				addeq r2,r2,#4		;inizia a leggere dalla posizione 1, perchè è lì che si trova la prima durata del segnale a 1
				;else il valore iniziale è 1, quindi inizia a leggere dalla posizione 0
				mov r5, #0
loop
				cmp r2,r4			;sono alla fine del vettore? --> caso num_variazioni = 0
				bge fine			;se sono alla fine o dopo, cioè il puntatore supera la fine... esci
				;else... 
				ldr r6, [r2]		;prendo valore da tempi_variazioni
				add r5, r5, r6		;sommo la durata con segnale a 1
				
				add r2,r2,#8		;mi sposto 2 posizioni più avanti, per leggere la successiva durata del segnale a 1, se esiste
				
				cmp r2,r4			;compare puntatore tempi_variazione e posizione finale
				blt loop			;se non sono arrivato alla fine del vettore, jump a loop

fine				
				mov r7, #100
				mul r5,r5,r7		;durata a 1 * 100
				udiv r5, r5, r1		; --> durata a 1 * 100 / durata tot = duty cycle
				;setup value for R0 to return
				MOV R0, R5
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;