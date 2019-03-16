	.file	"szemafor.c"
	.section	.rodata
.LC0:
	.string	"semget"
.LC1:
	.string	"semctl"
	.text
	.globl	szemafor_letrehozas
	.type	szemafor_letrehozas, @function
szemafor_letrehozas:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	-24(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	ftok
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$896, %edx
	movl	$1, %esi
	movl	%eax, %edi
	call	semget
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jns	.L2
	movl	$.LC0, %edi
	call	perror
.L2:
	movl	-28(%rbp), %edx
	movl	-8(%rbp), %eax
	movl	%edx, %ecx
	movl	$16, %edx
	movl	$0, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	semctl
	testl	%eax, %eax
	jns	.L3
	movl	$.LC1, %edi
	call	perror
.L3:
	movl	-8(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	szemafor_letrehozas, .-szemafor_letrehozas
	.section	.rodata
.LC2:
	.string	"semop"
	.text
	.globl	szemafor_muvelet
	.type	szemafor_muvelet, @function
szemafor_muvelet:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movw	$0, -16(%rbp)
	movl	-24(%rbp), %eax
	movw	%ax, -14(%rbp)
	movw	$0, -12(%rbp)
	leaq	-16(%rbp), %rcx
	movl	-20(%rbp), %eax
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	semop
	testl	%eax, %eax
	jns	.L5
	movl	$.LC2, %edi
	call	perror
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	szemafor_muvelet, .-szemafor_muvelet
	.globl	szemafor_torles
	.type	szemafor_torles, @function
szemafor_torles:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$0, %edx
	movl	$0, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	semctl
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	szemafor_torles, .-szemafor_torles
	.section	.rodata
	.align 8
.LC3:
	.string	"Szulo indul, kozos memoriaba irja: %s\n"
.LC4:
	.string	"Szulo, szemafor up!"
.LC5:
	.string	"Gyerek: Indula szemafor down!"
	.align 8
.LC6:
	.string	"Gyerek, down rendben, eredmeny: %s"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	movq	-80(%rbp), %rax
	movq	(%rax), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	ftok
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$896, %edx
	movl	$1024, %esi
	movl	%eax, %edi
	call	shmget
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	$0, %edx
	movl	$0, %esi
	movl	%eax, %edi
	call	shmat
	movq	%rax, -16(%rbp)
	movq	-80(%rbp), %rax
	movq	(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	szemafor_letrehozas
	movl	%eax, -20(%rbp)
	call	fork
	movl	%eax, -24(%rbp)
	cmpl	$0, -24(%rbp)
	jle	.L9
	movabsq	$5269323076028080201, %rax
	movq	%rax, -64(%rbp)
	movabsq	$8020955056186944620, %rax
	movq	%rax, -56(%rbp)
	movabsq	$4210994773112594544, %rax
	movq	%rax, -48(%rbp)
	movl	$663849, -40(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	$4, %edi
	call	sleep
	leaq	-64(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	$.LC4, %edi
	call	puts
	movl	-20(%rbp), %eax
	movl	$1, %esi
	movl	%eax, %edi
	call	szemafor_muvelet
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	shmdt
	movl	$0, %eax
	movq	%rax, %rdi
	call	wait
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	szemafor_torles
	movl	-8(%rbp), %eax
	movl	$0, %edx
	movl	$0, %esi
	movl	%eax, %edi
	call	shmctl
	jmp	.L10
.L9:
	cmpl	$0, -24(%rbp)
	jne	.L10
	movl	$.LC5, %edi
	call	puts
	movl	-20(%rbp), %eax
	movl	$-1, %esi
	movl	%eax, %edi
	call	szemafor_muvelet
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC6, %edi
	movl	$0, %eax
	call	printf
	movl	-20(%rbp), %eax
	movl	$1, %esi
	movl	%eax, %edi
	call	szemafor_muvelet
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	shmdt
.L10:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
