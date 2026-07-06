; --
; soma: soma dois números
; Parâmetros na pilha:
; [ebp+12]  = num1
; [ebp+8] = num2
; Retorno: eax = resultado em 32bits
; --

global soma_32
global soma_16
soma_32:
	push ebp
	mov ebp, esp

	mov eax, [ebp + 12] ; primeiro operando inserido pelo user!
	add eax, [ebp + 8] ; segundo operando

	mov esp, ebp
	pop ebp
	ret


soma_16:
	push ebp
	mov ebp, esp

	mov ax, [ebp + 12]
	add ax, [ebp + 8]
	movsx eax, ax   ; estende resultado para 32 bits

	mov esp, ebp
	pop ebp
	ret
