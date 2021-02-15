; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_cypher.asm                                      :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2020/12/05 14:12:13 by ezalos            #+#    #+#              ;
;    Updated: 2020/12/22 13:42:47 by ezalos           ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

global ft_cypher ; C function
; void	ft_cypher(char *data, int len, char key);
; ARG1 is RDI
; ARG2 is RSI
; ARG3 is RDX

section .text

ft_cypher:
	push rbp				; stack += rbp
	mov rbp, rsp			; rbp = rsp

	cmp RSI, 0
	jnz crypt_loop
	jmp end

crypt_loop:
	cmp RSI, -1
	jz end
	lea R10, [RDI + RSI]
	xor BYTE [R10], DL
	dec RSI
	jmp crypt_loop

end:
	mov rsp, rbp
    pop rbp
	jmp 0x1122334455667788
    ret
