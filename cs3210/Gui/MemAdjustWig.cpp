#include "StdAfx.h"
#include "MemAdjustWig.hxx"

#include "uic/ui_MemAdjustWig.h"
#include "moc/moc_MemAdjustWig.cpp"

MemAdjustWig::MemAdjustWig(QWidget* parent):
	QWidget(parent), 
	run_id(0),
	test_in("proc/vmguard/test_input", QSettings::IniFormat)
{
	setUI();
	setConnections();
}

MemAdjustWig::~MemAdjustWig(void)
{
	delete m_ui;
}

void MemAdjustWig::setUI()
{
	m_ui = new Ui_MemoryAdjust();

	m_ui->setupUi(this);

	test_in.sync ();
	m_ui->numDimmSB->setValue(test_in.value("num_dimm", 0).toInt());
	m_ui->capSB->setValue(test_in.value("capacity", 0).toInt());
	m_ui->druationSB->setValue(test_in.value("sim_druation", 0).toInt());
	m_ui->tempSB->setValue(test_in.value("temperature", 0).toInt());
	m_ui->guassSB->setValue(test_in.value("guass_poisson", 0).toInt());
}

void MemAdjustWig::setConnections()
{

}

void MemAdjustWig::on_okPB_clicked()
{
	hide();

	test_in.setValue("run_id", run_id);
	test_in.setValue("num_dimm", m_ui->numDimmSB->value());
	test_in.setValue("capacity", m_ui->capSB->value());
	test_in.setValue("sim_druation", m_ui->druationSB->value());
	test_in.setValue("temperature", m_ui->tempSB->value());
	test_in.setValue("guass_poisson", m_ui->guassSB->value());
	test_in.sync ();
	++run_id;
}