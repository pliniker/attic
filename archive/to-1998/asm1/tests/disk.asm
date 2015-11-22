.model tiny
.stack 100h

code segment 'Code'
assume cs:code,ds:code,es:code

no_sectors = 1

disk_sectors db no_sectors*512 dup(0)

; read the sectors from the disk
read_sector proc near
    mov  ah,2
    mov  al,no_sectors
    mov  ch,0
    mov  cl,1
    mov  dh,0
    mov  dl,0
    mov  bx,offset disk_sectors
    int  13h
    jc   read_error
    ret
read_error:
    mov  ah,09h
    mov  dx,offset read_err_msg
    int  21h
    ret
read_err_msg db 'Error reading disk.',13,10,'$'
read_sector endp

; save the sectors to a file on the hard disk
make_file proc near
    mov  ah,3Ch
    mov  cx,0
    mov  dx,offset file_name
    int  21h
    jc   file_error

    mov  ah,3Dh
    mov  al,01
    mov  dx,offset file_name
    int  21h
    jc   file_error
    mov  file_handle,ax

    mov  ah,40h
    mov  bx,file_handle
    mov  cx,no_sectors*512
    mov  dx,offset disk_sectors
    int  21h
    jc   file_error

    mov  ah,3Eh
    mov  bx,file_handle
    int  21h
    ret
file_error:
    mov  ah,09h
    mov  dx,offset file_err_msg
    int  21h
    ret
file_err_msg db 'Error creating file.',13,10,'$'
file_name db 'bootsctr.inf',0
file_handle dw 0
make_file endp

; main code section
beginning:
    mov  ax,code
    mov  ds,ax
    mov  es,ax

    call read_sector
    call make_file

    mov  ax,4C00h
    int  21h

code ends

end beginning
