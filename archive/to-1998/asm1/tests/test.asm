.model tiny
.stack 100h

code segment 'Code'
assume cs:code

    int 18h

    mov  ax,4C00h
    int  21h

code ends

end
