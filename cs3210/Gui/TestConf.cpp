#include "StdAfx.h"
#include "TestConf.hxx"


TestConf::TestConf(void):
	test_in("test_input", QSettings::IniFormat), 
	test_out("test_output", QSettings::IniFormat)
{
	test_out.beginGroup("Date");
	test_out.setValue("month", 1);
	test_out.endGroup();

	test_out.beginGroup("Dimm0");
	test_out.setValue("num_faults", 123);
	test_out.endGroup();

	test_out.beginGroup("Dimm1");
	test_out.setValue("num_faults", 123);
	test_out.endGroup();

	test_out.beginGroup("Dimm2");
	test_out.setValue("num_faults", 123);
	test_out.endGroup();

	test_out.beginGroup("Dimm3");
	test_out.setValue("num_faults", 123);
	test_out.endGroup();

	test_in.setValue("run_id", 3);
	test_in.setValue("num_dimm", 4);
	test_in.setValue("capacity", 1024);
	test_in.setValue("sim_druation", 12);
	test_in.setValue("temperature", 78.6);
	test_in.setValue("guass_poisson", 1);

	test_in.sync();
	test_out.sync();
}


TestConf::~TestConf(void)
{
}
