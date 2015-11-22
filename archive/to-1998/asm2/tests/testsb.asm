
.MODEL TINY
.STACK 100h
.386

Sample SEGMENT USE16
IName db 'sample.raw',0
IHandle dw 0
Buffer db 64000 dup (?)
Sample ENDS

Code SEGMENT 'CODE' USE16
ASSUME cs:Code,ds:Code

INCLUDE d_hex.086
INCLUDE m_dos.386
INCLUDE m_sb.386

ctr dw 0

irq PROC  FAR
    push ds
    push es
    pusha

    assume ds:Code
    mov  ax,Code
    mov  ds,ax

    inc  ctr
    cmp  ctr,4
    je   HaltProg

    mov  dx,224h
    mov  al,82h
    out  dx,al
    inc  dl
    in   al,dx
    add  dl,9
    in   al,dx

    mov  Slength,47000
    mov  SRate,166
    call PlaySample

    mov  al,20h
    out  20h,al
    popa
    pop  es
    pop  ds
    iret

HaltProg:
    call IRQHandlerOff
    mov  al,20h
    out  20h,al
    popa
    pop  es
    pop  ds
    iret
irq ENDP

Beginning:
    call SBReset
    call DetectIRQ
    SetIRQHandler irq

    mov  Slength,47000
    mov  SRate,166
    LoadInstr Sample,Buffer,64000
    call PlaySample

    mov  ah,31h
    mov  dx,65535/16
    int  21h

    mov  ax,4C00h
    int  21h

Code ENDS

END Beginning

