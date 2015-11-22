.model tiny
.stack 100h

.code

mov  ax, 0013h
int  10h

mov  ax, 0A000h
mov  ds, ax

mov  al, 0
mov  cx, 1000
outerloop:

    mov  bx, 64000
    loopbacktohere:

        mov  [bx], cl

        dec  bx
        jnz  loopbacktohere

    dec  cx
    jnz  outerloop

mov  ah, 4Ch
int  21h

end
