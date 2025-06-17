section .data
    format db "%d", 10, 0    ; format string for printf (%d and newline)
    limit equ 100000         ; upper limit for prime numbers

section .bss
    sieve resb limit        ; reserve bytes for the sieve array

section .text
    global main
    extern printf

main:
    push ebp
    mov ebp, esp

    ; Initialize sieve array (0 means prime, 1 means composite)
    mov ecx, limit          ; counter
    mov edi, sieve         ; destination for stosb
    xor al, al            ; fill with zeros
    rep stosb             ; fill memory

    ; Mark 0 and 1 as non-prime
    mov byte [sieve + 0], 1
    mov byte [sieve + 1], 1

    ; Implement Sieve of Eratosthenes
    mov ebx, 2            ; start with 2

sieve_loop:
    cmp ebx, limit
    jge print_primes

    ; Check if current number is prime
    movzx eax, byte [sieve + ebx]
    test al, al
    jnz next_number

    ; Mark multiples as composite
    mov ecx, ebx          ; counter starts at current prime
    add ecx, ebx          ; first multiple

mark_multiples:
    cmp ecx, limit
    jge next_number

    mov byte [sieve + ecx], 1    ; mark as composite
    add ecx, ebx                 ; next multiple
    jmp mark_multiples

next_number:
    inc ebx
    jmp sieve_loop

print_primes:
    ; Print all prime numbers
    mov ebx, 2            ; start with 2

print_loop:
    cmp ebx, limit
    jge done

    movzx eax, byte [sieve + ebx]
    test al, al
    jnz skip_print

    ; Print prime number
    push ebx
    push format
    call printf
    add esp, 8

skip_print:
    inc ebx
    jmp print_loop

done:
    mov esp, ebp
    pop ebp
    xor eax, eax         ; return 0
    ret