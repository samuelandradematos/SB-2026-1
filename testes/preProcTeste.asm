    ; comentário de teste no topo
SECTION DATA
DOIS:       
        CONST 2
OLD_DATA: SPACE
NEW_DATA: SPACE
TMP_DATA        : SPACE
section     text
TAM     : EQU 10
ZERO: EQU 0
IF TAM
LOAD OLD_DATA
IF ZERO
INPUT  OLD_DATA
L1      :
L2  :
L3      :     
    DIV DOIS ;comentário de teste aqui
STORE  NEW_DATA
DEZ: CONST TAM
MULT DOIS
STORE TMP_DATA
    LOAD         OLD_DATA   
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA, OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
; comentário de teste no fim
