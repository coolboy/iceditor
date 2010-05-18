#include "RegisterAllocator.h"


RegisterAllocator::RegisterAllocator(void)
{
	freeRegs.push_back ("r16");
	freeRegs.push_back ("r17");
	freeRegs.push_back ("r18");
	freeRegs.push_back ("r19");
	freeRegs.push_back ("r20");
	freeRegs.push_back ("r21");
	freeRegs.push_back ("r22");
	freeRegs.push_back ("r23");
}


RegisterAllocator::~RegisterAllocator(void)
{
}

std::string RegisterAllocator::getOne()
{
	std::string ret;
	if (freeRegs.empty())
		return ret;
	else{
		ret = freeRegs.at(freeRegs.size() - 1);
		freeRegs.pop_back();
		return ret;
	}
}
