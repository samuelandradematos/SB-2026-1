; ============================================================
; CALCULADORA.ASM - Programa principal + funções de I/O
; Software Básico - Calculadora IA-32 (16 e 32 bits)
; Compilar: nasm -f elf CALCULADORA.ASM -o CALCULADORA.O
; ============================================================

global _start
global print_string       ; exporta para os outros arquivos
global read_string
global read_num16
global read_num32
global print_int

extern soma               ; importa de SOMA.ASM

section .data
    ; --- Strings de mensagem (variáveis globais permitidas) ---
    ; Nota: strings terminam com 0 para facilitar strlen, mas len_xxx
    ;       NÃO inclui o byte nulo (para sys_write não imprimí-lo)
    msg_benvindo    db  'Bem-vindo. Digite seu nome: ', 0
    len_benvindo    equ $ - msg_benvindo - 1

    msg_hola_1      db  'Hola, ', 0
    len_hola_1      equ $ - msg_hola_1 - 1

    msg_hola_2      db  ', bem-vindo ao programa de CALC IA-32', 10, 0
    len_hola_2      equ $ - msg_hola_2 - 1

    msg_precisao    db  'Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): ', 0
    len_precisao    equ $ - msg_precisao - 1

    msg_menu0       db  'ESCOLHA UMA OPCAO:', 10, 0
    len_menu0       equ $ - msg_menu0 - 1

    msg_menu1       db  '- 1: SOMA', 10, 0
    len_menu1       equ $ - msg_menu1 - 1

    msg_menu2       db  '- 2: SUBTRACAO', 10, 0
    len_menu2       equ $ - msg_menu2 - 1

    msg_menu3       db  '- 3: MULTIPLICACAO', 10, 0
    len_menu3       equ $ - msg_menu3 - 1

    msg_menu4       db  '- 4: DIVISAO', 10, 0
    len_menu4       equ $ - msg_menu4 - 1

    msg_menu5       db  '- 5: EXPONENCIACAO', 10, 0
    len_menu5       equ $ - msg_menu5 - 1

    msg_menu6       db  '- 6: MOD', 10, 0
    len_menu6       equ $ - msg_menu6 - 1

    msg_menu7       db  '- 7: SAIR', 10, 0
    len_menu7       equ $ - msg_menu7 - 1

    msg_opcao_inv   db  'Opcao invalida. Tente novamente.', 10, 0
    len_opcao_inv   equ $ - msg_opcao_inv - 1

    msg_nao_impl    db  'Operacao ainda nao implementada.', 10, 0
    len_nao_impl    equ $ - msg_nao_impl - 1

    msg_enter       db  'Pressione ENTER para continuar...', 10, 0
    len_enter       equ $ - msg_enter - 1


    var_nome        times 64 db 0   ; nome do usuário
    var_precisao    db 0            ; 0 = 16 bits, 1 = 32 bits
    var_opcao       db 0            ; opção do menu

section .text

; ============================================================
; _start: programa principal - APENAS chama funções
; ============================================================
_start:
    ; Exibe "Bem-vindo. Digite seu nome:"
    push dword len_benvindo
    push dword msg_benvindo
    call print_string
    add esp, 8

    ; Lê o nome do usuário
    push dword 63
    push dword var_nome
    call read_string
    add esp, 8

    ; Exibe "Hola, "
    push dword len_hola_1
    push dword msg_hola_1
    call print_string
    add esp, 8

    ; Exibe o nome (calcula comprimento e imprime)
    push dword var_nome
    call strlen_and_print
    add esp, 4

    ; Exibe ", bem-vindo ao programa de CALC IA-32\n"
    push dword len_hola_2
    push dword msg_hola_2
    call print_string
    add esp, 8

    ; Pergunta precisão
    push dword len_precisao
    push dword msg_precisao
    call print_string
    add esp, 8

    ; Lê precisão (0 ou 1) - lemos como string e convertemos
    push dword var_precisao
    call read_precisao
    add esp, 4

.loop_menu:
    ; Exibe o menu completo (uma chamada por linha conforme especificação)
    push dword len_menu0
    push dword msg_menu0
    call print_string
    add esp, 8

    push dword len_menu1
    push dword msg_menu1
    call print_string
    add esp, 8

    push dword len_menu2
    push dword msg_menu2
    call print_string
    add esp, 8

    push dword len_menu3
    push dword msg_menu3
    call print_string
    add esp, 8

    push dword len_menu4
    push dword msg_menu4
    call print_string
    add esp, 8

    push dword len_menu5
    push dword msg_menu5
    call print_string
    add esp, 8

    push dword len_menu6
    push dword msg_menu6
    call print_string
    add esp, 8

    push dword len_menu7
    push dword msg_menu7
    call print_string
    add esp, 8

    ; Lê opção do menu
    push dword var_opcao
    call read_opcao
    add esp, 4

    ; Verifica opção e despacha para função correta
    movzx eax, byte [var_opcao]
    movzx ecx, byte [var_precisao]

    cmp al, 1
    je .op_soma

    cmp al, 2
    je .op_nao_impl   ; Subtração - não implementada ainda

    cmp al, 3
    je .op_nao_impl   ; Multiplicação - não implementada ainda

    cmp al, 4
    je .op_nao_impl   ; Divisão - não implementada ainda

    cmp al, 5
    je .op_nao_impl   ; Exponenciação - não implementada ainda

    cmp al, 6
    je .op_nao_impl   ; MOD - não implementada ainda

    cmp al, 7
    je .op_sair

    ; Opção inválida
    push dword len_opcao_inv
    push dword msg_opcao_inv
    call print_string
    add esp, 8
    jmp .loop_menu

.op_soma:
    ; Chama soma passando a precisão na pilha
    push ecx              ; var_precisao (0=16, 1=32)
    call soma
    add esp, 4
    jmp .aguarda_enter

.op_nao_impl:
    push dword len_nao_impl
    push dword msg_nao_impl
    call print_string
    add esp, 8
    jmp .aguarda_enter

.aguarda_enter:
    ; Aguarda ENTER antes de mostrar o menu novamente
    push dword msg_enter
    call wait_enter
    add esp, 4
    jmp .loop_menu

.op_sair:
    ; Encerra o programa
    mov eax, 1
    xor ebx, ebx
    int 0x80


; ============================================================
; print_string: imprime string via syscall write
; Parâmetros na pilha (cdecl): [ponteiro_string, num_bytes]
; Não tem retorno (retorno em EAX não é utilizado pelo chamador)
; ============================================================
print_string:
    push ebp
    mov  ebp, esp
    push ebx

    ; Calcula o comprimento real da string (até 0 ou usa o tamanho passado)
    mov  ecx, [ebp+8]    ; ponteiro para a string
    mov  edx, [ebp+12]   ; número de bytes

    ; Desconta o byte nulo final se presente
    ; (as strings têm 0 no final mas len inclui ele; ajustamos)
    ; Na prática nossas len_xxx não incluem o 0, ok.

    mov  eax, 4          ; sys_write
    mov  ebx, 1          ; fd = stdout
    ; ecx = ponteiro (já setado)
    ; edx = tamanho (já setado)
    int  0x80

    pop  ebx
    pop  ebp
    ret


; ============================================================
; read_string: lê string do teclado (stdin)
; Parâmetros na pilha: [ponteiro_buffer, max_bytes]
; Retorno em EAX: número de bytes lidos (sem o '\n')
; ============================================================
read_string:
    push ebp
    mov  ebp, esp
    push ebx

    mov  ecx, [ebp+8]    ; buffer destino
    mov  edx, [ebp+12]   ; max bytes

    mov  eax, 3          ; sys_read
    mov  ebx, 0          ; fd = stdin
    int  0x80

    ; Remove '\n' final
    test eax, eax
    jle  .rs_fim
    mov  ebx, [ebp+8]
    lea  ecx, [ebx + eax - 1]
    cmp  byte [ecx], 10
    jne  .rs_fim
    mov  byte [ecx], 0
    dec  eax

.rs_fim:
    pop  ebx
    pop  ebp
    ret


; ============================================================
; read_num16: lê número inteiro de 16 bits do teclado
; Parâmetros na pilha: nenhum adicional (lê para buffer local)
; Retorno em EAX: valor inteiro com sinal (sign-extended para 32 bits)
; ============================================================
read_num16:
    push ebp
    mov  ebp, esp
    sub  esp, 32          ; buffer local na pilha (32 bytes)
    push ebx
    push esi

    ; Lê string para buffer local
    lea  ecx, [ebp - 32]
    push dword 31
    push ecx
    call read_string
    add  esp, 8
    ; EAX = bytes lidos

    ; Converte string para inteiro
    lea  ecx, [ebp - 32]
    push ecx
    call str_to_int
    add  esp, 4

    ; Limita a 16 bits com sinal: sign-extend de 16 para 32 bits
    movsx eax, ax

    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; read_num32: lê número inteiro de 32 bits do teclado
; Parâmetros na pilha: nenhum adicional
; Retorno em EAX: valor inteiro com sinal de 32 bits
; ============================================================
read_num32:
    push ebp
    mov  ebp, esp
    sub  esp, 32
    push ebx
    push esi

    lea  ecx, [ebp - 32]
    push dword 31
    push ecx
    call read_string
    add  esp, 8

    lea  ecx, [ebp - 32]
    push ecx
    call str_to_int
    add  esp, 4

    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; str_to_int: converte string para inteiro
; Parâmetro na pilha: [ponteiro_string]
; Retorno em EAX: valor inteiro com sinal
; ============================================================
str_to_int:
    push ebp
    mov  ebp, esp
    push ebx
    push ecx
    push edx
    push esi

    mov  esi, [ebp+8]    ; ponteiro para a string

    xor  eax, eax        ; acumulador = 0
    xor  ecx, ecx        ; sinal: 0 = positivo
    xor  ebx, ebx

    ; Verifica sinal negativo
    cmp  byte [esi], '-'
    jne  .sti_loop
    mov  ecx, 1
    inc  esi

.sti_loop:
    movzx ebx, byte [esi]
    test  ebx, ebx
    jz    .sti_fim
    cmp   bl, '0'
    jl    .sti_fim
    cmp   bl, '9'
    jg    .sti_fim

    sub   bl, '0'
    imul  eax, eax, 10
    add   eax, ebx
    inc   esi
    jmp   .sti_loop

.sti_fim:
    test  ecx, ecx
    jz    .sti_ret
    neg   eax

.sti_ret:
    pop  esi
    pop  edx
    pop  ecx
    pop  ebx
    pop  ebp
    ret


; ============================================================
; int_to_str: converte inteiro para string ASCII
; Parâmetros na pilha: [ponteiro_buffer] [valor]
;   [ebp+8]  = ponteiro buffer destino
;   [ebp+12] = valor inteiro
; Retorna em EAX: comprimento da string gerada (com \n, sem \0)
; Layout local: [ebp-4] = ponteiro_inicio (para calcular len)
; ============================================================
int_to_str:
    push ebp
    mov  ebp, esp
    sub  esp, 4           ; [ebp-4] = ponteiro_inicio do buffer
    push ebx
    push ecx
    push edx
    push edi
    push esi

    mov  esi, [ebp+8]    ; ESI = buffer destino (percorre durante escrita)
    mov  eax, [ebp+12]   ; EAX = valor a converter

    mov  [ebp-4], esi    ; salva ponteiro de início

    xor  ecx, ecx        ; ECX = contador de dígitos empilhados
    xor  edi, edi        ; EDI = flag negativo (0 = positivo)

    ; Caso especial: zero
    test eax, eax
    jnz  .its_negativo
    mov  byte [esi], '0'
    mov  byte [esi+1], 10
    mov  byte [esi+2], 0
    mov  eax, 2
    jmp  .its_sair

.its_negativo:
    jge  .its_loop
    neg  eax
    mov  edi, 1

.its_loop:
    ; Extrai dígitos do menos para o mais significativo
    test eax, eax
    jz   .its_montar
    xor  edx, edx
    mov  ebx, 10
    div  ebx             ; EAX = quociente, EDX = dígito (0-9)
    push edx             ; guarda dígito na pilha
    inc  ecx
    jmp  .its_loop

.its_montar:
    ; Escreve sinal se negativo
    test edi, edi
    jz   .its_desempilha
    mov  byte [esi], '-'
    inc  esi

.its_desempilha:
    ; Desempilha dígitos (mais significativo primeiro)
    test ecx, ecx
    jz   .its_newline
    pop  edx
    add  dl, '0'
    mov  [esi], dl
    inc  esi
    dec  ecx
    jmp  .its_desempilha

.its_newline:
    mov  byte [esi], 10  ; '\n'
    inc  esi
    mov  byte [esi], 0   ; terminador nulo

    ; Comprimento = posição_atual - posição_inicial
    sub  esi, [ebp-4]    ; ESI = comprimento (inclui \n, exclui \0)
    mov  eax, esi

.its_sair:
    pop  esi
    pop  edi
    pop  edx
    pop  ecx
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; print_int: imprime inteiro no stdout
; Parâmetro na pilha: [valor inteiro]
; ============================================================
print_int:
    push ebp
    mov  ebp, esp
    sub  esp, 32          ; buffer local para a string do número
    push ebx
    push esi

    lea  ecx, [ebp - 32]  ; ponteiro para buffer local
    mov  eax, [ebp+8]     ; valor a imprimir

    ; Pilha (topo = menor endereço = primeiro a ser acessado por [ebp+8]):
    ;   push valor primeiro (ficará em [ebp+12])
    ;   push buffer depois  (ficará em [ebp+8])
    push eax              ; valor  → [ebp+12] dentro de int_to_str
    push ecx              ; buffer → [ebp+8]  dentro de int_to_str
    call int_to_str
    add  esp, 8
    ; EAX = comprimento da string

    lea  ecx, [ebp - 32]
    push eax              ; comprimento
    push ecx              ; ponteiro
    call print_string
    add  esp, 8

    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; strlen_and_print: calcula strlen de var_nome e imprime
; Parâmetro na pilha: [ponteiro_string]
; ============================================================
strlen_and_print:
    push ebp
    mov  ebp, esp
    push ebx
    push esi

    mov  esi, [ebp+8]
    xor  eax, eax ; eax = 0, limpa o eax

.sp_loop:
    cmp  byte [esi + eax], 0 ; verifica se o byte é \0, fim da string
    je   .sp_print
    inc  eax ; incrementa o eax
    jmp  .sp_loop

.sp_print:
    push eax
    push dword [ebp+8]
    call print_string
    add  esp, 8

    pop  esi
    pop  ebx
    pop  ebp
    ret


; ============================================================
; read_precisao: lê 0 ou 1 do teclado e armazena em [dest]
; Parâmetro na pilha: [ponteiro_destino (1 byte)]
; ============================================================
read_precisao:
    push ebp
    mov  ebp, esp
    sub  esp, 8
    push ebx
    push esi

    lea  ecx, [ebp - 8]
    push dword 7
    push ecx
    call read_string
    add  esp, 8

    lea  esi, [ebp - 8]
    movzx eax, byte [esi]

    ; Aceita '0' ou '1'
    cmp  al, '0'
    je   .rp_zero
    cmp  al, '1'
    je   .rp_um
    ; Default: 32 bits
    jmp  .rp_um

.rp_zero:
    mov  ebx, [ebp+8]
    mov  byte [ebx], 0
    jmp  .rp_fim

.rp_um:
    mov  ebx, [ebp+8]
    mov  byte [ebx], 1

.rp_fim:
    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; read_opcao: lê opção do menu (1-7) e armazena em [dest]
; Parâmetro na pilha: [ponteiro_destino (1 byte)]
; ============================================================
read_opcao:
    push ebp
    mov  ebp, esp
    sub  esp, 8
    push ebx
    push esi

    lea  ecx, [ebp - 8]
    push dword 7
    push ecx
    call read_string
    add  esp, 8

    lea  esi, [ebp - 8]
    movzx eax, byte [esi]

    sub  al, '0'          ; converte char para número
    mov  ebx, [ebp+8]
    mov  [ebx], al

    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret


; ============================================================
; wait_enter: exibe mensagem e aguarda ENTER
; Parâmetro na pilha: [ponteiro_mensagem]
; ============================================================
wait_enter:
    push ebp
    mov  ebp, esp
    sub  esp, 4
    push ebx
    push esi

    ; Calcula comprimento da mensagem
    mov  esi, [ebp+8]
    xor  eax, eax
.we_len:
    cmp  byte [esi + eax], 0
    je   .we_print
    inc  eax
    jmp  .we_len

.we_print:
    push eax
    push dword [ebp+8]
    call print_string
    add  esp, 8

    ; Lê e descarta o ENTER
    lea  ecx, [ebp - 4]
    push dword 3
    push ecx
    call read_string
    add  esp, 8

    pop  esi
    pop  ebx
    mov  esp, ebp
    pop  ebp
    ret
