section .text
write_woody:
	sub rsp, 0x10
	mov DWORD[rsp], 0x2e2e2e2e
	mov DWORD[rsp + 0x4], 0x444f4f57
	mov DWORD[rsp + 0x8], 0x2e2e2e59
	mov DWORD[rsp + 0xc], 0x00000a2e
	mov rdi, 0x1
	mov rsi, rsp
	mov rdx, 0xe
	mov rax, 0x1
	syscall
	add rsp, 0x10
