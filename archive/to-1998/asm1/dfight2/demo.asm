;
; D O G F I G H T / B I P L A N E
;
; version 2, 18/07/2000
; Peter Liniker
;

.MODEL SMALL
.STACK 200h
.386


INCLUDE m_file.386
INCLUDE m_err.386
INCLUDE m_sb.386
INCLUDE m_modseg.386
INCLUDE m_modcde.386
INCLUDE m_pal.386
INCLUDE m_grfx.386
INCLUDE m_maths.386
INCLUDE dogfight.386


Code SEGMENT 'CODE' WORD USE16 PUBLIC
ASSUME cs:Code


Beginning:      ;the program starts execution here

    ; mode 13h
    mov  ax,0013h
    int  10h

    ; set the palette
    ASSUME ds:Code
    mov  ax,Code
    mov  ds,ax
    mov  si,offset Palette
    call SetPalette

    ; load the sky
    call ImageLoad
    call Buf1toBuf2

    ; init keyboard
    call SetKeyboardHandler

    ; start the music gentlemen
    call SBReset
    call SpeakerON

    ASSUME ds:MusicSeg
    mov  ax,MusicSeg
    mov  dx,ax
    call ClearSamples
    call LoadInstrs
    neg  _currentmixer
    call ClearMixer
    neg  _currentmixer
    call ClearMixer

    call SetTimerOn

    ; write to buffer
    mov  ax,GBuf2
    mov  es,ax
    mov  ax,Code
    mov  ds,ax

    ; initialize vars
    mov  PlaneA_Angle,0
    mov  PlaneB_Angle,180
    mov  PlaneA_X,120
    mov  PlaneA_Y,100
    mov  PlaneB_X,200
    mov  PlaneB_Y,100

; * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Bigloop:
    ; plane A control
    call PlaneA_jstk
    mov  eax,-40000h
    FPMul eax,PlaneA_cos
    shrd eax,edx,16
    add  PlaneA_X,ax
    mov  eax,-40000h
    FPMul eax,PlaneA_sin
    shrd eax,edx,16
    add  PlaneA_Y,ax

    ; plane B control
;    call PlaneB_kbd
;    mov  eax,-40000h
;    FPMul eax,PlaneB_cos
;    shrd eax,edx,16
;    add  PlaneB_X,ax
;    mov  eax,-40000h
;    FPMul eax,PlaneB_sin
;    shrd eax,edx,16
;    add  PlaneB_Y,ax

    ; draw planes
    call PlaneA_Draw
    cmp  ax, 0
    je   quitit
;    call PlaneB_Draw
;    cmp  ax, 0
;    je   quitit

    ; draw bullets
    call calc_ABullets
    call Draw_ABullets
;    call calc_BBullets
;    call Draw_BBullets

    ; is anybody dead yet?
    call Died
    cmp  ax, 0
    je   quitit

    call ShowHits

    ; view the screen
    call ViewBufferMOV

    ; slowing loop
    mov  ecx,04FFFFh
    LoopMe: dec  ecx
            jnz  LoopMe

    ;erase planes
    call PlaneA_Draw
;    call PlaneB_Draw

    ; erase bullets
    call Draw_ABullets
;    call Draw_BBullets

    jmp  BigLoop

; * * * * * * * * * * * * * * * * * * * * * * * * * * * *

quitit:
    SetDs MusicSeg
    xor  eax,eax
    mov  eax, songlength
    mov  _notecounter, eax
waitforever: jmp waitforever

Code ENDS

END Beginning
