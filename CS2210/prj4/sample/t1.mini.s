.data
Enter:	.asciiz "  
"
base:
.text
.globl main
.data
	.word	-1			#offset 0
.text
main:					#method entry
	la	$28	base		#store global area address into $gp
	move	$t1	$28		#init base
	add	$t1	$t1	0	#init main access link in $t1; .data 0
	li	$t2	0		#init $t2 as 0-global access
	move	$fp	$sp		#init fp pointer
	sw	$ra	0($sp)		#save return address on stack
	addi	$sp	$sp	-4	#increase st
	li	$11	4		#load immediate 4 to t11
	sw	$11	0($sp)		#store t11 to stack top
	addi	$sp	$sp	-4	#increase st
	li	$11	4		#load offset in $11
	sub	$11	$fp	$11	#absoult addr by $fp
	lw	$12	($11)		#load value from ($11) to t12
	sw	$12	0($sp)		#push value on stack
	addi	$sp	$sp	-4	#increase st
	li	$11	0		#exp NUMNode $11<-imme
	sw	$11	0($sp)		#push imme on stack
	addi	$sp	$sp	-4	#increase st
	lw	$11	4($sp)		#stack top -> $11
	addi	$sp	$sp	4	#pop st
	lw	$12	4($sp)		#stack top -> $12
	sge	$11	$12	$11	#add r1 to r2
	sw	$11	4($sp)		#push the sum on stack
	lw	$11	4($sp)		#store boolean result in register
	beqz	$11	L_1		#if false, jump to L_1
	li	$11	0		#load offset in $11
	add	$11	$11	0	#offr+field relative addr
	add	$11	$11	$gp		#class.---, add to t1 base
	move	$25	$11		#$offr=>$t25, base address
	move	$24	$t2		#$offr=>$t24, flag
	sw	$24	0($sp)		#push base $t2
	addi	$sp	$sp	-4	#push st
	sw	$25	0($sp)		#push base $t1
	addi	$sp	$sp	-4	#push st
	li	$11	4		#load offset in $11
	sub	$11	$fp	$11	#absoult addr by $fp
	lw	$12	($11)		#load value from ($11) to t12
	sw	$12	0($sp)		#push value on stack
	addi	$sp	$sp	-4	#increase st
	li	$v0	1		#print_int
	lw	$a0	4($sp)		#integer to print
	syscall				#print the arg
	li	$v0	4		#print_str
	la	$a0	Enter		#address of entering to print
	syscall				#print the arg
.data
.align 2
S_35:	.asciiz	"hello world"
.text
	li	$11	0		#load offset in $11
	add	$11	$11	0	#offr+field relative addr
	add	$11	$11	$gp		#class.---, add to t1 base
	move	$25	$11		#$offr=>$t25, base address
	move	$24	$t2		#$offr=>$t24, flag
	sw	$24	0($sp)		#push base $t2
	addi	$sp	$sp	-4	#push st
	sw	$25	0($sp)		#push base $t1
	addi	$sp	$sp	-4	#push st
	li	$v0	4		#print_str
	la	$a0	S_35		#address of string to print
	syscall				#print the arg
	b	L_0			#true branch end, jump to end_if
L_1:				#false label
L_0:					#end if
	lw	$ra	0($fp)		#get back control line
	move	$sp	$fp		#pop stack to fp
	jr	$ra			#routine call return
