section .data
msg_nome db "Bem-vindo. Digite seu nome: ",0
msg_hola db "Hola, ",0
msg_inicial db ", bem-vindo ao programa de CALC IA-32", 0Ah,0 ; 0Ah = enter (\n)
msg_precisao db "Vai trabalhar com 16 ou 32 bits (digite 0 para 16, e 1 para 32): ",0

; linhas de opcoes do menu
opcoes_menu db "ESCOLHA UMA OPCAO:", 13, 10, 0
opcoes_menu_add db "- 1: SOMA", 13, 10, 0
opcoes_menu_sub db "- 2: SUBTRACAO", 13, 10, 0
opcoes_menu_mul db "- 3: MULTIPLICACAO", 13, 10, 0
opcoes_menu_div db "- 4: DIVISAO", 13, 10, 0
opcoes_menu_exp db "- 5: EXPONENCIACAO", 13, 10, 0
opcoes_menu_mod db "- 6: MOD", 13, 10, 0
opcoes_menu_sair db "- 7: SAIR", 13, 10, 0


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
	call leitura_nome
	call boas_vindas
	call entrada_precisao
	call executar_menu

	; todo: remover isso depois que as opcoes do menu estiverem prontas
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80

leitura_nome:
	; Pede para o usuário digitar o nome
	push msg_nome
	call print_string
	add esp, 4 ; remove tam_msg_nome e msg_nome da pilha

	; Recebe o nome digitado pelo usuário
	push nome ; epb+8
	call read_string ; epb+4, ele sai e a próxima chamada se torna epb+4
    add esp, 4

	push nome
    call remove_newline
    add esp, 4

	ret

boas_vindas:
	; printa: Hola, nome, bem-vindo ao programa...
	push msg_hola
	call print_string
	add esp, 4

	; Ecoa o nome recebido pelo user
	push nome
	call print_string
	add esp, 4
 
	push msg_inicial
	call print_string
	add esp, 4

	ret

entrada_precisao:
	; pergunta precisao e a lê
	push msg_precisao
	call print_string
	add esp, 4

	; TODO: Arrumar precisao
	push precisao
	call read_string
	add esp, 4

	ret

print_string:
	; parâmetros:
	; 	ebp + 8 = local de armazenamento da string
	
	; inicializa pilha interna
	push ebp ; <- esp ta apontando aqui
	mov ebp, esp ; esp e ebp tem memso valor agora

	; pega string da pilha
    push [ebp + 8]
    call strlen ; calcula tamanho da string
    add esp, 4
	mov edx, eax ; strlen retorna tamanho em eax

	; imprime uma string passada
	mov eax, 4 ; syscall write = 4
	mov ebx, 1 ; fd=1, tela
	mov ecx, [ebp + 8] ; acessa string na pilha
	int 0x80

	; limpa pilha local, volta para a do programa principal
	mov esp, ebp
	pop ebp
	ret

read_string:
	; parâmetros:
	; 	ebp + 8 = local de armazenamento da string

	push ebp
	mov ebp, esp

	; lê o nome do teclado
	mov eax, 3 ; syscall de leitura
	mov ebx, 0 ; fd=0, teclado
	mov ecx, [ebp + 8]
	mov edx, 100 ; número max de bytes a ler
	int 0x80

	; eax guarda retorno = tamanho em bytes da string inserida
	sub eax, 1

	mov esp, ebp
	pop ebp
	ret


strlen:
	push ebp
	mov ebp, esp

	mov esi, [ebp + 8]   ; string
	xor ecx, ecx         ; contador = 0

	.loop:
		cmp byte [esi + ecx], 0
		je .fim
		inc ecx
		jmp .loop

	.fim:
		mov eax, ecx         ; retorna tamanho em EAX

		mov esp, ebp
		pop ebp
		ret


remove_newline:
    ; pega o parâmetro da pilha
    ; [esp+4] = endereço da string (nome, por exemplo)
    mov esi, [esp+4]

	.loop:
		; carrega 1 byte da string atual
		mov byte al, [esi]

		; verifica se chegou no final da string
		; (0 = fim da string em C/assembly)
		cmp al, 0
		je .fim

		; verifica se o caractere atual é '\n'
		; ASCII 10 = quebra de linha do ENTER
		cmp al, 10
		je .zera

		; se não for '\n' nem fim da string,
		; avança para o próximo caractere
		inc esi
		jmp .loop

	.zera:
		; substitui '\n' por 0 (terminador de string)
		; isso “corta” a string aqui
		mov byte [esi], 0

	.fim:
		; finaliza a função
		ret
	



executar_menu:
.loop:
	call mostrar_menu

	push opcao_escolhida
	call read_string
    add esp, 4
	call remove_newline

	cmp byte [opcao_escolhida], '1'
	je soma


	cmp byte [opcao_escolhida], '7'
	je sair
	; cmp byte [precisao], '0'

	jmp .loop


mostrar_menu:
	push ebp
	mov ebp, esp

	push opcoes_menu
	call print_string
	add esp, 4

	push opcoes_menu_add
	call print_string
	add esp, 4

	push opcoes_menu_sub
	call print_string
	add esp, 4

	push opcoes_menu_mul
	call print_string
	add esp, 4

	push opcoes_menu_div
	call print_string
	add esp, 4

	push opcoes_menu_exp
	call print_string
	add esp, 4

	push opcoes_menu_mod
	call print_string
	add esp, 4

	push opcoes_menu_sair
	call print_string
	add esp, 4

	mov esp, ebp
	pop ebp
	ret

soma:
	cmp byte [precisao], '0'
	je ._16
._32:

._16:



sair:
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80



; menu_loop:
; 	; EXECUTAR MENU
; 	call mostrar_menu

; 	push tam_opcao_escolhida
; 	push opcao_escolhida


; 	add esp, 8

; 	; lê opcao escolhina no menu
; 	push [ebp + 12] ; tamamanho opcao
; 	push [ebp + 8] ; opcao_escolhida
; 	call read_string
; 	add esp, 8

; 	cmp byte [opcao_escolhida], '7'
; 	je sair


; MOSTRA MENU
; ESCLHE OPCAO
; EXECUTA A CONTA
; MOSTRA MENU ...