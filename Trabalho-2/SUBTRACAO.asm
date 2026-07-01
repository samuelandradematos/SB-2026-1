; --
; subtracao: subtrai dois números
; Parâmetros na pilha:
; [ebp+12]  = num1
; [ebp+8] = num2
; Retorno: eax = resultado em 32bits
; --

global subtracao_32
global subtracao_16
subtracao_32:
	push ebp
	mov ebp, esp

	mov eax, [ebp + 12] ; acessa o primeiro parâmetro
	sub eax, [ebp + 8]

	mov esp, ebp
	pop ebp
	ret


subtracao_16:
	push ebp
	mov ebp, esp

	mov ax, [ebp + 12] ; primeiro operando
	sub ax, [ebp + 8]
	movsx eax, ax   ; estende resultado para 32 bits

	mov esp, ebp
	pop ebp
	ret
