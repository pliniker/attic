
mov  ah,09h
mov  dx,offset msg
int  21h

x:
jmp x

msg db 'Hello!$'
