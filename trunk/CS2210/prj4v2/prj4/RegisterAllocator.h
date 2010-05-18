#pragma once

#include <vector>
#include <string>

typedef std::vector<std::string> RegNames;

class RegisterAllocator
{
public:
	RegisterAllocator(void);
	~RegisterAllocator(void);

	std::string getOne();
private:
	RegNames freeRegs;
};

