section .text

btc_decrypt_zone:
	mov rdi, 0x1111111111111111			; zone
	mov rsi, 0x1234567812345678			; len
	lea rdx, [rsp]						; permutations
	call 0x2222222222222222				; call btc_def_crypt 