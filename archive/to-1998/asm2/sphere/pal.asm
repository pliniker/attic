.386

code segment public use16
assume cs:code,ds:code

NewPalette:
db 0,0,24
x=0
REPT 64
db 0,0,x
db 0,0,x
db 0,0,x
db 0,0,x
x=x+1
ENDM

public DrawPixel
public SetPalette

SetPalette PROC PASCAL
    pusha
    push ds
    mov  ax,code
    mov  ds,ax
    mov  si,offset NewPalette

    mov  cx,256
    mov  ah,0
DACLoadLoop:
    mov  dx,03C8h
    mov  al,ah
    cli
    out  dx,al       ;set the DAC location #
    mov  dx,03C9h
    lodsb
    out  dx,al       ;set the red component
    lodsb
    out  dx,al       ;set the green component
    lodsb
    out  dx,al       ;set the blue component
    sti
    inc  ah
    loop DACLoadLoop

    pop  ds
    popa
    ret
SetPalette ENDP

DrawPixel PROC PASCAL x:word,y:word,c:byte
    pusha
    push ds
    mov  ax,0A000h
    mov  ds,ax
    mov  si,y
    imul si,320
    add  si,x
    mov  al,c
    mov  [si],al
    pop  ds
    popa
    ret
DrawPixel ENDP

code ends

END
