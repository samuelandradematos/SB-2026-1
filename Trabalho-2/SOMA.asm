; --
; soma: soma dois números
; Parâmetros na pilha:
; [ebp+8]  = num1
; [ebp+12] = num2
; Retorno: eax = resultado em 32bits
; --

global soma_32
global soma_16
soma_32:
	push ebp
	mov ebp, esp

	mov eax, [ebp + 8]
	add eax, [ebp + 12]

	mov esp, ebp
	pop ebp
	ret


soma_16:
	push ebp
	mov ebp, esp

	mov ax, [ebp + 8]
	add ax, [ebp + 12]
	movzx eax, ax   ; estende resultado para 32 bits

	mov esp, ebp
	pop ebp
	ret
