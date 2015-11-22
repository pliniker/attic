BITS 32


segment code use16

..start mov  ax, code
        mov  ds, ax
        mov  ss, ax
        mov  sp, stacktop

        mov  dx, string
        mov  ah, 09h
        int  21h

        mov  ah, 4Ch
        int  21h

string: db 'Hello world.',13,10,'$'

        resb 64
stacktop:

segment ends

