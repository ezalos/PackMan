section .text

btc_mprotect:

	mov rdi, 0x11111111
	mov rsi, 0x22222222
	mov rdx, 0x03
	mov rax, 0xa
	syscall