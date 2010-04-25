#include "StdAfx.h"

#include "memstat.hxx"
//#include "../shared.h"

#include "moc/moc_memstat.cpp"

MemStat::MemStat( QObject* parent ) 
	:QObject(parent), test_out("proc/vmguard/test_output", QSettings::IniFormat)
{

}

QVariant MemStat::getData(const QString &key)
{
	QVariant ret;

	test_out.sync();
	test_out.beginGroup(key);
	ret = test_out.value("num_faults", 0);
	test_out.endGroup();

	return ret;
}