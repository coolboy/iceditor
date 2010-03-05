#include "StdAfx.h"

#include <cassert>
#include <qwt_slider.h>

#include "MainWin.hxx"

#include "cpuplot.hxx"
#include "MemAdjustWig.hxx"

#include "moc/moc_MainWin.cpp"

MainWin::MainWin(void):memAdjustWig(0)
{
	setupUI();
	setConnections();
}

MainWin::~MainWin(void)
{
}

void MainWin::setupUI()
{
	setWindowTitle("Guard");

	cWig = new QWidget(this);

	CpuPlot *plot = new CpuPlot(cWig);
	plot->setTitle("History");
	plot->setMargin(5);

	QVBoxLayout *layout = new QVBoxLayout(cWig);
	layout->addWidget(plot);

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

	phyMemWig = new QTableWidget(centralwidget);
	phyMemWig->setRowCount(50);
	phyMemWig->setColumnCount(8);

	QStringList vhLst;

	for (int i = 0; i != 50; ++i)
	{
		vhLst<<QString::number(i * 8);
		for (int j = 0; j != 8; ++j)
		{
			QTableWidgetItem* item = new QTableWidgetItem();

			switch (qrand() % 5)
			{
			case 0:
				item->setBackground(QBrush(Qt::green));//free
				break;

			case 1:
				item->setBackground(QBrush(Qt::yellow));//tested
				break;

			case 2:
				item->setBackground(QBrush(Qt::red));//error
				break;

			case 3:
				item->setBackground(QBrush(Qt::gray));//unknow
				break;

			case 4:
				item->setBackground(QBrush(Qt::blue));//testing??
				break;

			default:
				assert(0);
				break;
			}
			phyMemWig->setItem(i, j, item);
		}
	}

	phyMemWig->setHorizontalHeaderLabels(QStringList()
		<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7");

	phyMemWig->setVerticalHeaderLabels(vhLst);

	verticalLayout_2->addWidget(phyMemWig);

	gridLayout_2->addLayout(verticalLayout_2, 0, 1, 3, 1);

	gridLayout_2->addWidget(cWig, 1, 0, 1, 1);

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

	menubar = new QMenuBar(this);
	//menubar->setGeometry(QRect(0, 0, 800, 26));
	setMenuBar(menubar);

	statusbar = new QStatusBar(this);
	setStatusBar(statusbar);
	//statusbar->showMessage("123");
}

void MainWin::setConnections()
{
	QConnect (memAdjustBut, clicked(), this, slotOnMemAdjustBut());
}

void MainWin::slotOnMemAdjustBut()
{
	if (memAdjustWig == 0)
		memAdjustWig = new MemAdjustWig(0);

	memAdjustWig->show();
}
