.data #pre df str for "\n"
Enter:	.asciiz "
"
	  .text
	  .align 2
	  .globl main
  main:
	# BeginFunc 28
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 28	# decrement sp to make space for locals/temps
	# _tmp0 = 4
	  li $t0, 4		# load constant value 4 into $t0
	# intVal = _tmp0
	  move $t1, $t0		# copy value
	# _tmp1 = 0
	  li $t2, 0		# load constant value 0 into $t2
	# _tmp2 = intVal < _tmp1
	  slt $t3, $t1, $t2	
	# _tmp3 = 1
	  li $t4, 1		# load constant value 1 into $t4
	# _tmp4 = _tmp2 - _tmp3
	  sub $t5, $t3, $t4	
	# IfZ _tmp4 Goto _L0
	# (save modified registers before flow of control change)
	  sw $t0, -12($fp)	# spill _tmp0 from $t0 to $fp-12
	  sw $t1, -8($fp)	# spill intVal from $t1 to $fp-8
	  sw $t2, -16($fp)	# spill _tmp1 from $t2 to $fp-16
	  sw $t3, -20($fp)	# spill _tmp2 from $t3 to $fp-20
	  sw $t4, -24($fp)	# spill _tmp3 from $t4 to $fp-24
	  sw $t5, -28($fp)	# spill _tmp4 from $t5 to $fp-28
	  beqz $t5, _L0	# branch if _tmp4 is zero 
	# PushParam intVal
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# load intVal from $fp-8 into $t0
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintInt
	# jal _PrintInt     # jump to function
	  li	$v0	1			#choice print_int syscall
	  lw	$a0	4($sp)		#integer to print
	  syscall				#print the arg
	  li	$v0	4		#print_str
	  la	$a0	Enter		#address of "\n" to print
	  syscall				#print the arg
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
  _L0:
	# _tmp5 = "hello world"
	  .data			# create string constant marked with label
	  _string1: .asciiz "hello world"
	  .text
	  la $t0, _string1	# load label
	# PushParam _tmp5
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	# (save modified registers before flow of control change)
	  sw $t0, -32($fp)	# spill _tmp5 from $t0 to $fp-32
	#  jal _PrintString   	# jump to function
	  li	$v0	4			#print_str
	  la	$a0	($t0)		#address of entering to print
	  syscall				#print the arg
	  li	$v0	4		#print_str
	  la	$a0	Enter		#address of "\n" to print
	  syscall				#print the arg
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
