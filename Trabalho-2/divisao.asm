segment .data
segment .text
    enter 0,0
    push edx
    mov eax, [ebp + 8]
    idiv eax, [ebp + 12]
    pop edx
    leave
    ret