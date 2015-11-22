
; to test the programmable interval timer (PIT)

.MODEL TINY
.STACK 100h
.386
 
Code SEGMENT 'CODE' USE16
    ASSUME cs:Code,ds:Code,es:Code

INCLUDE m_dos.386

OldHandlerSeg dw 0                          ; save the address of the old
OldHandlerOfs dw 0                          ; handler to be restored at end

bpm equ 1193180 / 125
songlength equ 32
_counter dw 0
speed equ 9
_notecounter dd 0

SetTimerOn PROC NEAR                        ; procedure to set the timer
ASSUME ds:Code
mov  ax,Code
mov  ds,ax
    cli                                     ; to my own handler
    mov  al,36h
    out  43h,al
    mov  ax,bpm
    out  40h,al
    shr  ax,8
    out  40h,al

    mov  al,08h
    mov  ah,35h
    int  21h                                ; dos function getintvec
    mov  OldHandlerSeg,es                   ; to get addr of old handler
    mov  OldHandlerOfs,bx

    mov  dx,seg IRQHandler
    mov  ds,dx
    mov  dx,offset IRQHandler
    mov  al,08h
    mov  ah,25h
    int  21h                                ; dos func setintvec to rerout
    sti                                     ; the int to my handler
    ret
SetTimerOn ENDP
;-------------------------------------------------------------------------
SetTimerOff PROC FAR                        ; procedure to restore the old
ASSUME ds:Code
mov  ax,Code
mov  ds,ax
    cli                                     ; handler at the end of the
    mov  dx,OldHandlerSeg                   ; program
    mov  ds,dx
    mov  dx,OldHandlerOfs
    mov  al,08h
    mov  ah,25h
    int  21h

    mov  al,36h
    out  43h,al
    mov  al,0
    out  40h,al
    out  40h,al
    sti

    in   al,20h
    and  al,0b
    out  20h,al

    mov  al,20h
    out  20h,al

    mov  ah,4Ch
    int  21h
    ret
SetTimerOff ENDP
;-------------------------------------------------------------------------
IRQHandler PROC FAR                     ; Interrupt handler for IRQ 0
    pusha                               ; this is the timer handler
    push ds                            
    push es
ASSUME ds:Code
mov  ax,Code
mov  ds,ax
inc  _counter
cmp  _counter,speed
je   Skipnote
jmp  Overnote
Skipnote:
mov  _counter,0
inc  _notecounter
cmp  _notecounter,songlength
je   EndMusic
Overnote:
    pop  es
    pop  ds
    popa
    mov  al,0h
    out  20h,al
    iret
EndMusic:
    call SetTimerOff
    pop  es
    pop  ds
    popa
    mov  al,0h
    out  20h,al
    iret
IRQHandler ENDP
 
Beginning:
    mov  ax,Code
    mov  ds,ax
    mov  es,ax

    call SetTimerOn

looptome:
jmp looptome

Code ENDS

END Beginning

