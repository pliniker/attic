
%out

.MODEL SMALL
.STACK 512
.386        

INCLUDE m_dos.386
INCLUDE m_sb.386
INCLUDE m_modcde.386

Code    SEGMENT WORD 'CODE' USE16 PUBLIC
        ASSUME  cs:Code

Msg  db 'Music Tracker by Peter Liniker (C) 1996',13,10,'$'

ProgramStart:
; Program starts here

        mov  ax,0003h
        int  10h

        assume ds:Code
        mov  ax,Code
        mov  ds,ax
        mov  dx,offset Msg
        mov  ah,09h
        int  21h

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

; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
MainProgramLoop:
jmp MainProgramLoop
; * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

; Program ends here

Code    ENDS
%out
        END  ProgramStart
