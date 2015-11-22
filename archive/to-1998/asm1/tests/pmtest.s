	.file	"pmtest.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 16
.globl test_proc
	.type	 test_proc,@function
test_proc:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 8(%ebp),%edx
	movb %dl,-1(%ebp)
	movsbl -1(%ebp),%eax
	movl %eax,%ecx
	addl 12(%ebp),%ecx
	movl %ecx,%eax
	jmp .L1
	.align 16
.L1:
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe1:
	.size	 test_proc,.Lfe1-test_proc
	.align 16
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	pushl $2
	pushl $1
	call test_proc
	addl $8,%esp
	xorl %eax,%eax
	jmp .L2
	.align 16
.L2:
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe2:
	.size	 main,.Lfe2-main
	.ident	"GCC: (GNU) 2.7.2.1"
