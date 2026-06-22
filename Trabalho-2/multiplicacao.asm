%include "main.asm"
segment .data
error_msg db "OCORREU OVERFLOW" ; Texto a ser exibido quando ocorre overflow
tam_error_msg equ $-error_msg ; Tamanho da mensagem de erro
zero equ 0x00000000

segment .text
    global MULTIPLICACAO

MULTIPLICACAO:
    enter 0,0
    push eax
    push edx
    mov edx, zero
    mov eax, [ebp + 12]
    mull eax, [ebp + 8]
    cmp edx, zero
    je RETORNO
    push tam_error_msg
    push error_msg
    call print_string
    leave
    ret
RETORNO:
    leave
    ret



