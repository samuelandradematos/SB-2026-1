; ============================================================
; SOMA.ASM - Operação de adição (16 e 32 bits)
; Software Básico - Calculadora IA-32
; Compilar: nasm -f elf SOMA.ASM -o SOMA.O
; ============================================================

global soma

extern print_string
extern print_int
extern read_num16
extern read_num32

section .data
    msg_num1      db  'Digite o primeiro numero: ', 0
    len_num1      equ $ - msg_num1 - 1

    msg_num2      db  'Digite o segundo numero: ', 0
    len_num2      equ $ - msg_num2 - 1

    msg_result    db  'Resultado: ', 0
    len_result    equ $ - msg_result - 1

section .text

; ============================================================
; soma: realiza a operação de soma
; Parâmetro na pilha: [precisao] (0 = 16 bits, 1 = 32 bits)
; Retorno em EAX: resultado da soma
; Variáveis: todas LOCAIS NA PILHA
;
; Layout do stack frame local:
;   [ebp - 4]  = operando A (dword)
;   [ebp - 8]  = operando B (dword)
; ============================================================
soma:
    push ebp
    mov  ebp, esp
    sub  esp, 8           ; 2 variáveis locais (4 bytes cada)
    push ebx
    push esi
    push edi

    ; Guarda precisão em EBX (0=16, 1=32)
    movzx ebx, byte [ebp+8]

    ; ---- Pede primeiro número ----
    push dword len_num1
    push dword msg_num1
    call print_string
    add  esp, 8

    ; Lê A conforme precisão
    test ebx, ebx
    jnz  .soma_a32

    call read_num16       ; retorna em EAX (sign-extended 16→32)
    jmp  .soma_a_ok

.soma_a32:
    call read_num32

.soma_a_ok:
    mov  [ebp - 4], eax   ; armazena A localmente

    ; ---- Pede segundo número ----
    push dword len_num2
    push dword msg_num2
    call print_string
    add  esp, 8

    ; Lê B conforme precisão
    test ebx, ebx
    jnz  .soma_b32

    call read_num16
    jmp  .soma_b_ok

.soma_b32:
    call read_num32

.soma_b_ok:
    mov  [ebp - 8], eax   ; armazena B localmente

    ; ---- Realiza a soma ----
    mov  eax, [ebp - 4]   ; EAX = A
    add  eax, [ebp - 8]   ; EAX = A + B

    ; Para 16 bits: limita o resultado a 16 bits com sinal
    test ebx, ebx
    jnz  .soma_resultado

    movsx eax, ax         ; sign-extend do resultado 16 bits

.soma_resultado:
    ; Armazena resultado localmente e exibe
    push eax              ; salva resultado na pilha temporariamente

    ; ---- Exibe "Resultado: " ----
    push dword len_result
    push dword msg_result
    call print_string
    add  esp, 8

    ; ---- Exibe o valor numérico ----
    pop  eax              ; recupera resultado
    push eax              ; passa para print_int (parâmetro na pilha)
    call print_int
    add  esp, 4

    ; EAX = resultado (para retorno)
    mov  eax, [ebp - 4]
    add  eax, [ebp - 8]

    pop  edi
    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret
