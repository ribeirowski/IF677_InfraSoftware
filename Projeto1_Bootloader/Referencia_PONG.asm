        org 7C00h                       ; Bootloader

main:
        setup_video_mode:
		mov ax, 13h                     ; 320 x 200 x 8
		int 10h

	setup_memory_offset:
		mov ax, Screen.addr
		mov es, ax
        match_start:
		neg word [Ball.xs]              ; Invert horizontal direction
		mov [Ball.x], word Ball.x0      ; Ball at starting position
		mov [Ball.y], word Ball.y0

	game_loop:
		update_player:
		    .input:
			in al, Keyboard.port            ; Read keyboard directly
			cmp al, Keyboard.w              ; Up
			je .move_up
			cmp al, Keyboard.s              ; Down
			je .move_down
			jmp .end1                        ; Still
		    .move_up:
			mov ax, [P1.y]                  ; Top of paddle
			cmp ax, 0                       ; Top of screen collision
			je .end1
			dec word [P1.y]                 ; Move up
			jmp .end1
		    .move_down:
			mov ax, [P1.y]                  ; Top of paddle
			add ax, P1.h                    ; Add size of paddle
			cmp ax, Screen.h                ; Bottom of screen collision
			je .end1
			inc word [P1.y]                 ; Move down
		    .end1:

		update_ai:
		    .think:
			mov cx, [Ball.x]                ; Only move if ball is within reach
			cmp cx, P2.ai_reach
			jle .end2
			mov cx, [P2.y]                  ; Compare to ball position
			cmp cx, [Ball.y]                ; Top of right paddle
			jg .move_up                     ; Paddle too low
			add cx, P2.h
			cmp cx, [Ball.y]                ; Bottom of right paddle
			jl .move_down                   ; Paddle too high
			jmp .end2
		    .move_up:
			dec word [P2.y]
			jmp .end2
		    .move_down:
			inc word [P2.y]
		    .end2:

		update_ball_x:
			mov ax, [Ball.x]
			mov bx, [Ball.xs]
			add ax, bx                      ; Calculate next position
		    .collide_screen_left:
			jnz .collide_screen_right       ; Left of screen collision - Win
			inc byte [P2.score.v]           ; Inc score
			cmp [P2.score.v], byte Match.max_score
			jge game_over                   ; Game over
			jmp match_start                 ; Restart match
		    .collide_screen_right:
			cmp ax, Screen.w
			jne .collide_p1                 ; Right of screen collision - Win
			inc byte [P1.score.v]           ; Inc score
			cmp [P1.score.v], byte Match.max_score
			jge game_over                   ; Game over
			jmp match_start                 ; Restart match
		    .collide_p1:
			cmp ax, P1.x                    ; Left paddle collision
			jne .collide_p2
			mov cx, [P1.y]
			cmp cx, [Ball.y]                ; Top of left paddle collision
			jg .collide_p2
			add cx, P1.h
			cmp cx, [Ball.y]                ; Bottom of left paddle collision
			jge .bounce
		    .collide_p2:
			cmp ax, P2.x                    ; Right paddle collision
			jne .apply
			mov cx, [P2.y]
			cmp cx, [Ball.y]                ; Top of right paddle collision
			jg .apply
			add cx, P2.h
			cmp cx, [Ball.y]                ; Bottom of right paddle collision
			jl .apply
		    .bounce:
			neg bx                          ; Invert direction
			mov [Ball.xs], bx
		    .apply:
			mov [Ball.x], ax                ; Apply speed
		    .end:

		update_ball:
			mov ax, [Ball.y]
			mov bx, [Ball.ys]
			add ax, bx                      ; Top of screen collision
			jz .bounce
			cmp ax, Screen.h                ; Bottom of screen collision
			jne .apply
		    .bounce:
			neg bx                          ; Invert direction
			mov [Ball.ys], bx
		    .apply:
			mov [Ball.y], ax                ; Apply speed
		    .end:
		clear_screen:
			mov cl, Screen.c                ; Clear screen to bg color
			mov di, Screen.w * Screen.h
		    .loop:
			mov [es:di], cl
			dec di
			jnz .loop

		draw_net:
			mov ax, Net.y                   ; Y
			mov bx, Net.x                   ; X
			mov cx, Net.h                   ; Height
			mov dl, Net.c                   ; Color
		    .loop:
			call Plot                       ; Plot
			add ax, 2                       ; Dotted line
			loop .loop                      ; Loop to height of net

		draw_scores:
		    .p1:
			mov dl, P1.score.x              ; X
			mov dh, P1.score.y              ; Y
			mov bl, P1.score.c              ; Color
			mov al, [P1.score.v]            ; Score value
			add al, 48                      ; Shift to `0` character
			call PlotChar                   ; Plot char
		    .p2:
			mov dl, P2.score.x              ; X
			mov dh, P2.score.y              ; Y
			mov bl, P2.score.c              ; Color
			mov al, [P2.score.v]            ; Score value
			add al, 48                      ; Shift to `0` character
			call PlotChar                   ; Plot char

		draw_p1:
			mov ax, [P1.y]                  ; Y
			mov bx, P1.x                    ; X
			mov cx, P1.h                    ; Height
			mov dl, P1.c                    ; Color
		    .loop:
			call Plot                       ; Plot
			inc ax                          ; Next row
			loop .loop                      ; Loop to size of paddle

		draw_p2:
			mov ax, [P2.y]                  ; Y
			mov bx, P2.x                    ; X
			mov cx, P2.h                    ; Height
			mov dl, P2.c                    ; Color
		    .loop:
			call Plot                       ; Plot
			inc ax                          ; Next row
			loop .loop                      ; Loop to size of paddle

		draw_ball:
			mov ax, [Ball.y]                ; Y
			mov bx, [Ball.x]                ; X
			mov dl, Ball.c                  ; Color
			call Plot                       ; Plot
	game_spin:
		mov ah, 86h                     ; Elapsed time wait call
		mov cx, 0                       ; Delay
		mov dx, Screen.frame_delay      ; Delay
		int 15h                         ; System BIOS call
		jmp game_loop

	game_over:
		mov di, GameOver.s              ; String pointer
		mov cx, GameOver.l              ; String index from end
		mov dl, GameOver.x              ; X
		mov dh, GameOver.y              ; Y
		mov bl, GameOver.c              ; Color
	    .print:
		mov al, [di]                    ; Get actual char
		push cx
		call PlotChar                   ; Plot char
		inc dl                          ; One x forward
		inc di                          ; One char forward
		pop cx
		loop .print                     ; Loop until string end
	    .spin:
		jmp .spin                       ; Spin forever
		
        Plot:
		; ax=y, bx=x, dl=color
		push ax
		imul ax, 320
		add ax, bx
		mov di, ax
		mov [es:di], dl
		pop ax
		ret

	PlotChar:
		; al=char bl=color dl=x dh=y
		cmp al, Keyboard.space          ; Don't draw spaces
		je .end
		mov bh, 0                       ; Page zero
		push ax
		push bx
		mov ax, 0200h                   ; Move cursor
		int 10h
		pop bx
		pop ax
		mov ah, 0Ah                     ; Plot char
		mov cx, 1                       ; Repeat once
		int 10h
	    .end:
		ret
data:
        Keyboard:
	    .port         equ 60h
	    .w            equ 11h
	    .s            equ 1Fh
	    .space        equ 20h

	Screen:
	    .frame_delay  equ 8192
	    .addr         equ 0A000h
	    .w            equ 320
	    .h            equ 200
	    .c            equ 0

	Match:
	    .max_score    equ 10

	Net:
	    .x:           equ Screen.w / 2
	    .y:           equ 1
	    .h:           equ (Screen.h - 2) / 2
	    .c:           equ 2

	Ball:
	    .x0           equ Screen.w / 2
	    .y0           equ Screen.h / 2
	    .x            dw .x0
	    .y            dw .y0
	    .xs           dw 1
	    .ys           dw 1
	    .c            equ 15

	P1:
	    .x            equ 23
	    .y            dw (Screen.h - .h) / 2
	    .h            equ 25
	    .c            equ 12
	    .score.v      db 0
	    .score.x      equ 1
	    .score.y      equ 24 / 2
	    .score.c      equ 4

	P2:
	    .x            equ Screen.w - 23
	    .y            dw (Screen.h - .h) / 2
	    .h            equ 25
	    .c            equ 9
	    .ai_reach     equ 223
	    .score.v      db 0
	    .score.x      equ 38
	    .score.y      equ 24 / 2
	    .score.c      equ 1

	GameOver:
	    .s            db "GAME  OVER"
	    .l            equ $ - .s
	    .x            equ (40 - .l) / 2
	    .y            equ 24 / 2
	    .c            equ 10

padding:
        %assign compiled_size $-$$
        %warning Compiled size: compiled_size bytes

        times 0200h-2-compiled_size  db 0       ; Zerofill up to 510 bytes
        dw 0AA55h                               ; Boot Sector signature