section .text
	global init_permutations_asm
	global schedule_key_asm
	global crypt_zone_asm


init_permutations_asm:
	push rbp
	mov rbp, rsp

	mov rcx, 0xff			; initialize counter to 255

	_fill_permutations:
	mov [rdi + rcx], cl		; put the value of cl in permutations
	loop _fill_permutations ; decrements cl until = 0, then stops looping
	
	mov [rdi], cl

	leave
	ret

schedule_key_asm:
; rdi is the permutation
; rsi is the key
; rcx is the equivalent for i
; rdx is the equivalent for j
; al and ah is used as a tmp to compute more complex values or to swap values
; r9 takes tmp values to compute j
	push rbp
	mov rbp, rsp

	xor rcx, rcx
	xor rdx, rdx
	_do_permutations:

	xor rax, rax
	mov al, cl
	and al, 0x7					; computes index in the key; !!careful!! for now it is modulo (8 - 1), if we want to change the key lenght it has to be a power of 2
	mov al, BYTE [rsi + rax]		; finds the value at the index computed above

	add dl, al					; adds the value computed above to j

	mov al, BYTE [rdi + rcx]	; finds the value at the index i in permutations

	add dl, al					; add it to j

	mov al, BYTE [rdi + rcx]	; 4 lines to swap values
	mov ah, BYTE [rdi + rdx]
	mov BYTE [rdi + rcx], ah
	mov BYTE [rdi + rdx], al

	inc rcx
	cmp rcx, 0xff
	jle _do_permutations

	leave
	ret


crypt_zone_asm:
; rdi: zone
; rsi: len
; rdx : permutations
; r8: index
; bl: i
; cl: j
; al and ah: tmp to swap values + k

	push rbp
	mov rbp, rsp

	push rbx					; save the value of rbx as it is a preserved register

	xor r8, r8
	xor rbx, rbx
	xor rcx, rcx
	xor rax, rax

	_do_cypher:
	inc bl						; i = (i + 1) mod 256
	add cl, [rdx + rbx]			; j = (j + permutations[i]) mod 256

	mov al, BYTE [rdx + rbx]	; 4 lines to swap permutations[i] and permutations[j]
	mov ah, BYTE [rdx + rcx]
	mov BYTE [rdx + rbx], ah
	mov BYTE [rdx + rcx], al

	xor rax, rax

	mov al, BYTE [rdx + rbx]
	add al, BYTE [rdx + rcx]
	mov al, BYTE [rdx + rax]

	xor [rdi + r8], rax

	inc r8
	cmp r8, rsi
	jne _do_cypher

	pop rbx
	leave
	ret
