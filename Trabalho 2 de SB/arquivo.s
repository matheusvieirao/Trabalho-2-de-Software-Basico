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
NUM DD -234


section .bss
	OUT_N RESB 12

section .text

;ecx contem o endereço do numero a ser impresso
output_n:
    push eax
    xor esi, esi ; contador de loop / casa decimal

    ;;;;pega o modulo;;;;
    mov eax, [ecx]    
    cmp eax, 0
    jge positivo
    imul eax, -1
    mov BYTE [OUT_N], '-'
    inc esi
    
    ;;;;divisao;;;;
    positivo:
    mov edx, eax
    shr edx, 16 ;mover divisor para dx:ax (dividendo)
    mov bx, 10 ;divisor
    div_10:
    idiv bx
    add dx, '0' ; soma com o valor de 0 em ASCII
    mov BYTE [OUT_N + esi], dl  ;salva na string o resto da divisao por 10
    inc esi
    xor dx, dx
    cmp ax, 0 ;ver se o quociente (ax) é zero
    jnz div_10 ;se nao for, dividir de novo ax por 10. 
	
    ;;;;marcando fim da string;;;;
    mov BYTE [OUT_N + esi], 0dh 
    inc esi
    mov BYTE [OUT_N + esi], 0ah
    inc esi
    
    ;;;;invertendo posicao dos numeros na string;;;;
    mov ecx, esi
    sub ecx, 3 ;ultimo char da string
    xor ebx, ebx ;ebx vai crescendo do inicio ao fim e ecx do fim ao inicio
    mov BYTE dl, [OUT_N] ;para ver se é negativo
    cmp dl, '-'
    jne denovo
    inc ebx
    denovo: 
    mov BYTE dh, [OUT_N + ebx]
    mov BYTE dl, [OUT_N + ecx]
    mov [OUT_N + ecx], dh
    mov [OUT_N + ebx], dl
    inc ebx
    sub ecx, 1
    cmp ecx, ebx
    jg denovo   
    
    ;;;;imprime;;;;
    mov eax, 4
    mov ebx, 1
    mov ecx, OUT_N
    mov edx, esi
    int 80h
    pop eax
ret 
