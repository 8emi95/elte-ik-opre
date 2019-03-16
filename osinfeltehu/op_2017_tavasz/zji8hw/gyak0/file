	.file	"file.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"You have to use program with two arguments, the file names copy_from copy_to"
.LC1:
	.string	"Error at opening the file\n"
	.align 8
.LC2:
	.string	"There is a mistake in writing\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	cmpl	$3, -20(%rbp)
	je	.L2
	movl	$.LC0, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L2:
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jns	.L3
	movl	$.LC1, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L3:
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movl	$384, %edx
	movl	$577, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jns	.L4
	movl	$.LC1, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L4:
	jmp	.L5
.L6:
	movzbl	-9(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar
	leaq	-9(%rbp), %rcx
	movl	-8(%rbp), %eax
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	movl	$0, %eax
	call	write
	cmpl	$1, %eax
	je	.L5
	movl	$.LC2, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L5:
	leaq	-9(%rbp), %rcx
	movl	-4(%rbp), %eax
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	movl	$0, %eax
	call	read
	testl	%eax, %eax
	jne	.L6
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	close
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	close
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
