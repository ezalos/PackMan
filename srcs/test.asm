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

; void	ft_cypher(char *data, int len, char key);
; ARG1 is RDI
; ARG2 is RSI
; ARG3 is RDX

section .text

plop:
	push rbp				; stack += rbp
	mov rbp, rsp			; rbp = rsp
	; mov rip, 0x11223344			; rbp = rsp

	jmp 0x1122334455667788
    ret
