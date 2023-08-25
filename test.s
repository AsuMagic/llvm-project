	.text
	.file	"test.c"
	.globl	test                            ; -- Begin function test
	.type	test,@function
test:                                   ; @test
; %bb.0:                                ; %entry
	iadd	r0, r1
	iadd	r0, r2
	iadd	r0, r3
	iadd	r0, r4
	iadd	r0, r5
	iadd	r0, r6
	ret
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
                                        ; -- End function
	.ident	"clang version 18.0.0 (git@github.com:AsuMagic/llvm-project.git 572afe044482f9ab4429ba4a0867024ae1a6b8cc)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
