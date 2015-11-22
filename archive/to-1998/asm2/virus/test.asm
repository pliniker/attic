
.model tiny
.stack 100h

Data    SEGMENT 'DATA' WORD USE16 PUBLIC
        db 10000 dup(0)
Msg db 'Test program running...',13,10,'$'
Data    ENDS        

Code    SEGMENT 'CODE' WORD USE16 PUBLIC
        ASSUME cs:Code

db 16000 dup (0)

Beginning:
        mov  ax,Data
        mov  ds,ax
        mov  dx,offset Msg
        mov  ah,09h
        int  21h
        mov  ax,4C00h
        int  21h

db 15444 dup (0)

Code    ENDS

END Beginning

