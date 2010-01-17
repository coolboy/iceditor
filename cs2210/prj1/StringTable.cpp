#include "StringTable.h"

#include <vector>
#include <string>
#include <algorithm>

typedef std::vector<std::string> StringTable;
StringTable strTable;

int putString(const char* str, int bCase)
{
	std::string val = str;
	if (bCase == 0)
		std::transform(val.begin(), val.end(), val.begin(), tolower);

	StringTable::iterator index = std::find(strTable.begin(), strTable.end(), val);

	if (index == strTable.end())
	{
		strTable.push_back(val);
		return strTable.size() - 1;
	}
	else
		return std::distance(strTable.begin(), index );
}

const char* getString(int index)
{
	return strTable[index].c_str();
}

void printTable()
{
	printf("String Table : ");
	for (StringTable::iterator iter = strTable.begin(); iter != strTable.end(); ++iter)
		printf("%s ", iter->c_str());
	printf("\n");
}