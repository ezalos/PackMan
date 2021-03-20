section .text

btc_decrypt_prepare:

	sub rsp, 0x108					; allocating space for key and perm tab
	mov [rsp + 0x100], 0x12345678	; key value
	lea rdi, [rsp]
	call 0x11111111					; call btc_init_perm
	lea rdi, [rsp]
	lea rdi, [rsp + 0x100]
	call 0x22222222					; call btc_key_sched
