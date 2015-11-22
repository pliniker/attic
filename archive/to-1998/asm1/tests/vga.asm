
.MODEL TINY
.STACK 100h
.386

INCLUDE m_vga.386

Code SEGMENT 'CODE' WORD USE16 PUBLIC
ASSUME cs:Code,ds:Code

ProgramStart:
; set ds to code seg
    mov  ax,Code
    mov  ds,ax

    call Mode_X

    mov  ax,4C00h
    int  21h
Code ENDS

END ProgramStart
