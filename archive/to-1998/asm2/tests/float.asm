; testing floating point arithmetic ( ???HOW??? does it work!?!)

.MODEL TINY
.STACK 

Code SEGMENT 'CODE' USE16
ASSUME cs:Code,ds:Code

INCLUDE m_float.386

; 32 bit floating point numbers : 24bit mantissa, 8bit exponent
; sign,exponent,sign,mantissa
;       se      sm 

;Num1 dd 00000001010000000000000000000000b
;Num2 dd 00000001010000000000000000000000b
Num1 dd 70000001h
Num2 dd 70000001h

Beginning:
    mov  ax,Code
    mov  ds,ax
    finit

    fld  Num1
    fld  Num2
    fadd (0),(1)

;    FloatAdd Num1,Num2
;    mov  ebx,Num1
;    call OutFloat

    mov  ax,4C00h
    int  21h

Code ENDS

END Beginning
