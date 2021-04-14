section .text

btc_def_find_abs_vaddr:
	
	push rbp
	mov rbp, rsp

	lea rcx, [rel $ - 0x4]				; puts the address of the function in rcx
	
	sub rsp, 0x8
	mov DWORD [rsp], 0xaabbccdd			; fun vaddr 2
	mov DWORD [rsp + 0x4], 0xccddeeff	; fun vaddr 2
	sub rcx, QWORD [rsp]				; compute the the absolute addr for vaddr = 0
										; by subtracting rip vaddr to rip value
	mov rax, rdi
	add rax, rcx						; computes the absolute address

	add rsp, 0x8

	leave								; mov rsp rbp + pop rbp
	ret									; pop rip