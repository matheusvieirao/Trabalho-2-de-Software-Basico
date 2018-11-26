global _start
msg_overflow DB 'Overflow na multiplicacao (o resultado deve ter ate 32 bits)', 0dH, 0aH
section .text
_start:
mov DWORD eax, 3
add eax, [DOIS]
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
R RESD 1
section .data
DOIS DD 2
