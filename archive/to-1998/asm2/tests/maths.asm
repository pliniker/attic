.MODEL TINY
.STACK 100h
.386

Code SEGMENT 'CODE' USE16
    ASSUME cs:Code,ds:Code,es:Code
    jmp  Beginning

INCLUDE d_hex.086
INCLUDE m_maths.386

Beginning:

    mov  eax,0FFh            
looptome:
    push eax
    SquareRoot eax
    mov  bx,ax
    call OutHex
    pop  eax
    dec  eax
    jnz  looptome

    mov  ax,4C00h
    int  21h


Code ENDS

END
