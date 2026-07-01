; ============================================================
; XXXXX.ASM - Operação de XXXXX (16 e 32 bits)
; Software Básico - Calculadora IA-32
; Substituir XXXXX pelo nome da operação (SUBTRACAO, MULTIPLICACAO, etc.)
; ============================================================
;
; Para cada nova operação:
; 1. Copie este arquivo com o nome correto (ex: SUBTRACAO.ASM)
; 2. Substitua "xxxxx" pelo nome da função (ex: subtracao)
; 3. Implemente a lógica na seção indicada
; 4. Em CALCULADORA.ASM: adicione "extern xxxxx" no topo
; 5. Em CALCULADORA.ASM: adicione o bloco ".op_xxxxx:" no _start
; 6. Compile: nasm -f elf32 XXXXX.ASM -o XXXXX.O
; 7. Adicione XXXXX.O no comando ld
;
; NOTAS IMPORTANTES:
; - Para MULTIPLICACAO e EXPONENCIACAO: verificar overflow após imul/mul
;   usando a flag OF (jo para saltar se overflow)
; - Para DIVISAO e MOD: usar idiv (divisão com sinal)
;   EDX:EAX / operando → quociente em EAX, resto em EDX
;   Lembre de fazer cdq antes de idiv para sign-extend EAX→EDX:EAX
; - Para EXPONENCIACAO: implementar como loop de multiplicações sucessivas
; ============================================================

global xxxxx

extern print_string
extern print_int
extern read_num16
extern read_num32

section .data
    msg_num1_x    db  'Digite o primeiro numero: ', 0
    len_num1_x    equ $ - msg_num1_x - 1

    msg_num2_x    db  'Digite o segundo numero: ', 0
    len_num2_x    equ $ - msg_num2_x - 1

    msg_result_x  db  'Resultado: ', 0
    len_result_x  equ $ - msg_result_x - 1

    ; Descomente e use para overflow (MULTIPLICACAO e EXPONENCIACAO):
    ; msg_overflow  db  'OCORREU OVERFLOW', 10, 0
    ; len_overflow  equ $ - msg_overflow - 1

section .text

; ============================================================
; xxxxx: realiza a operação
; Parâmetro na pilha: [precisao] (0 = 16 bits, 1 = 32 bits)
; Retorno em EAX: resultado
; Layout local:
;   [ebp - 4] = operando A
;   [ebp - 8] = operando B
; ============================================================
xxxxx:
    push ebp
    mov  ebp, esp
    sub  esp, 8
    push ebx
    push esi
    push edi

    movzx ebx, byte [ebp+8]   ; EBX = precisão (0=16, 1=32)

    ; --- Lê operando A ---
    push dword len_num1_x
    push dword msg_num1_x
    call print_string
    add  esp, 8

    test ebx, ebx
    jnz  .x_a32
    call read_num16
    jmp  .x_a_ok
.x_a32:
    call read_num32
.x_a_ok:
    mov  [ebp-4], eax

    ; --- Lê operando B ---
    push dword len_num2_x
    push dword msg_num2_x
    call print_string
    add  esp, 8

    test ebx, ebx
    jnz  .x_b32
    call read_num16
    jmp  .x_b_ok
.x_b32:
    call read_num32
.x_b_ok:
    mov  [ebp-8], eax

    ; ================================================================
    ; TODO: implemente a operação aqui usando [ebp-4] e [ebp-8]
    ; Coloque o resultado em EAX
    ; ================================================================
    mov  eax, [ebp-4]
    ; ...sua operação aqui...

    ; --- Exibe resultado ---
    push eax
    push dword len_result_x
    push dword msg_result_x
    call print_string
    add  esp, 8
    pop  eax

    push eax
    call print_int
    add  esp, 4

    ; --- Retorno ---
    mov  eax, [ebp-4]    ; ajuste conforme necessário
    pop  edi
    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret
