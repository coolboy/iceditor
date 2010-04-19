#include "StdAfx.h"
#include "asmrefiner.h"


AsmRefiner::AsmRefiner(void)
{
}


AsmRefiner::~AsmRefiner(void)
{
}

void AsmRefiner::setDecafAsm( const std::string& dasm )
{
	decafAsm_ = dasm;
}

std::string AsmRefiner::getMiniJavaAsm()
{
	transform();
	return miniJavaAsm_;
}

void AsmRefiner::transform()
{
	/* headr refine
	* - # standard Decaf preamble //maybe delete this line in dcc source?
	* + .data #pre df str for "\n" //this for the predefined "\n"
	* + Enter:	.asciiz "
	* + "
	*/

	/* Replace PrintInt
	* -  jal _PrintInt      	# jump to function
	* + li	$v0	1			#choice print_int syscall
	* + lw	$a0	4($sp)		#integer to print
	* + syscall				#print the arg
	* + li	$v0	4		#print_str
	* + la	$a0	Enter		#address of "\n" to print
	* + syscall				#print the arg
	*/

	/* Replace PrintString
	* -  jal _PrintString      	# jump to function
	* + li	$v0	4			#print_str
	* + la	$a0	($t0)		#address of entering to print
	* + syscall				#print the arg
	* + li	$v0	4		#print_str
	* + la	$a0	Enter		#address of "\n" to print
	* + syscall				#print the arg
	*/

	/* Replace _Alloc
	*  jal _Alloc         	# jump to function
	* ->
	* li	$v0	9			#choice new syscall
	* move	$a0	$t0		#size to alloc
	* syscall				#print the arg
	*/
}
