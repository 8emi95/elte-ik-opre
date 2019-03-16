	.file	"nevcso.c"
	.intel_syntax noprefix
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
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 1056
	mov	DWORD PTR [rbp-1044], edi
	mov	QWORD PTR [rbp-1056], rsi
	mov	edi, OFFSET FLAT:.LC0
	call	puts
	mov	esi, 384
	mov	edi, OFFSET FLAT:.LC1
	call	mkfifo
	mov	DWORD PTR [rbp-4], eax
	cmp	DWORD PTR [rbp-4], -1
	jne	.L2
	call	__errno_location
	mov	eax, DWORD PTR [rax]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC2
	mov	eax, 0
	call	printf
	mov	edi, 1
	call	exit
.L2:
	mov	edi, OFFSET FLAT:.LC3
	call	puts
	call	fork
	mov	DWORD PTR [rbp-8], eax
	cmp	DWORD PTR [rbp-8], 0
	jle	.L3
	movabs	rax, 452807451987
	mov	QWORD PTR [rbp-1040], rax
	lea	rsi, [rbp-1032]
	mov	eax, 0
	mov	edx, 127
	mov	rdi, rsi
	mov	rcx, rdx
	rep stosq
	mov	eax, DWORD PTR [rbp-4]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	mov	esi, 0
	mov	edi, OFFSET FLAT:.LC1
	mov	eax, 0
	call	open
	mov	DWORD PTR [rbp-12], eax
	lea	rcx, [rbp-1040]
	mov	eax, DWORD PTR [rbp-12]
	mov	edx, 1024
	mov	rsi, rcx
	mov	edi, eax
	call	read
	lea	rax, [rbp-1040]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC5
	mov	eax, 0
	call	printf
	mov	eax, DWORD PTR [rbp-12]
	mov	edi, eax
	call	close
	mov	edi, OFFSET FLAT:.LC1
	call	unlink
	jmp	.L4
.L3:
	mov	edi, OFFSET FLAT:.LC6
	call	puts
	mov	eax, DWORD PTR [rbp-4]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC7
	mov	eax, 0
	call	printf
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC1
	mov	eax, 0
	call	open
	mov	DWORD PTR [rbp-12], eax
	mov	eax, DWORD PTR [rbp-12]
	mov	edx, 12
	mov	esi, OFFSET FLAT:.LC8
	mov	edi, eax
	call	write
	mov	eax, DWORD PTR [rbp-12]
	mov	edi, eax
	call	close
	mov	edi, OFFSET FLAT:.LC9
	call	puts
.L4:
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
