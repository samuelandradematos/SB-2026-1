global mod_16
global mod_32
extern divisao_16
extern divisao_32

segment .data
segment .text

mod_32:
    enter 0,0
    push edx                ; Salva EDX pois DIVISAO altera ele
    push [ebp + 12]         ; Empilha operando 1 como dividendo para DIVISAO
    push [ebp + 8]          ; Empilha operando 2 como divisor para DIVISAO
    call divisao_32         ; Chama divisão para realizar a operação
    add esp, 8

    mov eax, edx            ; Coloca em EAX o resto da divisão
    pop edx
    leave
    ret

mod_16:
    enter 0,0
    push edx                ; Salva EDX pois DIVISAO altera ele
    push [ebp + 12]         ; Empilha operando 1 como dividendo para DIVISAO
    push [ebp + 8]          ; Empilha operando 2 como divisor para DIVISAO
    call divisao_16         ; Chama divisão para realizar a operação
    add esp, 8

    movsx eax, dx            ; Coloca em EAX o resto da divisão(sinal extendido para 32bits)
    pop edx
    leave
    ret