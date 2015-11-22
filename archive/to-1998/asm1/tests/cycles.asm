
; !!! Pentium only !!!
.386
.model tiny
.stack 100h

INCLUDE m_opcode.586
INCLUDE d_hex.086

Code    SEGMENT 'CODE' USE16 PUBLIC
        ASSUME cs:Code

ClockMsg db 'Clock cycles passed since machine started: $'
_h db 'h',13,10,'$'

Beginning:
        push cs
        pop  ds
        
        mov  ah,09h
        mov  dx,offset ClockMsg
        int  21h

        rdtsc
        outhex32 edx
        outhex32 eax

        mov  ah,09h
        mov  dx,offset _h
        int  21h

        mov  ax,4C00h
        int  21h

Code    ENDS

END Beginning

