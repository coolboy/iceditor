#include "StdAfx.h"
#include "asmrefiner.h"

//#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace boost::algorithm;

AsmRefiner::AsmRefiner(void){}

AsmRefiner::~AsmRefiner(void){}

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
	/* header refine
	*/

	miniJavaAsm_ = decafAsm_;

	/* Replace PrintInt
	* -  jal _PrintInt      	# jump to function
	* + li	$v0	1			#choice print_int syscall
	* + lw	$a0	4($sp)		#integer to print
	* + syscall				#print the arg
	*/

	replace_all(miniJavaAsm_, "jal _PrintInt      	# jump to function", 
		"li	$v0	1			#choice print_int syscall\n"
		"\tlw	$a0	4($sp)		#integer to print\n"
		"\tsyscall				#print the arg");

	/* Replace ReadInteger
	* jal _ReadInteger   	# jump to function
	* ->
	* li	$v0	5		#system call code for read_int
	* syscall
	*/

	replace_all(miniJavaAsm_, "jal _ReadInteger   	# jump to function", 
		"li	$v0	5		#system call code for read_int\n"
		"\tsyscall");

	/* Replace PrintString
	* -  jal _PrintString   	# jump to function
	* + li	$v0	4			#print_str
	* + la	$a0	($t0)		#address of entering to print
	* + syscall				#print the arg
	*/

	replace_all(miniJavaAsm_, "jal _PrintString   	# jump to function", 
		"li	$v0	4			#print_str\n"
		"\tla	$a0	($t0)		#address of entering to print\n"
		"\tsyscall				#print the arg");

	/* Replace _Alloc
	*  jal _Alloc         	# jump to function
	* ->
	* li	$v0	9			#choice new syscall
	* move	$a0	$t0		#size to alloc
	* syscall				#alloc memory
	*/

	replace_all(miniJavaAsm_, "jal _Alloc         	# jump to function", 
		"li	$v0	9			#choice new syscall\n"
		"\tmove	$a0	$t0		#size to alloc\n"
		"\tsyscall				#alloc memory");

	/*Replace _Halt
	* jal _Halt          	# jump to function
	* ->
	* li	$v0	10			#choice new syscall
	* syscall				#exit
	*/

	replace_all(miniJavaAsm_, "jal _Halt          	# jump to function", 
		"li	$v0	10 		#choice new syscall\n"
		"\tsyscall				#exit");
}
