; O QUE FAZ:
; Testa o montador abrangendo todos os comandos
; da arquitetura hipotética, incluindo EQU e IF

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

	DOIS:      CONST DIVISOR

	ZERO:      CONST 0
	UM:        CONST 1
	MENOSUM:   CONST -1

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

    COPY SOMA, COPIA

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