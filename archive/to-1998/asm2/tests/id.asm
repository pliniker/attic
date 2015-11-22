
.MODEL TINY
.STACK 100h

INCLUDE m_opcode.586
INCLUDE d_hex.086

Code    SEGMENT 'CODE' USE16
        ASSUME cs:Code,ds:Code

CPUimpossible LABEL BYTE
            db 'CPUID unavailable',13,10,'$'
CPUdescription1 LABEL BYTE
            db 'CPU ID string: '
IDstring    dd 0,0,0
            db 13,10,'$'
CPUdescription2 LABEL BYTE
            db 'CPU type: $'
CPU486      db '486',13,10,'$'
CPU586      db 'Pentium',31,10,'$'
CPU686      db 'Pentium Pro',13,10,'$'
CPUnot      db '???',13,10,'$'

Beginning:
        mov  ax,Code
        mov  ds,ax

; is cpuid possible?
        pushfd
        pop  eax
        and  eax,1000000000000000000000b
        jz   Impossible
        jmp  Possible
    Impossible:
        mov  ah,09h
        mov  dx,offset CPUimpossible
        int  21h
        mov  ax,4C00h
        int  21h
    Possible:            

; dump out registers first
        cpuid 0
        mov  esi,ebx
        outhex32 esi
        return
        outhex32 ecx
        return
        outhex32 edx
        return
        cpuid 1
        outhex32 eax
        return
        outhex32 edx
        return
        cpuid 2
        mov  esi,ebx
        outhex32 eax
        return
        outhex32 esi
        return
        outhex32 ecx
        return
        outhex32 edx
        return

; get the cpu id string / "GenuineIntel" or "AuthenticAMD" etc
        cpuid 0
        mov  IDstring[0],ebx
        mov  IDstring[4],ecx
        mov  IDstring[8],edx
        mov  ah,09h
        mov  dx,offset CPUdescription1
        int  21h

; get the cpu type - 486, Pentium, Pentium Pro, ???
        mov  ah,09h
        mov  dx,offset CPUdescription2
        int  21h
        cpuid 1
        xor  ecx,ecx
        and  eax,011111111000b
        mov  ebx,eax
        and  ebx,011110000000b

    Is486:
        cmp  ebx,001000000000b
        jne  IsPentium
        mov  dx,offset CPU486
    IsPentium:
        cmp  ebx,001100000000b
        jne  IsPentiumPro
        mov  dx,offset CPU586
    IsPentiumPro:
        cmp  ebx,010100000000b
        jne  IsNothing
        mov  dx,offset CPU686
        jmp  CPUoutput
    IsNothing:
        mov  dx,offset CPUnot
    CPUoutput:
        mov  ah,09h
        int  21h

        mov  ax,4C00h
        int  21h

Code    ENDS

END Beginning
