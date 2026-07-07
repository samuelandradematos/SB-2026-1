global exponenciacao_32
global exponenciacao_16
extern multiplicacao_32
extern multiplicacao_16

segment .data
segment .text

exponenciacao_32:
    enter 0,0
    push ebx
    push ecx
    sub ebx, ebx            ; Zera EBX
    mov ecx, [ebp + 8]      ; Salva em ECX operando 2(expoente)
    cmp ecx, 0               ; Verifica se o expoente é zero
    je  .zero_exp_32
    cmp ecx, 1               ; Verfica se o expoente é 1
    je .um_exp_32           
    push [ebp + 12]         ; Empilha operando 1(base), como operando 2
    push [ebp + 12]         ; Empilha operando 1(base), como operando 1
    call multiplicacao_32   ; Chama multiplicacao_32 para realizar a operação
    sub ecx, 2
    cmp ecx, 1
    jge .loop_exp_32        ; Vai para o loop se o expoente for maior que 2
    leave
    ret

    .loop_exp_32:
        mov [esp + 4], eax      ; Troca o valor do operando 2 para o resultado da operação anterior
        call multiplicacao_32   ; Chama multiplicacao_32 para realizar a operação
        sub ecx, 1              
        jnz .loop_exp_32        ; Realiza mais um loop
        pop ecx
        pop ebx
        leave
        ret
    
    .zero_exp_32:
        mov eax, 1
        pop ecx
        pop ebx
        leave
        ret
    .um_exp_32:
        mov eax, [ebp + 12]
        pop ecx
        pop ebx
        leave
        ret

exponenciacao_16:
    enter 0,0
    push ebx
    push ecx
    sub ebx, ebx            ; Zera EBX, embora vai usar apenas BX
    mov cx, [ebp + 8]       ; Salva em CX operando 2(expoente)
    cmp cx, 0               ; Verifica se o expoente é zero
    je  .zero_exp_16
    cmp cx, 1               ; Verfica se o expoente é 1
    je .um_exp_16           
    push [ebp + 12]         ; Empilha operando 1(base), como operando 2 da multiplicação
    push [ebp + 12]         ; Empilha operando 2(base), como operando 1 da multiplicação
    call multiplicacao_16   ; Chama multiplicacao_16 para realizar a operação
    sub cx, 2               ; Subtrai 2 do expoente(primeira operação é equivalente a base²)
    cmp cx, 1
    jge .loop_exp_16        ; Vai para o loop se o expoente for maior que 2
    leave
    ret

    .loop_exp_16:
        mov [esp + 4], ax
        call multiplicacao_16
        sub cx, 1
        jnz .loop_exp_16
        leave
        ret

    .zero_exp_16:
        mov ax, 1
        pop ecx
        pop ebx
        leave
        ret

    .um_exp_16:
        mov ax, [ebp + 12]
        pop ecx
        pop ebx
        leave
        ret
