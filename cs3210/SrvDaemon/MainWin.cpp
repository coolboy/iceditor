#include "StdAfx.h"
#include "MainWin.hxx"

#include "uic/ui_mainwin.h"
#include "moc/moc_mainwin.cpp"

MainWin::MainWin( QWidget* parent /*= 0*/ )
{
	m_ui = new Ui_MainWin();
	m_ui->setupUi(this);
}

MainWin::~MainWin(void)
{
	delete m_ui;
}
