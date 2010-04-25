#include "StdAfx.h"

#include <fstream>

#include "memstat.hxx"
//#include "../shared.h"

#include "moc/moc_memstat.cpp"

MemStat::MemStat( QObject* parent ) 
	:QObject(parent), 
	test_out("proc/vmguard/test_output", QSettings::IniFormat)
	//system_alloc_rate("proc/vmguard/system_alloc_rate", QSettings::IniFormat),
	//test_rate("proc/vmguard/test_rate", QSettings::IniFormat)
{

}

QVariant MemStat::getData(const QString &key)
{
	QVariant ret;

	if (key == "TestRate")
	{
		double out = 0;
		std::ifstream ifs("proc/vmguard/test_rate");
		ifs >> out;
		ret = out;

	}
	else if (key == "SysAllocRate")
	{
		double out = 0;
		std::ifstream ifs("proc/vmguard/system_alloc_rate");
		ifs >> out;
		ret = out;
	}
	else
	{
		test_out.sync();
		test_out.beginGroup(key);
		ret = test_out.value("num_faults", 0);
		test_out.endGroup();
	}

	return ret;
}