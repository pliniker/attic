	.file	"critters.cc"
	.version	"01.01"
/ GNU C++ version egcs-2.90.27 980315 (egcs-1.0.2 release) (i386-redhat-linux) compiled by GNU C version egcs-2.90.27 980315 (egcs-1.0.2 release).
/ options passed:  -c
/ options enabled:  -fpeephole -ffunction-cse -fkeep-static-consts
/ -fpcc-struct-return -fsched-interblock -fsched-spec -fexceptions -fcommon
/ -fverbose-asm -fgnu-linker -fargument-alias -m80387 -mhard-float
/ -mno-soft-float -mieee-fp -mfp-ret-in-387 -mschedule-prologue -mcpu=i386
/ -march=pentium

gcc2_compiled.:
.globl i
.data
	.align 4
	.type	 i,@object
	.size	 i,4
i:
	.long 0
.globl printf
.section	.rodata
.LC0:
	.string	"DNA dump: \n\n"
.data
	.align 4
	.type	 printf,@object
	.size	 printf,4
printf:
	.long .LC0
.globl lookedy
	.align 4
	.type	 lookedy,@object
	.size	 lookedy,4
lookedy:
	.long 0
.globl lookedx
	.align 4
	.type	 lookedx,@object
	.size	 lookedx,4
lookedx:
	.long 1
.globl a
	.align 4
	.type	 a,@object
	.size	 a,4
a:
	.long 0
.globl b
	.align 4
	.type	 b,@object
	.size	 b,4
b:
	.long 0
.globl c
	.align 4
	.type	 c,@object
	.size	 c,4
c:
	.long 0
.globl move_counter
	.align 4
	.type	 move_counter,@object
	.size	 move_counter,4
move_counter:
	.long 0
.text
	.align 4
.globl _GLOBAL_.I.i
	.type	 _GLOBAL_.I.i,@function
_GLOBAL_.I.i:
.LFB1:
	pushl %ebp
.LCFI0:
	movl %esp,%ebp
.LCFI1:
	movl b,%eax
	subl %eax,a
	jmp .L2
	.align 4
	jmp .L1
	.align 4
.L2:
.L1:
	leave
	ret
.LFE1:
.Lfe1:
	.size	 _GLOBAL_.I.i,.Lfe1-_GLOBAL_.I.i
.section	.ctors,"aw"
	.long	 _GLOBAL_.I.i
.globl moved
.bss
	.align 4
	.type	 moved,@object
	.size	 moved,4
moved:
	.zero	4
.globl targetx
	.align 4
	.type	 targetx,@object
	.size	 targetx,4
targetx:
	.zero	4
.globl targety
	.align 4
	.type	 targety,@object
	.size	 targety,4
targety:
	.zero	4

.section	.eh_frame,"aw",@progbits
__FRAME_BEGIN__:
	.4byte	.LLCIE1
.LSCIE1:
	.4byte	0x0
	.byte	0x1
	.byte	0x0
	.byte	0x1
	.byte	0x7c
	.byte	0x8
	.byte	0xc
	.byte	0x4
	.byte	0x4
	.byte	0x88
	.byte	0x1
	.align 4
.LECIE1:
	.set	.LLCIE1,.LECIE1-.LSCIE1
	.4byte	.LLFDE1
.LSFDE1:
	.4byte	.LSFDE1-__FRAME_BEGIN__
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.byte	0x4
	.4byte	.LCFI0-.LFB1
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xd
	.byte	0x5
	.align 4
.LEFDE1:
	.set	.LLFDE1,.LEFDE1-.LSFDE1
	.ident	"GCC: (GNU) egcs-2.90.27 980315 (egcs-1.0.2 release)"
