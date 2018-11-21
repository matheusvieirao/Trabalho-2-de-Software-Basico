TRIANGULO EQU 1
TESTE EQU AA
section .text
_start:
mov eax, 3
mov ebx, 0
mov ecx, B
mov edx, 4
int 80h
mov eax, 3
mov ebx, 0
mov ecx, H
mov edx, 4
int 80h
mov DWORD eax, B
imul [H]
idiv [DOIS]
mov [R], eax
mov eax, 4
mov ebx, 1
mov ecx, R
mov edx, 4
int 80h
mov eax, 1
mov ebx, 0
int 80h
section .bss
B RESB 1
H RESB 1
R RESB 1
section .data
DOIS DW 2
