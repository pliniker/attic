
; 3D graphics

.MODEL TINY
.STACK 512
.386

GSeg SEGMENT 'GSEG1' USE16
Buffer db 64000 dup(?)
GSeg ENDS


include m_maths.386
include m_3D.386

Code    SEGMENT WORD 'CODE' USE16 PUBLIC
    assume  cs:Code,ds:Code,es:Code

ViewBuffer PROC NEAR
    pusha
    push ds
    push es

    mov  ax,GSeg
    mov  ds,ax
    mov  si,0
    mov  ax,0A000h
    mov  es,ax
    mov  di,0
    mov  cx,64000
    rep  movsb

    pop  es
    pop  ds
    popa
    ret
ViewBuffer ENDP

ClearBuffer PROC NEAR
    pusha
    push ds
    push es

    mov  ax,GSeg
    mov  es,ax
    mov  di,0
    mov  al,0
    mov  cx,64000
    rep  stosb

    pop  es
    pop  ds
    popa
    ret
ClearBuffer ENDP

include d_misc.386
include d_hex.086

ViewCoords PROC NEAR
    mov  _Angle,2
    mov  _Object,offset Point1
    call RotX
    mov  _Object,offset Point2
    call RotX
    mov  _Object,offset Point3
    call RotX
    mov  _Object,offset Point4
    call RotX

    Out32 [Point1],    '.'
    Out32 [Point1+4],  '.'
    Out32 [Point1+8],  '.'
    ReturnKey
    Out32 [Point2],    '.'
    Out32 [Point2+4],  '.'
    Out32 [Point2+8],  '.'
    ReturnKey
    Out32 [Point3],    '.'
    Out32 [Point3+4],  '.'
    Out32 [Point3+8],  '.'
    ReturnKey
    Out32 [Point4],    '.'
    Out32 [Point4+4],  '.'
    Out32 [Point4+8],  '.'
    ReturnKey
    ret
    ENDP

ViewPoints PROC NEAR
    mov  _Object,offset Point1
    call RotY
    mov  _Object,offset Point2
    call RotY
    mov  _Object,offset Point3
    call RotY
    mov  _Object,offset Point4
    call RotY

    mov  ax,13h
    int  10h
    ViewAsPixel Point1,World1
    ViewAsPixel Point2,World1
    ViewAsPixel Point3,World1
    ViewAsPixel Point4,World1
    ret
    ENDP

Beginning:
    mov  ax,Code
    mov  ds,ax
    mov  es,ax

;    call ViewCoords

    mov  ax,GSeg
    mov  es,ax

    mov  _Angle,0
    Rotate:
        push _Angle

        mov  _Angle,2
        call ClearBuffer
        call ViewPoints
        call ViewBuffer

        ; slow loop
        mov  ecx,04FFFFh
        loopme: dec  ecx
                jnz  loopme    

        pop  _Angle
        inc  _Angle
        cmp  _Angle,90
        jne  Rotate

    mov  ah,4Ch
    int  21h

Code    ENDS

END Beginning
