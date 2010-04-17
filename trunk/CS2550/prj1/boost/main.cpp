#include "stdafx.h"

#include "QueryTree.h"
//test
int main()
{	
	std::string text = "(PATIENT.FirstName='Michael' AND PATIENT.FirstName=DOCTOR.FirstName)";

	QueryTreeNodePtr root = ParseQueryTree(text);

	return 0;
}
