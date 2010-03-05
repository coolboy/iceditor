#include "StdAfx.h"
#include "MemAdjustWig.hxx"
#include "moc/moc_MemAdjustWig.cpp"
#include "uic/ui_MemAdjustWig.h"

MemAdjustWig::MemAdjustWig(QWidget* parent):QWidget(parent)
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
	m_ui = new Ui_MemAdjust();

	m_ui->setupUi(this);
}

void MemAdjustWig::setConnections()
{

}