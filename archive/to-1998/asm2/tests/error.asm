.MODEL TINY
.STACK 256
.386

Code    SEGMENT 'CODE' USE16
        ASSUME  cs:Code,ds:Code,es:Code
        jmp  Beginning

include m_err.386

Beginning:
        
        mov  bx,0ABCDh
        call OutHex

        mov  ah,4Ch
        int  21h

Code    ENDS

END
