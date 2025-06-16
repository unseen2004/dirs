section .data
    hex_chars db '0123456789ABCDEF'

section .bss
    output resb 8
    newline resb 1

section .text
global _start

_start:
    mov eax, 123456
    mov edi, output + 7
    mov byte [newline], 0xA
    mov ecx, 8

convert_loop:
    mov edx, eax
    and edx, 0Fh
    mov dl, [hex_chars + edx]
    mov [edi], dl
    shr eax, 4
    dec edi
    dec ecx
    jnz convert_loop

    mov eax, 4
    mov ebx, 1
    mov ecx, output
    mov edx, 8
    int 80h

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 80h

    mov eax, 1
    xor ebx, ebx
    int 80h