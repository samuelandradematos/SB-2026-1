global strlen
global remove_newline


; --
; remove_newline: func auxiliar para tirar o \n do fim da string
; Parâmetro na pilha: [ponteiro_string]
; --
remove_newline:
    push ebp
    mov ebp, esp

    mov esi, [ebp+8]   ; ponteiro da string

	.loop:
		mov al, [esi]
		cmp al, 0
		je .fim
		cmp al, 10 ; '\n'
		je .troca
		inc esi
		jmp .loop

	.troca:
		mov byte [esi], 0  ; substitui '\n' por NULL

	.fim:
		pop ebp
		ret


; --
; strlen: calcula o tamanho da string em bytes, sem contar o byte nulo
; Parâmetro na pilha: [ponteiro_string]
; --
strlen:
    push ebp
    mov  ebp, esp
    push esi

    mov  esi, [ebp + 8] ; string passada como param
    xor  eax, eax ; eax = 0, limpa o eax

	.sp_loop:
		cmp  byte [esi + eax], 0 ; verifica se o byte é \0, fim da string
		je   .fim ; se for o último byte, acaba, nao incrementa
		inc  eax ; incrementa o eax (contagem de bytes)
		jmp  .sp_loop

	.fim:
		pop  esi
		pop  ebp
		ret