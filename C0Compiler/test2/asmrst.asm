	.text
		ori	$fp	$sp	0
		li	$t9	0x7fffeffc	#global stack bottom
		li	$t8	0x10010000	#save word
		j	__main
kid:
		#Save Register
		sw	$fp	($sp)
		add	$fp	$sp	$0
		subi	$sp	$sp	4
		sw	$ra	($sp)
		subi	$sp	$sp	4
		#Save Register Done!
		li	$t0	0	#$_0
		sw	$t0	($sp)
		subi	$sp	$sp	4
		lw	$t0	4($fp)
		li	$t1	1
		bne	$t0	$t1	__tLABEL0
		li	$t0	1
		j	__tLABEL1
__tLABEL0:
		li	$t0	0
__tLABEL1:
		bne	$t0	1	_LABEL_0
		lw	$t0	4($fp)
		li	$t1	1
		add	$t0	$t0	$t1
		sw	$t0	-8($fp)
		lw	$t0	-8($fp)
		sw	$t0	4($fp)
		j	_LABEL_1
_LABEL_0:
_LABEL_1:
__FEND_LAB_1:
		lw	$ra	-4($fp)
		add	$sp	$fp	$0
		lw	$fp	($fp)
		jr	$ra	
__main:
		#Save Register
		sw	$fp	($sp)
		add	$fp	$sp	$0
		subi	$sp	$sp	4
		sw	$ra	($sp)
		subi	$sp	$sp	4
		#Save Register Done!
		li	$t0	0	#main
		sw	$t0	($sp)
		subi	$sp	$sp	4
		li	$t0	0
		sw	$t0	-8($fp)
		lw	$t0	-8($fp)
		li	$t1	0
		bne	$t0	$t1	__tLABEL2
		li	$t0	1
		j	__tLABEL3
__tLABEL2:
		li	$t0	0
__tLABEL3:
		bne	$t0	1	_LABEL_2
		lw	$t0	-8($fp)
		sw	$t0	($sp)
		subi	$sp	$sp	4
		jal	kid
		nop
		j	_LABEL_3
_LABEL_2:
_LABEL_3:
__FEND_LAB_2:
		lw	$ra	-4($fp)
		add	$sp	$fp	$0
		lw	$fp	($fp)
		li	$v0	10
		syscall
