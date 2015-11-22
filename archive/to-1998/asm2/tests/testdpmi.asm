
.MODEL SMALL
.STACK 256

Code SEGMENT 'Code' 
ASSUME cs:Code,ds:Code

Msg1 db 'Int 31h available.',13,10,'$'
Msg2 db 'Int 31h unavailable.',13,10,'$'
Msg3 db 'DPMI available.',13,10,'$'
Msg4 db 'DPMI unavailable.',13,10,'$'

Beginning:
    mov  ax,Code
    mov  ds,ax

    mov  ax,1687h
    int  2Fh
    cmp  ax,0
    jne  NoDPMI
    mov  ah,9h
    mov  dx,offset Msg3
    int  21h
    jmp  YesDPMI
    NoDPMI:
    mov  ah,9h
    mov  dx,offset Msg4
    int  21h
    YesDPMI:

    mov  ax,1686h
    int  2Fh
    cmp  ax,0
    jne  NoInt31
    mov  ah,9h
    mov  dx,offset Msg1
    int  21h
    jmp  YesInt31
    NoInt31:
    mov  ah,9h
    mov  dx,offset Msg2
    int  21h
    YesInt31:

    mov  ax,4C00h
    int  21h

Code ENDS

END Beginning
