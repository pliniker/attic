	.386p
	model flat
	ifndef	??version
	?debug	macro
	endm
	endif
	?debug	S "pmtest.c"
	?debug	T "pmtest.c"
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
DGROUP	group	_BSS,_DATA
_TEXT	segment dword public use32 'CODE'
_test_proc	proc	near
?live1@0:
   ;	
   ;	int test_proc(char x, int y) {
   ;	
	push      ebp
	mov       ebp,esp
   ;	
   ;	    return x + y;
   ;	
@1:
	movsx     eax,byte ptr [ebp+8]
	add       eax,dword ptr [ebp+12]
   ;	
   ;	}
   ;	
@3:
@2:
	pop       ebp
	ret 
_test_proc	endp
_main	proc	near
?live1@36:
   ;	
   ;	int main() {
   ;	
	push      ebp
	mov       ebp,esp
   ;	
   ;	
   ;	    test_proc(1 ,2);
   ;	
@4:
	push      2
	push      1
	call      _test_proc
	add       esp,8
   ;	
   ;	
   ;	    return 0;
   ;	
	xor       eax,eax
   ;	
   ;	}
   ;	
@6:
@5:
	pop       ebp
	ret 
_main	endp
_TEXT	ends
_s@	equ	s@
	public	_test_proc
	public	_main
	?debug	D "pmtest.c" 9537 29846
	end
