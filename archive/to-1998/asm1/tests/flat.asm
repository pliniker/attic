.386

.model flat

.stack 200h

.data

some_data db 1024*1024 dup(?)       ; jumble of useless bytes
message db 'Hello world',13,10,'$'

.code

beginning:
    mov  ax,_data
    mov  ds,ax
    mov  edx,offset message
    mov  ah,09h
    int  21h

    mov  ah,4Ch
    int  21h

end beginning
