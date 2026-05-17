; O QUE FAZ:
; Testa o montador abrangendo todos os comandos
; da arquitetura hipotética, incluindo EQU, IF
; e CONST com decimal, negativo e hexadecimal.

; ==========================================
; TESTE EQU
; ==========================================

VALOR_TESTE: EQU 1
DIVISOR:     EQU 2

SECTION DATA

	N1:        SPACE
	N2:        SPACE

	SOMA:      SPACE
	DIF:       SPACE
	PROD:      SPACE
	METADE:    SPACE
	COPIA:     SPACE

	; ==========================================
	; TESTE CONST
	; ==========================================

	DOIS:      CONST DIVISOR     ; EQU + decimal positivo
	ZERO:      CONST 0           ; decimal zero
	UM:        CONST 1           ; decimal positivo
	MENOSUM:   CONST -1          ; decimal negativo

	; ==========================================
	; TESTES HEXADECIMAIS
	; ==========================================

	HEXA:      CONST 0x12        ; 18 decimal
	HEXFF:     CONST 0XFF        ; 255 decimal
	HEXAB:     CONST 0xAB        ; 171 decimal
	HEXZERO:   CONST 0x0         ; 0 decimal

	; ==========================================
	; TESTES DECIMAIS EXTRAS
	; ==========================================

	POS:       CONST 25
	NEG:       CONST -13

SECTION TEXT

    INPUT N1
    INPUT N2

    ; ==========================================
    ; TESTE IF
    ; ==========================================
    ; Como VALOR_TESTE = 1,
    ; a próxima linha deve permanecer
    ; após o pré-processamento

    IF VALOR_TESTE
    OUTPUT UM

    ; ==========================================
    ; TESTES CONST
    ; ==========================================

    OUTPUT POS
    OUTPUT NEG

    OUTPUT HEXA
    OUTPUT HEXFF
    OUTPUT HEXAB
    OUTPUT HEXZERO

    ; ==========================================
    ; TESTE LOAD / ADD / STORE
    ; ==========================================

    LOAD N1
    ADD N2
    STORE SOMA 

    ; ==========================================
    ; TESTE SUB
    ; ==========================================

    LOAD N1
    SUB N2
    STORE DIF

    ; ==========================================
    ; TESTE MUL
    ; ==========================================

    LOAD N1
    MUL N2
    STORE PROD

    ; ==========================================
    ; TESTE DIV
    ; ==========================================

    LOAD N1
    DIV DOIS
    STORE METADE

    ; ==========================================
    ; TESTE COPY
    ; ==========================================

    COPY SOMA  ,      COPIA

    ; ==========================================
    ; TESTE OUTPUT
    ; ==========================================

    OUTPUT SOMA
    OUTPUT DIF
    OUTPUT PROD
    OUTPUT METADE
    OUTPUT COPIA

    ; ==========================================
    ; TESTE JMPZ
    ; ==========================================

    LOAD DIF
    JMPZ IGUAIS

    ; ==========================================
    ; TESTE JMPN
    ; ==========================================

    LOAD DIF
    JMPN NEGATIVO

    ; ==========================================
    ; TESTE JMPP
    ; ==========================================

    LOAD DIF
    JMPP POSITIVO

IGUAIS:
    OUTPUT ZERO
    JMP FIM

NEGATIVO:
    OUTPUT MENOSUM
    JMP FIM

POSITIVO:
    OUTPUT UM
    JMP FIM

FIM:
    STOP