#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
	int putString(const char* str, int bCase);

	const char* getString(int index);

	void printTable();
#ifdef __cplusplus
};
#endif