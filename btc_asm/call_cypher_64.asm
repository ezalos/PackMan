section .text

btc_call_cypher:
	sub rsp, 0x10
	mov DWORD [rsp], 0x0				; zone 2
	mov DWORD [rsp + 0x4], 0x0			; zone 1
	mov rdi, QWORD [rsp]				; zone
	mov DWORD [rsp + 0x8], 0x0			; len 2
	mov DWORD [rsp + 0xc], 0x0			; len 1
	mov rsi, QWORD [rsp + 0x8]			; len
	lea rdx, [rsp + 0x10]				; permutations
	call 0x0							; call btc_def_crypt
	add rsp, 0x10