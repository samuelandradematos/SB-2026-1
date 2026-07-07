global multiplicacao_16
global multiplicacao_32
extern print_string

segment .data
error_msg db "OCORREU OVERFLOW", 0 ; Texto a ser exibido quando ocorre overflow
tam_error_msg equ $-error_msg-1 ; Tamanho da mensagem de erro

segment .text

multiplicacao_16:
    enter 0,0
    push edx
    push ebx
    mov ax, [ebp + 12]
    mov bx, [ebp + 8]
    imul bx
    movsx eax, ax    
    cmp dx, 0x0000
    je retorno
    push dword error_msg
    push dword tam_error_msg
    call print_string
    add esp, 8
    ; Exit Syscall
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80

multiplicacao_32:
    enter 0,0
    push edx
    push ebx
    mov edx, 0x00000000
    mov eax, [ebp + 12]
    mov ebx, [ebp + 8]
    imul ebx
    cmp edx, 0x00000000
    je retorno
    push dword error_msg
    push dword tam_error_msg
    call print_string
    add esp, 8
    ; Exit Syscall
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80

retorno:
    pop edx
    pop ebx
    leave
    ret



