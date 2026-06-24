extern strlen
extern remove_newline

global _start
global print_string       ; exporta para os outros arquivos
global read_string
; global read_num16
; global read_num32
; global print_int

; extern soma               ; importa de SOMA.ASM


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

espaco_extra db 0, 10


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

	push dword msg_precisao
	push dword tam_msg_precisao
	call print_string
	add esp, 8

	push dword precisao
	push dword tam_precisao
	call read_string
	add esp, 8

	call executar_menu

	; todo: retirar isso daqui
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80


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

	push dword espaco_extra
	push dword 2
	call print_string

	mov esp, ebp
    pop ebp
    ret



executar_menu:
	push ebp
	mov ebp, esp

	.loop:
		call mostrar_menu

		; escolher opcao
		push opcao_escolhida
		push tam_opcao_escolhida
		call read_string
		add esp, 8

		cmp byte [opcao_escolhida], '1'
		je .soma

		cmp byte [opcao_escolhida], '7'
		je .sair

		jmp .loop

	.soma:
		call soma
		jmp .loop

	.sair:
		mov esp, ebp
		pop ebp

		mov eax, 1 ; syscall sair
		mov ebx, 0
		int 0x80

soma:
	push ebp
	mov ebp, esp
	; sub esp, 16 ; reserva espaco para o número, string de até 16bytes 

	push msg_numero1
	push tam_msg_numero1
	call print_string
	add esp, 8

	mov esp, ebp
	pop ebp
	ret

