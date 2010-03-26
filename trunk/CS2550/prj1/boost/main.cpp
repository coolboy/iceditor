#include "stdafx.h"

#include "QueryTree.h"

int main()
{	
	std::string text = "UNION"
		"\t1 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t1,1 SCAN(DOCTOR)"
		"\t2 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t2,1 SELECT(PATIENT.SSN=PRESCRIPTION.Patient)";

	ParseQueryTree(text);

	return 0;
}
