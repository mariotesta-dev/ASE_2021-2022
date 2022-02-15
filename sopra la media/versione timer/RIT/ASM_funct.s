				
				AREA 	asm_functions, CODE, READONLY
				EXPORT  sopra_la_media

sopra_la_media
				MOV R12, sp
				STMFD sp!, {r4-r8, r10-r11, lr}
				
				;ro = VETT
				;r1 = N
				
				;1. trovo valore medio
				add r3, r0, r1				;indirizzo fine di VETT
				add r2, r0, #0				;indirizzo di partenza di r0, lo salvo perchè mi serve due volte
				mov r4, #0					;r4 conterrà la media
loop_media
				ldrb r5, [r2]					;carico byte in r5
				add r4, r4, r5				;sommo tutti i valori uno ad uno
				
				add r2, r2,#1
				cmp r2, r3
				bne loop_media				;finchè non ho raggiunto la fine, branch a loop_media
				
				udiv r4, r4, r1				;media = sommavalori/#valori
				
				;2. trovo i valori > media
				mov r6,#0					;inizializzo counter valori > media
loop_search
				ldrb r5, [r0]					;carico byte in r5
				cmp r5,r4
				addhi r6,r6,#1				;se valore caricato in r5 > media, incrementa il counter
				;else vai avanti nella ricerca
				add r0, r0,#1
				cmp r0, r3
				bne loop_search				;finchè non ho raggiunto la fine, branch a loop_searcb
				
				;setup value for R0 to return R6, cioè il counter di valori maggiori della media
				MOV R0, R6
				
				;restore volatile registers
				LDMFD sp!, {r4-r8, r10-r11, pc}
				
				END;