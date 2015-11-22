
; !!! Pentium only !!!

.model tiny
.stack 100h

INCLUDE m_opcode.586
INCLUDE d_hex.086

Code    SEGMENT 'CODE' USE16 PUBLIC
        ASSUME cs:Code

NoMSRs equ 14
MsrMsg   db 'Model specific register $'
_spc     db ' $'
_h       db 'h',13,10,'$'

Beginning:
        push cs
        pop  ds

        mov  ebx,0

        ShowMSRs:
            cmp  ebx,03h
            je   NotMSR
            cmp  ebx,0Ah
            je   NotMSR
            cmp  ebx,0Fh
            je   NotMSR
            push ebx

            mov  ah,09h
            mov  dx,offset MsrMsg
            int  21h
            mov  esi,ebx
            outhex32 esi
            mov  ah,09h
            mov  dx,offset _spc
            int  21h

            pop  ebx
            rdmsr ebx
            push ebx
            outhex32 edx
            outhex32 eax

            mov  ah,09h
            mov  dx,offset _h
            int  21h

            pop  ebx
            NotMSR:
            inc  ebx
            cmp  ebx,NoMSRs
            jne  ShowMSRs

        mov  ax,4C00h
        int  21h

Code    ENDS

END Beginning

