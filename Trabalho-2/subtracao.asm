segment .data
segment .text
    global SUBTRACAO

SUBTRACAO:
    enter 0,0
    mov eax, [ebp + 12]
    sub eax, [ebp + 8]
    leave
    ret