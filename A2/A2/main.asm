
; Definiciones 
.DEF temp     = r16			 ; Registro de propósito general
.DEF counter  = r17			 ; Registro para contadores de bucle
.DEF val      = r18			 ; Registro para almacenar el valor actual de la secuencia
.DEF bcdin    = r20			 ; Registro para el valor de entrada a la subrutina BCD
.DEF decenas  = r21			 ; Registro para almacenar el dígito de las decenas

;Constantes 
.EQU n = 99                 ; N
.EQU HEXMEM = 0x0120		; Dirección HEX
.EQU BCDMEM = 0x0200		; Dirección BCD

.CSEG
.ORG 0x0000
   
main:
    ;Inicializar el Stack Pointer 
    LDI temp, LOW(RAMEND)
    OUT SPL, temp
    LDI temp, HIGH(RAMEND)
    OUT SPH, temp
    LDI val, n					; Cargar el valor inicial N
    LDI ZL, LOW(HEXMEM)			; Apuntar a la dirección 0x0120
    LDI ZH, HIGH(HEXMEM)

mempointer:
    ST Z+, val						; Guardar el valor actual en SRAM e incrementar Z
    DEC val							
    BRPL mempointer					
                                    
    LDI counter, n + 1          ; Inicializar el contador N+1 números 
    LDI XL, LOW(HEXMEM)			; Apuntar a la zona de datos HEX
    LDI XH, HIGH(HEXMEM)
    LDI YL, LOW(BCDMEM)			; Apuntar a la zona de datos BCD
    LDI YH, HIGH(BCDMEM)

convercion:
    LD bcdin, X+					; Cargar un número de la zona HEX
    RCALL bintobcd					
    ST Y+, bcdin					; Guardar BCD 
    DEC counter						; Decrementar el contador de bucle
    BRNE convercion					

end:
    RJMP end
	

bintobcd:
    CLR decenas						
restabin:
    CPI bcdin, 10					; Comparar el número con 10
    BRLO juntar						
    SUBI bcdin, 10					
    INC decenas						; Incrementar el contador 
    RJMP restabin					

juntar:
    
    MOV temp, decenas				; temporal
    SWAP temp						 
    OR bcdin, temp					; Combinar con el dígito de las unidades
                                    
    RET                             
