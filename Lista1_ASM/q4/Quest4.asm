org 0x7c00
jmp main
data:
	string: times 10 db 0
main:
	xor ax, ax
	mov di, string
	call scanf	
	mov si, string
	call inteiro
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
		cmp cl, 0x2c
		je dinheiro                   ;reais
		sub cl, '0'
		mov bx, 10
		mul bx
		add ax, cx
		jmp loop2
	endlp2:
		mov dx, ax
		xor ax, ax
		mov cx, ax
		jmp lp_mei                    ;centavos
	dinheiro:
		mov dx, ax
		xor ax, ax
		mov cx, ax	
		lp_cem:   ; 100
			cmp dx, 100
			jb zera0
			inc cl
			sub dx, 100
			jmp lp_cem
		zera0:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line	
		lp_cqt:   ; 50
			cmp dx, 50
			jb zera1
			inc cl
			sub dx, 50
			jmp lp_cqt
		zera1:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line
		lp_vin:   ;20
			cmp dx, 20
			jb zera2
			inc cl
			sub dx, 20
			jmp lp_vin
		zera2:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line
		lp_dez:   ;10
			cmp dx, 10
			jb zera3
			inc cl
			sub dx, 10
			jmp lp_dez
		zera3:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line			
		lp_cin:   ;5
			cmp dx, 5
			jb zera4
			inc cl
			sub dx, 5
			jmp lp_cin
		zera4:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line	
		lp_dos:   ;2
			cmp dx, 2
			jb zera5
			inc cl
			sub dx, 2
			jmp lp_dos
		zera5:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line		
		lp_uno:   ;1
			cmp dx, 1
			jb zera6
			inc cl
			sub dx, 1
			jmp lp_uno
		zera6:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line
			jmp inteiro		
		lp_mei:   ;0,5           CENTAVOS
			cmp dx, 50
			jb zera7
			inc cl
			sub dx, 50
			jmp lp_mei
		zera7:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line		
		lp_vintcinc:   ;0,25
			cmp dx, 25
			jb zera8
			inc cl
			sub dx, 25
			jmp lp_vintcinc	
		zera8:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line
		lp_ten:   ;0,10
			cmp dx, 10
			jb zera9
			inc cl
			sub dx, 10
			jmp lp_ten
		zera9:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line	
		lp_five:   ;0,05
			cmp dx, 5
			jb zera10
			inc cl
			sub dx, 5
			jmp lp_five
		zera10:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
			call end_line
		lp_ichi:   ;0,01
			cmp dx, 1
			jb zera11
			inc cl
			sub dx, 1
			jmp lp_ichi
		zera11:
			mov al, cl
			add al, '0'
			call print
			xor cx, cx
		end:
			ret	
times 510 - ($-$$) db 0
dw 0xaa55
