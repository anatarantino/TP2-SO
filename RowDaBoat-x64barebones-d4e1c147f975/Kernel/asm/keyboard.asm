GLOBAL get_key
GLOBAL pressed_key

section .text

get_key:
	push rbp
	mov rbp,rsp

	mov rax,0
	in al,60h ;a donde redirecciono

	mov rsp,rbp
	pop rbp

	ret

pressed_key: ;hasKey
	push rbp
	mov rbp,rsp

	mov rax,0
	in al,64h ;64 es el teclado
	and al,0x01

	mov rsp,rbp
	pop rbp
	ret
