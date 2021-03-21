section .text

btc_call_mprotect:
	sub rsp, 0x10
	mov DWORD [rsp], 0x0		; addr 2
	mov DWORD [rsp + 0x4], 0x0	; addr 1
	mov rdi, QWORD [rsp]
	mov DWORD [rsp + 0x8], 0x0	; len 2
	mov DWORD [rsp + 0xc], 0x0	; len 1
	mov rsi, QWORD [rsi]
	xor rdx, rdx
	mov rdx, 0x03
	xor rax, rax
	mov rax, 0xa
	syscall
	add rsp, 0x10