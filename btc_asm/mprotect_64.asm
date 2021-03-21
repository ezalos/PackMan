section .text

btc_mprotect:

	mov rdi, 0x1111111111111111
	mov rsi, 0x2222222222222222
	mov rdx, 0x03
	mov rax, 0xa
	syscall