%include "main.asm"
segment .data
error_msg db "OCORREU OVERFLOW", 0 ; Texto a ser exibido quando ocorre overflow
tam_error_msg equ $-error_msg ; Tamanho da mensagem de erro
zero equ 0x00000000

segment .text
    global multiplicacao

multiplicacao:
    enter 0,0
    push eax
    push edx
    mov edx, zero
    mov eax, [ebp + 12]
    mull eax, [ebp + 8]
    cmp edx, zero
    je retorno
    push tam_error_msg
    push error_msg
    call print_string
; Exit Syscall
	mov eax, 1 ; syscall sair
	mov ebx, 0
	int 0x80

retorno:
    pop edx
    pop eax
    leave
    ret



