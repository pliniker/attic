%out

.MODEL SMALL
.STACK 200h
.386

INCLUDE m_modseg.386

Code        SEGMENT WORD 'CODE' USE16
    ASSUME cs:Code
    ASSUME ds:Code
    mov  ax,Code
    mov  ds,ax
    jmp Beginning

INCLUDE m_dos.386
INCLUDE m_pmode.386
INCLUDE m_sb.386

INCLUDE m_modcde.386

Beginning:

; set fs & gs to 4Gb segments
    call Protected_Mode

; reset the SB card
    call SBReset
    call ClearData

    call LoadInstruments
    call ClearData
    GetSample 1

; play the sample
    mov  SampleSeg,seg SData
    mov  SampleOfs,offset SData
    call PlaySample

    mov  ax,4C00h
    int  21h

Code        ENDS

%out
END
