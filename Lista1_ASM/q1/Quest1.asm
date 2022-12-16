org 0x7c00
jmp main

data:
	cor db 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 8, 7, 8, 9, 9, 9, 1, 1, 1, 1, 9, 1, 0, 0, 0, 0, 8, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 1, 0, 0, 0, 0, 8, 7, 8, 9, 9, 9, 15, 15, 9, 9, 9, 1, 0, 0, 0, 0, 8, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 1, 0, 0, 0, 0, 8, 7, 8, 9, 15, 15, 15, 9, 9, 9, 9, 1, 0, 0, 0, 0, 8, 7, 8, 9, 9, 9, 15, 15, 9, 9, 9, 1, 0, 0, 0, 0, 8, 7, 8, 1, 1, 9, 9, 9, 9, 1, 1, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	
main:
	xor ax, ax
	mov ds, ax
	mov cx, ax
	mov dx, ax
	
	call video
	mov si, cor
	
	loop1:
		cmp dx, 16
		je fim_lp1
		mov cx, 0
		loop2:
			cmp cx, 16
			je fim_lp2
			lodsb ; al = 0/
			mov ah, 0xc
			int 10h
			inc cx
			jmp loop2
			
		fim_lp2:
			inc dx
			jmp loop1
		
	fim_lp1:
		jmp $
		
video:
	mov ah, 0
	mov al, 13h
	int 10h
	ret
	
times 510-($-$$) db 0
dw 0xaa55
