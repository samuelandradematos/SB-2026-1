global strlen
global remove_newline
global atoi_32
global atoi_16
global itoa_32

extern read_32
extern read_16

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


; --
; strlen: converte de string para int 32bits
; Parâmetro na pilha: [ponteiro_string]
; Retorno em eax:  número
; --
atoi_32:
    push ebp
    mov ebp, esp
    push esi            ; preserva ESI

    mov ecx, [ebp+8]    ; ← lê o ponteiro da pilha

    xor eax, eax
    xor ebx, ebx
    xor edx, edx

    mov bl, [ecx]
    cmp bl, '-'
    jne .loop
    mov dl, 1
    inc ecx

    .loop:
        mov bl, [ecx]
        cmp bl, 10
        je .fim
        cmp bl, 0
        je .fim
        sub bl, '0'
        imul eax, eax, 10
        movzx ebx, bl
        add eax, ebx
        inc ecx
        jmp .loop

    .fim:
        cmp dl, 1
        jne .end
        neg eax

    .end:
        pop esi
        pop ebp
        ret

; --
; strlen: converte de string para int 16bits
; Parâmetro na pilha: [ponteiro_string]
; Retorno em eax:  número
; --
atoi_16:
    push ebp
    mov ebp, esp
    push esi            ; preserva ESI

    mov esi, [ebp+8]    ; ← lê o ponteiro da pilha (use ESI, não SI)

    xor ax, ax
    xor bx, bx
    xor dx, dx

    mov bl, [esi]
    cmp bl, '-'
    jne .loop
    mov dl, 1
    inc esi

    .loop:
        mov bl, [esi]
        cmp bl, 10
        je .fim
        cmp bl, 0
        je .fim
        sub bl, '0'
        imul ax, ax, 10
        movzx bx, bl
        add ax, bx
        inc esi
        jmp .loop

    .fim:
        cmp dl, 1
        jne .end
        neg ax

    .end:
        pop esi
        pop ebp
        ret

; --
; itoa_32: converte o número em EAX para string
; Parâmetro na pilha: [endereco_buffer]
; Retorno: EAX = endereco do início da string
; --
itoa_32:
	push ebp
	mov ebp, esp

	mov edi, [ebp + 8] ; buffer recebido
	add edi, 11        ; fim do buffer

	mov byte [edi], 0  ; terminador nulo

	dec edi

	cmp eax, 0
	jne .loop

	mov byte [edi], '0'
	mov eax, edi
	jmp .fim

	.loop:
		xor edx, edx
		mov ecx, 10
		div ecx

		add dl, '0'
		mov [edi], dl

		dec edi

		test eax, eax
		jnz .loop

		inc edi
		mov eax, edi

	.fim:
		mov esp, ebp
		pop ebp
		ret


