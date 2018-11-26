global _start

section .text
_start:
mov DWORD eax, 3
add eax, [DOIS]
mov DWORD [R], eax
mov ecx, [R]
call output_n
mov eax, 1
mov ebx, 0
int 80h
section .bss
R RESD 1
section .data
NUM DD 234


section .bss
	OUT_N RESB 12

section .text
output_n:
	push eax
	; mover de ecx para dx : ax(dividendo)
	mov edx, ecx
	shr edx, 16; ver se  right mesmo
	mov eax, ecx

	xor esi, esi; contador de loop / casa decimal
	mov bx, 10; divisor

	div_10 : 
	idiv bx

	; Colocar o resto(dx) no vetor.
	; ver se o quociente(ax) j  0
	; se nao for, dividir de novo ax por 10.
	; (provavelmente isso nao funciona pra numeros muito grandes ja que vamos perder parte do quociente na divisao nesses casos.)
	add dx, '0'; soma com o valor de 0 em ASCII
	mov BYTE[OUT_N + esi], dl; salva na string
			xor dx, dx
	inc esi
	cmp ax, 0
	jnz div_10

	mov BYTE[OUT_N + esi], 0dh
	inc esi
	mov BYTE[OUT_N + esi], 0ah
	inc esi

	mov ebx, OUT_N
	mov ecx, esi
	sub ecx, 3
	call swap_b; ebx contem o endereço da string e acx quantos bytes vao ser invertidos

	mov eax, 4
	mov ebx, 1
	mov ecx, OUT_N
	mov edx, esi
	int 80h
	pop eax
	ret

swap_b :
	push esi
	xor esi, esi; esi vai crescendo do inicio ao fim e ecx do fim ao inicio

	denovo :
	mov BYTE dh, [ebx + esi]
	mov BYTE dl, [ebx + ecx]
	mov[ebx + ecx], dh
	mov[ebx + esi], dl

	inc esi
	sub ecx, 1

	cmp ecx, esi
	jg denovo

	pop esi
	ret
