org 0x500
jmp main

data:
	msg1 db "Rodaaaaaaando...", 0
	msg2 db "Weeeeeeeeeeee...", 0
	msg3 db "Paro de brincadeira, VAMO INICIAR!!", 0
main:
	xor ax, ax
	mov es, ax
	mov ds, ax
	mov bx, ax
	
	mov ax, 0x7e0 ;coloca o endereço do kernel / 16 em 'es'
	mov es, ax ; ax * 16
	
	mov si, msg1
	call string
	mov si, msg2
	call string
	mov si, msg3
	call string
	
reset:
	mov ah, 00h ;reseta sistema de disco
	mov dl, 0   ;floppy
	int 13h     ;retorna carry 1 se der certo e 0 se der errado
	jc reset    ;se der errado tenta denovo

load:
	mov ah, 02h ;lê um setor do disco
	mov al, 60   ;quantidade de setores ocupados pelo kernel
	mov ch, 0   ;track 0
	mov cl, 3   ;sector 3
	mov dh, 0   ;head 0
	mov dl, 0   ;drive 0
	int 13h
	jc load
	jmp 0x7e00 ;pula pro kernel
string:
	loop:
		lodsb ;coloca de si em al
		cmp al, 0
		je end
		call print
		jmp loop
	end:
		mov al, 10
		call print
		mov al, 13
		call print
		ret
print:
	mov ah, 0xe
	int 10h
	ret
	
times 510-($-$$) db 0 ;512 bytes
dw 0xaa55	
