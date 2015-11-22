
; this program demonstrates a real plasma effect

.MODEL SMALL
.STACK 100h
.386

INCLUDE m_maths.386

Code SEGMENT 'CODE' WORD USE16 PUBLIC
ASSUME cs:Code,ds:Code

Beginning:      ;the program starts execution here
    mov  ax,0013h
    int  10h

    ASSUME ds:Code
    mov  ax,Code
    mov  ds,ax
    mov  si,offset Palette
    call SetPalette

; *********************************
    mov  cx,512
    LoopToMe:
        push cx

        ;mov  ax,cx
        ;and  ax,11111111b
        ;CosSin ax
        ;shr  eax,16
        ;add  eax,1

        add  AddValue1,0
        add  AddValue2,0
        add  AddValue3,2
        add  AddValue4,1

        call DrawRealPlasma

        ;call RotatePalRed
        ;call RotatePalGreen
        ;call RotatePalBlue
        ;mov  si,offset Palette
        ;call SetPalette

        pop  cx
        dec  cx
        jnz  LoopToMe
; *********************************

    mov  ax,0003h
    int  10h

    mov  ax,4C00h
    int  21h

Code ENDS

INCLUDE m_pal.386
INCLUDE m_plasma.386

END Beginning
