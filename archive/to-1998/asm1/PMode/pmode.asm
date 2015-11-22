
;
; Protected Mode interface. Doesn't work completely. Yet.
; Interrupts do not work. They reboot the computer.
; Memory reads/writes do not work!
;

.MODEL SMALL
.STACK 256
.386

INCLUDE p_dpmi.386

Code    SEGMENT 'CODE' USE16 PUBLIC
        ASSUME  cs:Code,ds:Code

Beginning:

        ; setup gdt & idt
        call SetupGDT
        call LoadGDTR
        call SetupIDT
        call LoadIDTR

        ; switch to pmode & set selectors
        call PMODE
        call SetSelect

        ; return to real mode
        call RMODE
        call ResetDTRs

        ; end the program
        call Terminate

Code    ENDS
;
; Protected Mode Code Segment
;
PCode   SEGMENT 'CODE' PUBLIC USE16
        assume cs:PCode

Beginning32:
        mov  ax,textseg
        mov  ds,ax
        mov  si,8
        mov  al,'X'
        mov  [si],al
        infloop: jmp infloop

PCode   ENDS
;
END  Beginning
;
