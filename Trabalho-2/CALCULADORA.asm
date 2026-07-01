; IMPORTA FUNCOES AUXILIARES
extern strlen
extern remove_newline
extern ler_operandos
extern atoi_16
extern atoi_32
extern itoa_32

; IMPORTA OPERACOES
extern soma_16
extern soma_32
extern subtracao_32
extern subtracao_16

; exporta para os outros arquivos
global _start
global print_string       
global read_string
global read_32
global read_16


section .data
; --- Strings de mensagem (variáveis globais permitidas) ---
; OBS: strings terminam com 0 para facilitar strlen, mas tam_...
;      NÃO incluem o byte nulo (para sys_write não imprimí-lo)
msg_nome db "Bem-vindo. Digite seu nome: ",0
tam_msg_nome equ $-msg_nome-1 ; calcula o tamanho da mensagem em bytes, menos o byte nulo

msg_hola db "Hola, ",0
tam_msg_hola equ $-msg_hola-1

msg_inicial db ", bem-vindo ao programa de CALC IA-32", 0Ah,0 ; 0Ah = enter (\n)
tam_msg_inicial equ $-msg_inicial-1
msg_precisao db "Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): ",0
tam_msg_precisao equ $-msg_precisao-1

msg_numero1 db "Digite o primeiro número: ",0
tam_msg_numero1 equ $-msg_numero1-1
msg_numero2 db "Digite o segundo número: ",0
tam_msg_numero2 equ $-msg_numero2-1

; -- Strings de OPCOES do menu
opcoes_menu db "ESCOLHA UMA OPCAO:", 10, 0
tam_opcoes_menu equ $-opcoes_menu-1

opcoes_menu_add db "- 1: SOMA", 10, 0
tam_opcoes_menu_add equ $-opcoes_menu_add-1

opcoes_menu_sub db "- 2: SUBTRACAO", 10, 0
tam_opcoes_menu_sub equ $-opcoes_menu_sub-1

opcoes_menu_mul db "- 3: MULTIPLICACAO", 10, 0
tam_opcoes_menu_mul equ $-opcoes_menu_mul-1

opcoes_menu_div db "- 4: DIVISAO", 10, 0
tam_opcoes_menu_div equ $-opcoes_menu_div-1

opcoes_menu_exp db "- 5: EXPONENCIACAO", 10, 0
tam_opcoes_menu_exp equ $-opcoes_menu_exp-1

opcoes_menu_mod db "- 6: MOD", 10, 0
tam_opcoes_menu_mod equ $-opcoes_menu_mod-1

opcoes_menu_sair db "- 7: SAIR", 10, 0
tam_opcoes_menu_sair equ $-opcoes_menu_sair-1


msg_resultado db "RESULTADO: ",0
tam_msg_resultado equ $-msg_resultado

msg_nova_linha db 10, 0
tam_msg_nova_linha equ $-msg_nova_linha

msg_enter db "Pressione ENTER para continuar", 0
tam_msg_enter equ $-msg_enter


section .bss
nome resb 100 ; reserva cem bytes para o nome
tam_nome equ $-nome

precisao resb 2 ; 1 caracter numérico + \n
tam_precisao equ 2

opcao_escolhida resb 2 ; 1 caracter numérico + \n
tam_opcao_escolhida equ 2

section .text

_start:

	push dword msg_nome
	push dword tam_msg_nome
	call print_string
	add esp, 8

	push dword nome
	push dword tam_nome
	call read_string
	add esp, 8

	push dword msg_hola
	push dword tam_msg_hola
	call print_string
	add esp, 8

	push dword nome
	call strlen
	add esp, 4

	push dword nome
	push eax ; tamanho da string retornado de strlen
	call print_string
	add esp, 8

	push dword msg_inicial
	push dword tam_msg_inicial
	call print_string
	add esp, 8

	call executar_menu


; funcao de saída de dados de string
print_string:
	push ebp
	mov ebp, esp

	mov eax, 4 ; syscall write
	mov ebx, 1 ; fd=1, tela
	mov ecx, [ebp + 12] ; string
	mov edx, [ebp + 8] ; tamanho da string em bytes
	int 0x80

	xor eax, eax ; descarta o retorno para garantir que nao vamos usar

	mov esp, ebp
	pop ebp
	ret

; funcao de entrada de dados (strings)
read_string:
	push ebp
	mov ebp, esp

	mov eax, 3 ; syscall read
	mov ebx, 0 ; fd=0, teclado
	mov ecx, [ebp + 12] ; variável onde escrever
	mov edx, [ebp + 8] ; tamanho da string em bytes
	int 0x80

	push ecx ; ponteiro para a string lida
	call remove_newline ; remove o \n do fim da string
	add esp, 4

	mov esp, ebp
	pop ebp
	ret

; --
; read_32: lê um número de até 10 dígitos em 32bits e escreve o valor convertido
; na variável apontada pelo parâmetro
; Parâmetro na pilha: [endereco_variavel]
; --
read_32:
	push ebp
	mov ebp, esp

	sub esp, 16 ; buffer local para armazenar a string digitada

	lea eax, [ebp - 16] ; endereco do buffer local
	push eax ; variável onde escrever a string
	push 16 ; recebe string de até 15 caracteres + '\0'
	call read_string
	add esp, 8

	lea eax, [ebp - 16]
	push eax ; passa ponteiro da string lida
	call atoi_32 ; retorna o número em eax
	add esp, 4

	mov ecx, [ebp + 8] ; endereco da variável recebida por parâmetro
	mov [ecx], eax ; escreve o número convertido na variável

	mov esp, ebp
	pop ebp
	ret

; --
; read_32: lê um número de até 10 dígitos em 16bits e escreve o valor convertido
; na variável apontada pelo parâmetro
; Parâmetro na pilha: [endereco_variavel]
; --
read_16:
    push ebp
    mov ebp, esp

    sub esp, 16

    lea eax, [ebp - 16]
    push eax
    push 16
    call read_string
    add esp, 8

    lea eax, [ebp - 16]
    push eax            ; ← passa pela pilha, igual ao atoi_32
    call atoi_16
    add esp, 4

    mov ecx, [ebp + 8]
    mov [ecx], ax       ; só AX (16 bits)

    mov esp, ebp
    pop ebp
    ret


; --
; mostrar_menu: printa o menu e todas as suas opcoes
; Parâmetro na pilha: nenhum. acessa os ponteiros das strings globais
; --
mostrar_menu:
	push ebp
    mov ebp, esp

	push dword opcoes_menu
	push dword tam_opcoes_menu
	call print_string
	add esp, 8

	push dword opcoes_menu_add
	push dword tam_opcoes_menu_add
	call print_string
	add esp, 8

	push dword opcoes_menu_sub
	push dword tam_opcoes_menu_sub
	call print_string
	add esp, 8

	push dword opcoes_menu_mul
	push dword tam_opcoes_menu_mul
	call print_string
	add esp, 8

	push dword opcoes_menu_div
	push dword tam_opcoes_menu_div
	call print_string
	add esp, 8

	push dword opcoes_menu_exp
	push dword tam_opcoes_menu_exp
	call print_string
	add esp, 8

	push dword opcoes_menu_mod
	push dword tam_opcoes_menu_mod
	call print_string
	add esp, 8

	push dword opcoes_menu_sair
	push dword tam_opcoes_menu_sair
	call print_string
	add esp, 8

	push dword msg_nova_linha
	push dword tam_msg_nova_linha
	call print_string

	mov esp, ebp
    pop ebp
    ret



executar_menu:
	push ebp
	mov ebp, esp

	sub esp, 32
	; [ebp-4]  = num1
	; [ebp-8]  = num2
	; [ebp-20] = buffer de saída (12 bytes)

	.loop:
		; pergunta e recebe a precisao
		push dword msg_precisao
		push dword tam_msg_precisao
		call print_string
		add esp, 8

		push dword precisao
		push dword tam_precisao
		call read_string
		add esp, 8

		call mostrar_menu

		; escolher opcao
		push opcao_escolhida
		push tam_opcao_escolhida
		call read_string
		add esp, 8

		; sair
		cmp byte [opcao_escolhida], '7'
		je .sair

		; ler operandos (feito uma vez por loop)
		lea eax, [ebp - 4]
		push eax
		lea eax, [ebp - 8]
		push eax
		call ler_operandos
		add esp, 8

		; checar precisão
		cmp byte [precisao], '0'
		je .op_16

		cmp byte [precisao], '1'
		je .op_32

		jmp .loop

	; OPERACOES DE 32 BITS
	.op_32:
		cmp byte [opcao_escolhida], '1'
		je .soma_32

		cmp byte [opcao_escolhida], '2'
		je .subtracao_32
		; aqui depois entram outras operações:
		; je .mul_32
		; etc

		jmp .loop

	.soma_32:
		push dword [ebp - 8]   ; num2
		push dword [ebp - 4]   ; num1
		call soma_32
		add esp, 8

		jmp .print

	.subtracao_32:
		push dword [ebp - 8]   ; num2
		push dword [ebp - 4]   ; num1
		call subtracao_32
		add esp, 8

		jmp .print


	; OPERACOES DE 16 BITS
	.op_16:
		cmp byte [opcao_escolhida], '1'
		je .soma_16

		cmp byte [opcao_escolhida], '2'
		je .subtracao_16

		; outras operações de 16 bits vao entrar aquiii
		jmp .loop

	.soma_16:
		push dword [ebp - 8]
		push dword [ebp - 4]
		call soma_16
		add esp, 8
		; retorno tbm em eax (32bits)

		jmp .print

	.subtracao_16:
		push dword [ebp - 8]
		push dword [ebp - 4]
		call subtracao_16
		add esp, 8
		; retorno tbm em eax (32bits)

		jmp .print

	.print: ; converte o resultado para string e printa
		lea ecx, [ebp - 20]

		push ecx
		call itoa_32
		add esp, 4

		mov esi, eax

		push esi
		call strlen
		add esp, 4

		mov edx, eax

		; printa resultado:
		; push msg_resultado
		; push tam_msg_resultado
		; call print_string
		; add esp, 8

		push esi
		push edx
		call print_string
		add esp, 8

		; imprime \n dps do número
		push dword msg_nova_linha
		push dword tam_msg_nova_linha
		call print_string
		add esp, 8

		; pede pro user pressionar enter
		push msg_enter
		push tam_msg_enter
		call print_string
		add esp, 8

		; espera o  ENTER
		lea eax, [ebp - 24]
		push eax
		push 2
		call read_string
		add esp, 8

		jmp .loop

	.sair:
		mov esp, ebp
		pop ebp

		mov eax, 1 ; syscall sair
		mov ebx, 0
		int 0x80

; --
; ler_operandos: ;e os 2 operandos passados, tanto para 16bits como para 32
; Parâmetros:
; [ebp+8]  = endereço num1
; [ebp+12] = endereço num2
; --
ler_operandos:
    push ebp
    mov ebp, esp

    cmp byte [precisao], '1'
    je .modo32

	; ler operandos em 16 BITS
	.modo16:

		push msg_numero1
		push tam_msg_numero1
		call print_string
		add esp, 8

		push dword [ebp+8]
		call read_16
		add esp, 4

		push msg_numero2
		push tam_msg_numero2
		call print_string
		add esp, 8

		push dword [ebp+12]
		call read_16
		add esp, 4

		jmp .fim

	; ler operandos em 32 bits
	.modo32:

		push msg_numero1
		push tam_msg_numero1
		call print_string
		add esp, 8

		push dword [ebp+8]
		call read_32
		add esp, 4

		push msg_numero2
		push tam_msg_numero2
		call print_string
		add esp, 8

		push dword [ebp+12]
		call read_32
		add esp, 4

	.fim:
		mov esp, ebp
		pop ebp
    	ret
