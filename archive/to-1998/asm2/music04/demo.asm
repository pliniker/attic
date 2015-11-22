;
.MODEL SMALL
.STACK 200h
;
INCLUDE m_dos.386
INCLUDE m_pmode.386
INCLUDE m_sb.386
INCLUDE m_mod.386
;
;***************************************************************************
;                           CODE SEGMENT
;***************************************************************************
Code    SEGMENT WORD 'CODE' USE16 PUBLIC
        ASSUME cs:Code
;
Beginning:
; set fs & gs to 4Gb segments
    call Test_V86
    call Protected_Mode
;
; reset the SB card
    call SBReset
    call DetectIRQ
;
; load all the samples
    call LoadInstruments
    call Initialize
;
    call Clear_Mixer
    call Get_Next_Notes
    call Get_Note
    neg  _current_mixer
    call PlayMixer
;
    mov  ax,4C00h
    int  21h
;
Code    ENDS
;
END Beginning
;
