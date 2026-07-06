global divisao_32
global divisao_16

segment .data
segment .text

divisao_32:
    enter 0,0
    push ebx
    mov eax, [ebp + 12]
    cdq
    mov ebx, [ebp + 8]
    idiv ebx
    pop ebx
    leave
    ret

divisao_16:
    enter 0,0
    push ebx
    mov ax, [ebp + 12]
    cwd
    mov bx, [ebp + 8]
    idiv bx
    movsx eax, ax
    pop ebx
    leave
    ret


