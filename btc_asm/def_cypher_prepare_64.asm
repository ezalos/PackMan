section .text

btc_def_cypher_prepare:

	sub rsp, 0x108						; allocating space for key and perm tab
	mov DWORD [rsp + 0x100], 0x0		; key values 1
	mov DWORD [rsp + 0x104], 0x0		; key values 2
	lea rdi, [rsp]
	call 0x0							; call btc_init_perm
	lea rdi, [rsp]
	lea rdi, [rsp + 0x100]
	call 0x0							; call btc_key_sched