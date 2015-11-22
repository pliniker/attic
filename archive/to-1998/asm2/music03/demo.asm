%out

.MODEL TINY
.STACK 200h

INCLUDE m_dos.386
INCLUDE m_pmode.386
INCLUDE m_sb.386
INCLUDE m_mod.386

;***************************************************************************
;                           CODE SEGMENT
;***************************************************************************
Code        SEGMENT WORD 'CODE' USE16 PUBLIC
    ASSUME cs:Code

ProgramStart:
    ASSUME ds:Samples1
    mov  ax,Samples1
    mov  ds,ax
    
; set fs & gs to 4Gb segments
    call Test_V86
    call Protected_Mode

    call LoadInstruments

; reset the SB card
    call SBReset
    call SpeakerOn

; play the sample
    mov  SampleSeg,seg SData
    mov  SampleOfs,offset SData

    GetSample 2
    call PlaySample

    mov  ax,4C00h
    int  21h

Code        ENDS

%out
END ProgramStart
