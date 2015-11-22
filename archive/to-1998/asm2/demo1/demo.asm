 %out
; M A J O R   D E M O number 5
    
.MODEL SMALL
.STACK 512
.386        

INCLUDE m_modseg.386

Code    SEGMENT WORD 'CODE' USE16
        ASSUME  cs:Code
ProgramStart:
        jmp  BEGINNING

INCLUDE m_dos.386
INCLUDE m_sb.386
INCLUDE m_mod.386
INCLUDE m_grfx.386

Beginning:
; Program starts here

; setup video mode
        mov  ax,0013h
        int  10h

        assume ds:Code
        mov  ax,Code
        mov  ds,ax
        mov  si,offset PaletteList
        call SetPalette

        mov  ax,0A000h
        mov  gs,ax

; setup sound
        call SBReset

        call ClearSamples
        call LoadInstrs
        neg  _currentmixer
        call ClearMixer
        neg  _currentmixer
        call ClearMixer
        
        call SetTimerOn

; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
MainProgramLoop:

    mov  bx,255
    Loop1:
        call EffectD
        dec  bx
        jnz  Loop1

    mov  bx,255
    Loop2:
        call EffectE
        dec  bx
        jnz  Loop2

    mov  bx,0
    Loop8a:
        call EffectK
        add  bx,1
        cmp  bx,320
        jne  Loop8a

    mov  bx,0
    Loop9a:
        call EffectL
        add  bx,322
        cmp  bx,64400
        jne  Loop9a

    mov  bx,255
    Loop3:
        call EffectF
        dec  bx
        jnz  Loop3

    mov  bx,255
    Loop4:
        call EffectG
        dec  bx
        jnz  Loop4

    mov  bx,0
    Loop8b:
        call EffectK
        add  bx,-1
        cmp  bx,-320
        jne  Loop8b

    mov  bx,255
    Loop5:
        call EffectH
        dec  bx
        jnz  Loop5

    mov  bx,255
    Loop6:
        call EffectI
        dec  bx
        jnz  Loop6

    mov  bx,0
    Loop8c:
        call EffectK
        add  bx,320
        cmp  bx,64000
        jne  Loop8c

    mov  bx,0
    Loop9b:
        call EffectL
        add  bx,322
        cmp  bx,64400
        jne  Loop9b

jmp MainProgramLoop
; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

; Program ends here

Code    ENDS
%out
        END  ProgramStart
