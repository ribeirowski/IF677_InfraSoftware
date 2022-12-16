org 0x7c00
jmp main

data:
	string: times 50 db 0
	numero: times 20 db 0
main:
	xor ax, ax
	mov bx, ax
	mov cx, ax
	mov dx, ax
	push ax
	mov di, numero
	call scanf
	mov si, numero
	call inteiro
	mov dl, al             ; dl = numero de repetições, dh = contador
	mov dh, 0
	loop0:
		loop3:
			pop ax
			cmp ax, 0
			je endlp3
			jmp loop3
		endlp3:
			xor ax, ax
			push ax
		cmp dh, dl
		je end
		inc dh
		mov di, string
		call scanf
		mov si, string
		xor ax, ax
		mov bx, ax
		jmp search
	end:
		jmp $

apagar:
	mov al, 0x8
	call print
	mov al, ' '
	call print
	mov al, 0x8
	call print
	ret	

print:
	mov ah, 0xe
	int 10h
	ret	
	
end_line:
	mov al, 0xa
	call print
	mov al, 0xd
	call print
	ret
	
scanf:
	xor cx, cx
	loop1:
		mov ah, 0
		int 16h
		cmp al, 0x8
		je delete
		cmp al, 0xd
		je enter
		inc cl
		stosb
		call print
		jmp loop1
	delete:
		cmp cl, 0
		je loop1
		dec cl
		dec di
		mov byte[di], 0
		call apagar
		jmp loop1
	enter:
		mov al, 0
		stosb
		call end_line
		ret
		
inteiro:
	xor ax, ax
	mov cx, ax
	loop2:
		push ax
		lodsb
		mov cl, al
		pop ax
		cmp cl, 0
		je endlp2
		sub cl, '0'
		mov bx, 10
		mul bx
		add al, cl
		jmp loop2	
	endlp2:
		ret
		
empilha:
	push ax
	jmp search

inicia_pilha:
	xor ax, ax
	push ax
	jmp fail
	
search:
	lodsb
	cmp al, 0
	je end_search
	cmp al, '('
	je empilha
	cmp al, '{'
	je empilha
	cmp al, '['
	je empilha
	cmp al, ')'
	je verifica1
	cmp al, '}'
	je verifica2
	cmp al, ']'
	je verifica3
	xor ax, ax
	jmp search
	verifica1:
		pop ax
		cmp ax, '('
		je search
		cmp ax, 0
		je inicia_pilha
		jmp fail
		
	verifica2:
		pop ax
		cmp ax, '{'
		je search
		cmp ax, 0
		je inicia_pilha
		jmp fail
		
	verifica3:
		pop ax
		cmp ax, '['
		je search
		cmp ax, 0
		je inicia_pilha
		jmp fail
	fail:
		xor ax, ax
		push ax
		mov al, 'N'
		call print
		call end_line
		jmp loop0
end_search:
	pop ax
	cmp ax, 0
	jne fail
	push ax
	mov al, 'S'
	call print
	call end_line
	jmp loop0
		
		
times 510 - ($-$$) db 0
dw 0xaa55
