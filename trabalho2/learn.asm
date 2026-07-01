; nasm -f elf32 ola.asm -o ola.o # montar (gera .objs)
; ld -m elf_i386 ola.o -o ola # linkar (gera .exe)

section .data ;dados inicializados
msg db "Ola", 0Ah

section .bss ;dados não inicializados
nome resb 100 ; reserva 100bytes


section .text
global _start

_start:
	; mov eax, 1 ; chama a syscall 1 = exit
	; mov ebx, 0 ; passa parâmetros para a syscall, exit(0)
	; int 0x80 ;chamadas (syscalls) para o S.O

	; ler do teclado
	mov eax, 3
	mov ebx, 0 ; fd=0, teclado
	mov ecx, nome
	mov edx, 10
	int 0x80

	; escrever na tela
	mov eax, 4 ; 4 = escrever na tela
	mov ebx, 1 ; fd (0=teclado, 1=tela, 2=stderr)
	mov ecx, msg ; buffer = msg
	mov edx, 10 ; tamanho
	int 0x80

	; sair
	mov eax, 1
	mov ebx, 0
	int 0x80