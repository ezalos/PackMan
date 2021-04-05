section .text

btc_call_cypher:
	sub rsp, 0x10
	mov DWORD [rsp], 0xffffffff			; zone 2
	mov DWORD [rsp + 0x4], 0xffffffff	; zone 1
	mov rdi, QWORD [rsp]				; zone
	call 0x0							; call btc_def_find_abs_vaddr
	mov rdi, rax
	mov DWORD [rsp + 0x8], 0xffffffff	; len 2
	mov DWORD [rsp + 0xc], 0xffffffff	; len 1
	mov rsi, QWORD [rsp + 0x8]			; len
	lea rdx, [rsp + 0x10]				; permutations
	call 0x0							; call btc_def_crypt
	add rsp, 0x10