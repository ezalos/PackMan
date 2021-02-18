section .text
;Le pbm vient peut etre de l'absence d'un \0 dans le write. https://stackoverflow.com/questions/16403193/debugging-a-segfault-program-always-does-as-it-should-but-always-segfaults
; Non je ne pense pas que ce soit ca, mais c'est ma seule piste actuellement
; tu penses ? car on limite l'ecriture a 4 char precedemment et 14 desormais
; je fais un test sans juste pour ecrire correctement le truc et ensuite je regarde de ce cote
; ok, merci

ft_write:
	push rdi
	push rsi
	push rdx
	push rax


	push 0x2e2e2e2e
	mov rsi, rsp
	mov rdi, 0x1
	mov rdx, 0x4
	mov rax, 0x1
	syscall

	push 0x444f4f57
	mov rsi, rsp
	mov rdi, 0x1
	mov rdx, 0x4
	mov rax, 0x1
	syscall

	push 0x2e2e2e59
	mov rsi, rsp
	mov rdi, 0x1
	mov rdx, 0x4
	mov rax, 0x1
	syscall

	push 0x00000a2e
	mov rsi, rsp
	mov rdi, 0x1
	mov rdx, 0x2
	mov rax, 0x1
	syscall

	;push 0x0a746c53
	;add rsp, 0x10
	pop rax
	pop rax
	pop rax
	pop rax

	pop rax
	pop rdx
	pop rsi
	pop rdi