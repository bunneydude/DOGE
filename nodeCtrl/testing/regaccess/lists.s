	.file	"lists.c"
	.data
	.type	maskList, @object
	.size	maskList, 2
maskList:
	.byte	63
	.byte	1
	.type	shiftList, @object
	.size	shiftList, 2
shiftList:
	.byte	1
	.byte	0
	.text
	.type	set_field, @function
set_field:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, %eax
	movb	%al, -8(%rbp)
	movl	-4(%rbp), %eax
	movzbl	maskList(%rax), %eax
	andb	-8(%rbp), %al
	movzbl	%al, %edx
	movl	-4(%rbp), %eax
	movzbl	shiftList(%rax), %eax
	movzbl	%al, %eax
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	set_field, .-set_field
	.globl	set_addr
	.type	set_addr, @function
set_addr:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	movzbl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$0, %edi
	call	set_field
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	set_addr, .-set_addr
	.section	.rodata
.LC0:
	.string	"Value = %d\n"
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
	movl	$3, %edi
	call	set_addr
	movb	%al, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
