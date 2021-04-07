GLOBAL syscalls:

section .text

%macro push_registers 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15		
%endmacro

%macro	pop_registers 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

syscalls:
    push rbp
    mov rbp, rsp
	push_registers

    mov rax,rdi
    mov rdi, rsi
    mov rsi,rdx
    mov rdx,rcx
    mov r10,r8
    mov r8,r9
	mov r9,[rbp+16]
    int 80h

	pop_registers
	mov rsp, rbp
    pop rbp

    ret