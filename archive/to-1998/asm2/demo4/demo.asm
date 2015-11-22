    
.MODEL SMALL
.STACK 100h
.386        

INCLUDE m_modseg.386

Code    SEGMENT WORD 'CODE' USE16
        ASSUME  cs:Code

INCLUDE m_dos.386
INCLUDE m_sb.386
INCLUDE m_modcde.386

INCLUDE m_maths.386
INCLUDE m_plasma.386
INCLUDE m_pal.386

ProgramStart:
; Program starts here

; setup sound
        call SBReset

        call SpeakerON

        call ClearSamples
        call LoadInstrs
        neg  _currentmixer
        call ClearMixer
        neg  _currentmixer
        call ClearMixer
        
        call SetTimerOn

        mov  ax,0013h
        int  10h

        ASSUME ds:Code
        mov  ax,Code
        mov  ds,ax
        mov  si,offset Palette
        call SetPalette

; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
MainProgramLoop:

    mov  cx,512
    LoopToMe1:
        push cx
        mov  ax,cx
        and  ax,11111111b
        CosSin ax
        shr  eax,16

        add  eax,3
        add  AddValue1,ax
        add  eax,-1
        add  AddValue2,ax
        add  eax,-1
        add  AddValue3,ax
        add  eax,2
        add  AddValue4,ax

        call DrawRealPlasma

        call RotatePalRed
        call RotatePalBlue
        ASSUME ds:Code
        mov  ax,Code
        mov  ds,ax
        mov  si,offset Palette
        call SetPalette

        pop  cx
        dec  cx
        jnz  LoopToMe1

    mov  cx,512
    LoopToMe2:
        push cx
        mov  ax,cx
        and  ax,11111111b
        CosSin ax
        shr  eax,16

        add  eax,3
        add  AddValue1,ax
        add  eax,-1
        add  AddValue2,ax
        add  eax,-1
        add  AddValue3,ax
        add  eax,2
        add  AddValue4,ax

        call DrawRealPlasma

        call RotatePalGreen
        call RotatePalBlue
        ASSUME ds:Code
        mov  ax,Code
        mov  ds,ax
        mov  si,offset Palette
        call SetPalette

        pop  cx
        dec  cx
        jnz  LoopToMe2

jmp MainProgramLoop
; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


; Program ends here

Code    ENDS

        END  ProgramStart
