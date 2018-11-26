global _start
msg_overflow DB 'Overflow na multiplicacao (o resultado deve ter ate 32 bits)', 0dH, 0aH
section .text
_start:
push eax
mov eax, 3
mov ebx, 0
mov ecx, B
mov edx, 4
int 80h
pop eax
push eax
mov eax, 3
mov ebx, 0
mov ecx, H
mov edx, 4
int 80h
pop eax
mov DWORD eax, B
add eax, [H]
mov DWORD [R], eax
push eax
mov eax, 4
mov ebx, 1
mov ecx, R
mov edx, 4
int 80h
pop eax
mov eax, 1
mov ebx, 0
int 80h
section .bss
B RESD 1
H RESD 1
R RESD 1
