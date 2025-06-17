; Matrix operations program
; For x86_64 Linux

section .data
    ; 3x3 matrix stored as 9 consecutive integers
    matrix  dd  1, 2, 3,     ; First row
            dd  4, 5, 6,     ; Second row
            dd  7, 8, 9      ; Third row

    msg1    db  "Sum of elements: ", 0
    msg1_len equ $ - msg1
    msg2    db  10, "Sum of diagonal: ", 0    ; 10 is newline
    msg2_len equ $ - msg2
    newline db  10                            ; newline character

section .bss
    sum_str     resb 20      ; Buffer for number string
    diag_str    resb 20      ; Buffer for diagonal sum string

section .text
    global _start

_start:
    ; Calculate sum of all elements
    xor rax, rax            ; rax will hold total sum
    xor rbx, rbx            ; rbx will hold diagonal sum
    xor rcx, rcx            ; counter for elements

sum_loop:
    cmp rcx, 9              ; check if we've processed all 9 elements
    je print_results

    ; Add current element to total sum
    mov edx, dword [matrix + rcx*4]
    add eax, edx

    ; Check if element is on diagonal (positions 0, 4, and 8)
    mov r9, rcx
    cmp r9, 0
    je diagonal
    cmp r9, 4
    je diagonal
    cmp r9, 8
    je diagonal
    jmp not_diagonal

diagonal:
    add ebx, edx

not_diagonal:
    inc rcx
    jmp sum_loop

print_results:
    ; Save the sums
    push rax                ; Save total sum
    push rbx                ; Save diagonal sum

    ; Print "Sum of elements: "
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, msg1          ; message
    mov rdx, msg1_len      ; length
    syscall

    ; Convert and print total sum
    pop rbx                 ; Restore diagonal sum
    pop rax                 ; Restore total sum
    push rbx                ; Save diagonal sum again
    push rax                ; Save total sum again
    call number_to_string

    ; Print total sum
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, sum_str       ; converted number string
    mov rdx, r12           ; length returned by conversion
    syscall

    ; Print "Sum of diagonal: "
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, msg2          ; message
    mov rdx, msg2_len      ; length
    syscall

    ; Convert and print diagonal sum
    pop rax                 ; Discard total sum
    pop rax                 ; Get diagonal sum
    call number_to_string

    ; Print diagonal sum
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, sum_str       ; converted number string
    mov rdx, r12           ; length returned by conversion
    syscall

    ; Print final newline
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

    ; Exit program
    mov rax, 60             ; sys_exit
    xor rdi, rdi            ; status 0
    syscall

; Function to convert number to string
; Input: RAX = number to convert
; Output: string in sum_str, length in R12
number_to_string:
    mov rsi, sum_str
    mov r12, 0              ; string length counter
    mov r13, 10            ; divisor

    ; Handle zero case
    test rax, rax
    jnz convert_loop_start
    mov byte [rsi], '0'
    mov byte [rsi + 1], 0
    mov r12, 1
    ret

convert_loop_start:
    mov rdi, rsi           ; save start of buffer

convert_loop:
    xor rdx, rdx           ; clear dividend high bits
    div r13                ; divide by 10
    add dl, '0'            ; convert remainder to ASCII
    mov [rsi], dl          ; store ASCII digit
    inc rsi                ; move buffer pointer
    inc r12                ; increment length
    test rax, rax          ; check if more digits
    jnz convert_loop       ; continue if more digits

    mov byte [rsi], 0      ; null terminate

    ; Reverse the string
    mov rcx, r12           ; length for reversing
    mov rsi, rdi           ; start of buffer
    dec rcx                ; adjust for zero-based index

reverse_loop:
    cmp rcx, 0
    jl reverse_done
    mov al, [rsi]
    mov dl, [rsi + rcx]
    mov [rsi + rcx], al
    mov [rsi], dl
    inc rsi
    dec rcx
    dec rcx
    jns reverse_loop

reverse_done:
    ret