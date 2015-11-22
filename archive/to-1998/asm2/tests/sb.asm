
.MODEL TINY
.STACK 100h
.386

Data SEGMENT 'MSEG' USE16
IName   db 'sample.raw',0
IHandle dw 0
Buffer  db 47000 dup (?)
Data ENDS

Code SEGMENT 'CODE' USE16
ASSUME cs:Code,ds:Data

    jmp  Beginning

INCLUDE m_dos.386
INCLUDE m_sb.386

IRQHandler PROC FAR
    call PlaySample
    mov  ah,4Ch
    int  21h

    mov  al,20h
    out  20h,al
    iret
IRQHandler ENDP

HandlerSeg dw 0
HandlerOfs dw 0

Beginning:
    cli
    mov  HandlerSeg,seg IRQHandler
    mov  HandlerOfs,offset IRQHandler
    SetIntVec 13,HandlerSeg,HandlerOfs
    sti

    LoadInstr 0,47000
    call PlaySample

    mov  ax,4C00h
    int  21h

Code ENDS

END
