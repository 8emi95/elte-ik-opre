	.file	"nevcso.c"
	.section	.rodata
.LC0:
	.string	"Fifo start!"
.LC1:
	.string	"fifo.ftc"
.LC2:
	.string	"Error number: %i"
.LC3:
	.string	"Mkfifo vege, fork indul!"
	.align 8
.LC4:
	.string	"Csonyitas eredmenye szuloben: %d!\n"
.LC5:
	.string	"Ezt olvastam a csobol: %s \n"
.LC6:
	.string	"Gyerek vagyok, irok a csobe!"
.LC7:
	.string	"Csonyitas eredmenye: %d!\n"
.LC8:
	.string	"Hajra Fradi!\n"
	.align 8
.LC9:
	.string	"Gyerek vagyok, beirtam, vegeztem!"
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
	subq	$1056, %rsp
	movl	%edi, -1044(%rbp)
	movq	%rsi, -1056(%rbp)
	movl	$.LC0, %edi
	call	puts
	movl	$384, %esi
	movl	$.LC1, %edi
	call	mkfifo
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L2
	call	__errno_location
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L2:
	movl	$.LC3, %edi
	call	puts
	call	fork
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jle	.L3
	movabsq	$452807451987, %rax
	movq	%rax, -1040(%rbp)
	leaq	-1032(%rbp), %rsi
	movl	$0, %eax
	movl	$127, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	open
	movl	%eax, -12(%rbp)
	leaq	-1040(%rbp), %rcx
	movl	-12(%rbp), %eax
	movl	$1024, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read
	leaq	-1040(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	close
	movl	$.LC1, %edi
	call	unlink
	jmp	.L4
.L3:
	movl	$.LC6, %edi
	call	puts
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	open
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	$12, %edx
	movl	$.LC8, %esi
	movl	%eax, %edi
	call	write
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	close
	movl	$.LC9, %edi
	call	puts
.L4:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
