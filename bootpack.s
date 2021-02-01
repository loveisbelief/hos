	.file	"bootpack.c"
	.text
	.globl	HariMain
	.def	HariMain;	.scl	2;	.type	32;	.endef
	.seh_proc	HariMain
HariMain:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
.L2:
	call	io_hlt
	jmp	.L2
	.seh_endproc
	.ident	"GCC: (tdm64-1) 9.2.0"
	.def	io_hlt;	.scl	2;	.type	32;	.endef
