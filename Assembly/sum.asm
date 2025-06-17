section .data
    input_buf    db 20          ; Buffer to store input
    input_len    equ $ - input_buf

    output_buf   db 20          ; Buffer to store output

section .bss

section .text
    global _start

_start:
    ; Read input from stdin
    mov eax, 3                  ; sys_read system call
    mov ebx, 0                  ; file descriptor (stdin)
    mov ecx, input_buf          ; buffer to store input
    mov edx, 20                 ; maximum number of bytes to read
    int 0x80

    mov esi, input_buf          ; pointer to input buffer
    mov ecx, eax                ; number of bytes read
    mov ebx, 0                  ; sum initialized to 0

convert_and_sum:
    cmp byte [esi], 10          ; check for newline character
    je calculate_done
    cmp byte [esi], 0           ; check for null terminator
    je calculate_done

    cmp byte [esi], '0'         ; check if character >= '0'
    jl skip_char
    cmp byte [esi], '9'         ; check if character <= '9'
    jg skip_char

    sub byte [esi], '0'         ; convert ASCII to integer
    add bl, [esi]               ; add to sum

skip_char:
    inc esi                     ; move to next character
    loop convert_and_sum

calculate_done:
    ; Convert sum to string
    mov eax, ebx                ; move sum to eax
    xor ebx, ebx                ; clear ebx
    mov edi, output_buf + 19    ; pointer to end of output buffer
    mov byte [edi], 0           ; null terminator
    dec edi

convert_to_string:
    xor edx, edx
    mov ebx, 10
    div ebx                     ; divide eax by 10
    add edx, '0'                ; convert remainder to ASCII
    mov [edi], dl               ; store character
    dec edi
    cmp eax, 0
    jne convert_to_string

    inc edi                     ; adjust pointer to start of string

    ; Write output to stdout
    mov eax, 4                  ; sys_write system call
    mov ebx, 1                  ; file descriptor (stdout)
    mov ecx, edi                ; pointer to output buffer
    mov edx, output_buf + 20    ; calculate length
    sub edx, ecx
    int 0x80

    ; Exit program
    mov eax, 1                  ; sys_exit system call
    xor ebx, ebx                ; exit code 0
    int 0x80