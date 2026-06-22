section .data
msg_nome db "Bem-vindo. Digite seu nome: "
tam_msg_nome equ $-msg_nome ; calcula o tamanho da mensagem em bytes

msg_hola db "Hola, "
tam_msg_hola equ $-msg_hola

msg_inicial db ", bem-vindo ao programa de CALC IA-32", 0Ah ; 0Ah = enter (\n)
tam_msg_inicial equ $-msg_inicial
msg_precisao db "Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): "
tam_msg_precisao equ $-msg_precisao

; linhas de opcoes do menu
opcoes_menu db "ESCOLHA UMA OPCAO:", 13, 10
tam_opcoes_menu equ $-opcoes_menu

opcoes_menu_add db "- 1: SOMA", 13, 10
tam_opcoes_menu_add equ $-opcoes_menu_add

opcoes_menu_sub db "- 2: SUBTRACAO", 13, 10
tam_opcoes_menu_sub equ $-opcoes_menu_sub

opcoes_menu_mul db "- 3: MULTIPLICACAO", 13, 10
tam_opcoes_menu_mul equ $-opcoes_menu_mul

opcoes_menu_div db "- 4: DIVISAO", 13, 10
tam_opcoes_menu_div equ $-opcoes_menu_div

opcoes_menu_exp db "- 5: EXPONENCIACAO", 13, 10
tam_opcoes_menu_exp equ $-opcoes_menu_exp

opcoes_menu_mod db "- 6: MOD", 13, 10
tam_opcoes_menu_mod equ $-opcoes_menu_mod

opcoes_menu_sair db "- 7: SAIR", 13, 10
tam_opcoes_menu_sair equ $-opcoes_menu_sair



section .bss
nome resb 100 ; reserva cem bytes para o nome
tam_nome equ $-nome

precisao resb 1 ; é um número (0 ou 1) = string de 1 byte
tam_precisao equ 1

opcao_escolhida resb 1
tam_opcao_escolhida equ 1

section .text
global _start

_start:
	; Pede para o usuário digitar o nome
	push ebp
	mov ebp, esp

	push tam_msg_nome
	push msg_nome
	call print_string
	add esp, 8 ; remove tam_msg_nome e msg_nome da pilha

	; Recebe o nome digitado pelo usuário
	push tam_nome ; epb+12
	push nome ; epb+8
	call read_string ; epb+4, ele sai e a próxima chamada se torna epb+4
	sub eax, 1 ; remove o enter do nome
	mov [ebp - 4], eax ; eax guarda retorno = tamanho em bytes da string inserida

	; printa: Hola, nome, bem-vindo ao programa...
	push tam_msg_hola
	push msg_hola
	call print_string
	add esp, 8

	; Ecoa o nome recebido pelo user
	call print_string
	add esp, 8
 
	push tam_msg_inicial
	push msg_inicial
	call print_string
	add esp, 8

	; pergunta precisao e lê
	push tam_msg_precisao
	push msg_precisao
	call print_string
	add esp, 8

	push tam_precisao
	push precisao
	call read_string
	; sub ebp, 4 ; remove tam_precisao da pilha
	; sub esp, 4

	; MOSTRAR MENU
	push tam_opcoes_menu
	push opcoes_menu
	call mostrar_menu

	mov esp, ebp
	pop ebp

	; exit
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80

print_string:
	; inicializa pilha interna
	push ebp ; <- esp ta apontando aqui
	mov ebp, esp ; esp e ebp tem memso valor agora

	; 1. imprime uma mensagem e sai
	mov eax, 4 ; syscall write = 4
	mov ebx, 1 ; fd=1, tela
	mov ecx, [ebp + 8] ; acessa msg na pilha
	mov edx, [ebp + 12] ; acessa tamanho EM BYTES
	int 0x80

	; limpa pilha local, volta para a do programa principal
	mov esp, ebp
	pop ebp
	ret

read_string:
	push ebp
	mov ebp, esp

	; lê o nome do teclado
	mov eax, 3 ; syscall de leitura
	mov ebx, 0 ; fd=0, teclado
	mov ecx, [ebp + 8]
	mov edx, [ebp + 12]
	int 0x80

	mov esp, ebp
	pop ebp
	ret

mostrar_menu:
	push ebp
	mov ebp, esp

	push tam_opcoes_menu
	push opcoes_menu
	call print_string
	add esp, 8

	push tam_opcoes_menu_add
	push opcoes_menu_add
	call print_string
	add esp, 8

	push tam_opcoes_menu_sub
	push opcoes_menu_sub
	call print_string
	add esp, 8

	push tam_opcoes_menu_mul
	push opcoes_menu_mul
	call print_string
	add esp, 8

	push tam_opcoes_menu_div
	push opcoes_menu_div
	call print_string
	add esp, 8

	push tam_opcoes_menu_exp
	push opcoes_menu_exp
	call print_string
	add esp, 8

	push tam_opcoes_menu_mod
	push opcoes_menu_mod
	call print_string
	add esp, 8

	push tam_opcoes_menu_sair
	push opcoes_menu_sair
	call print_string
	add esp, 8

	mov esp, ebp
	pop ebp
	ret
