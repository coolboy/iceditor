#include "StdAfx.h"

#include <cassert>
#include <qwt_slider.h>

#include "../shared.h"

#include "MainWin.hxx"
#include "memplot.hxx"
#include "memstat.hxx"
#include "MemAdjustWig.hxx"

#include "moc/moc_MainWin.cpp"

static const int memRange = 10e6/(16*16);

MainWin::MainWin(void):memAdjustWig(0)
{
	beginAddress = 0;

	setupUI();
	setConnections();

	MemStat *memStat = new MemStat(this);
	memPlot->setDataSrc(memStat);
}

MainWin::~MainWin(void)
{
	delete memAdjustWig;
}

void MainWin::setupUI()
{
	setWindowTitle("Guard Client");

	memPlot = new MemPlot();
	memPlot->setTitle("History");
	memPlot->setMargin(5);

	plotLayout = new QVBoxLayout();
	plotLayout->addWidget(memPlot);

	centralwidget = new QWidget(this);
	gridLayout_2 = new QGridLayout(centralwidget);
	verticalLayout = new QVBoxLayout();
	label = new QLabel("PID", centralwidget);

	verticalLayout->addWidget(label);

	pidLE = new QLineEdit(centralwidget);

	verticalLayout->addWidget(pidLE);

	gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

	verticalLayout_2 = new QVBoxLayout();
	label_4 = new QLabel("Physical Memory Page Layout", centralwidget);

	verticalLayout_2->addWidget(label_4);

	int endAddress = beginAddress + memRange - 1;

	QString addressInfo = QString("Range: %1 - %2")
		.arg(beginAddress).arg(endAddress);

	AddressInfoLab = new QLabel(addressInfo, this);
	verticalLayout_2->addWidget(AddressInfoLab);

	addressSpin = new QSpinBox(this);
	addressSpin->setMinimum(0);
	addressSpin->setMaximum(memRange);
	verticalLayout_2->addWidget(addressSpin);

	phyMemWig = new QTableWidget(centralwidget);
	phyMemWig->setRowCount(16);
	phyMemWig->setColumnCount(16);


	for (int i = 0; i != 16; ++i)	{
		for (int j = 0; j != 16; ++j){
			QTableWidgetItem* item = new QTableWidgetItem();
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			phyMemWig->setItem(i, j, item);
		}
	}

	QStringList hhLst;
	for (int i = 0; i != 16; ++i)
		hhLst<<QString::number(i);

	phyMemWig->setHorizontalHeaderLabels(hhLst);

	phyMemWig->setVerticalHeaderLabels(hhLst);

	verticalLayout_2->addWidget(phyMemWig);

	gridLayout_2->addLayout(verticalLayout_2, 0, 1, 3, 1);

	gridLayout_2->addLayout(plotLayout, 1, 0, 1, 1);

	gridLayout = new QGridLayout();
	testRateHS = new QwtSlider(centralwidget, Qt::Horizontal, QwtSlider::TopScale, QwtSlider::BgTrough);
	testRateHS->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(testRateHS, 0, 0, 1, 1);

	label_2 = new QLabel("Test Rate", centralwidget);

	gridLayout->addWidget(label_2, 0, 1, 1, 1);

	faultHS = new QwtSlider(centralwidget, Qt::Horizontal, QwtSlider::TopScale, QwtSlider::BgTrough);
	faultHS->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(faultHS, 1, 0, 1, 1);

	label_3 = new QLabel("Random Fault Rate", centralwidget);

	gridLayout->addWidget(label_3, 1, 1, 1, 1);


	gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

	memAdjustBut = new QPushButton("Memory Adjust", this);

	gridLayout_2->addWidget(memAdjustBut, 3, 1);

	setCentralWidget(centralwidget);

	connectAct = new QAction("Connect", this);
	exitAct = new QAction("Exit", this);

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(connectAct);
	fileMenu->addAction(exitAct);
}

void MainWin::setConnections()
{
	QConnect (memAdjustBut, clicked(), this, slotOnMemAdjustBut());
	QConnect (connectAct, triggered(), this, slotOnConnect());

	QConnect (phyMemWig, cellDoubleClicked ( int, int), this, slotOnCellDoubleClicked(int, int));
	QConnect (addressSpin, editingFinished(), this, slotOnAddressSpinEnter());

	QConnect (exitAct, triggered(), qApp, quit());
}

void MainWin::slotOnMemAdjustBut()
{
	if (memAdjustWig == 0)
		memAdjustWig = new MemAdjustWig(0);

	memAdjustWig->show();
}

void MainWin::slotOnConnect()
{
}

void MainWin::slotOnCellDoubleClicked( int row, int column )
{
	beginAddress = (column + row * 16) * memRange;
	int endAddress = beginAddress + memRange - 1;

	QString addressInfo = QString("Range: %1 - %2")
		.arg(beginAddress).arg(endAddress);

	AddressInfoLab->setText(addressInfo);
}

void MainWin::slotOnAddressSpinEnter()
{
	//////////////////////////////////////////////////////////////////////////
	//syscall
	int address = beginAddress + addressSpin->value();
}