
.MODEL TINY
.STACK 100h

Code    SEGMENT 'CODE' USE16
        ASSUME cs:Code,ds:Code,es:Code

Handle dw 0
SearchPath db '*.EXE', 13 dup(0)
DTA db 48 dup(0)
    db 0,13,10,'$'

Beginning:
        mov  ax,cs
        mov  ds,ax
        mov  es,ax

        mov  ah,1Ah
        mov  dx,offset DTA
        int  21h
        jc   Error
    
        mov  ah,4Eh
        mov  al,00h
        mov  cx,0
        mov  dx,offset SearchPath
        int  21h
        jc   Error
        mov  ah,9
        mov  dx,offset DTA+1Eh
        int  21h
        mov  cx,13
        mov  di,offset DTA+1Eh
        mov  al,0
        rep  stosb

        SearchLoop:
            mov  ah,4Fh
            int  21h
            jc   Error
            mov  ah,9
            mov  dx,offset DTA+1Eh
            int  21h
            mov  cx,13
            mov  di,offset DTA+1Eh
            mov  al,0
            rep  stosb
            jmp  SearchLoop

Error:
        mov  ax,4C00h
        int  21h

Code    ENDS

END Beginning
