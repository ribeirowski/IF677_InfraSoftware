org 0x7c00
jmp main

main:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov bx, ax
	
	mov ax, 0x50 ;coloca endereço de boot2 / 16 em 'es'
	mov es, ax ; ax * 16
	
	call reset
	call load
	jmp 0x500 ;pula pro boot2
	
reset:
	mov ah, 00h ;reseta sistema de disco
	mov dl, 0   ;floppy
	int 13h     ;retorna carry 1 se der certo e 0 se der errado
	jc reset    ;se der errado tenta denovo
	ret
load:
	mov ah, 02h ;lê um setor do disco
	mov al, 1   ;quantidade de setores ocupados pelo boot2
	mov ch, 0   ;track 0
	mov cl, 2   ;sector 2
	mov dh, 0   ;head 0
	mov dl, 0   ;drive 0
	int 13h
	jc load
	ret
	
times 510-($-$$) db 0 ;512 bytes
dw 0xaa55             ;assinatura
