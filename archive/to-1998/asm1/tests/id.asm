

Code    SEGMENT 'CODE' USE16
        ASSUME cs:Code,ds:Code

        mov  ax,4C00h
        int  21h

Code    ENDS

END 
