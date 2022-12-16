org 0x7c00
jmp main

data:
	string: times 100 db 0	
main:
	xor ax, ax
	mov bx, ax
	mov di, string
	call scanf
	mov si, string
	call print_string
	jmp $
end_line:
	mov al, 0xa
	call print
	mov al, 0xd
	call print
	ret	
delete:
	mov al, 0x8
	call print
	mov al, ' '
	call print
	mov al, 8h
	call print
	ret	
print:
	mov ah, 0xe
	int 10h
	ret	
scanf:
	xor cx, cx
	loop1:
		mov ah, 0
		int 16h
		cmp al, 0x8
		je apagar
		cmp al, 0xd
		je enter
		inc cl
		stosb
		call print
		jmp loop1
	apagar:
		cmp cl, 0
		je loop1
		dec di
		dec cl
		mov byte[di], 0
		call delete
		jmp loop1
	enter:
		mov al, 0
		stosb
		call end_line
		ret
print_string:
	loop2:
		lodsb
		cmp al, 0
		je end
		cmp bl, 0
		je big
		jmp small
		big:
			cmp al, 32
			je space
			mov bl, 1
			sub al, 32
			call print
			jmp loop2	
		small:
			cmp al, 32
			je space
			mov bl, 0
			call print
			jmp loop2
		space:
			call print
			jmp loop2
	end:
		ret
		
times 510 - ($-$$) db 0
dw 0xaa55
