section .text

btc_decrypt_zone:
	mov rdi, 0x11111111				; zone
	mov rsi, 0x12345678				; len
	lea rdx, [rsp]					; permutations
	call 0x22222222					; call btc_def_crypt 