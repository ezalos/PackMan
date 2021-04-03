section .text

btc_def_cypher_prepare:

	sub rsp, 0x108						; allocating space for key and perm tab
	mov DWORD [rsp + 0x100], 0xffffffff	; key values 1
	mov DWORD [rsp + 0x104], 0xffffffff	; key values 2
	lea rdi, [rsp]
	call 0xffeeddcd						; call btc_init_perm
	lea rdi, [rsp]
	lea rsi, [rsp + 0x100]
	call 0xffeeddcd						; call btc_key_sched
