#include "StdAfx.h"

#include "MainWin.hxx"
#include "cpuplot.hxx"

#include "moc/moc_MainWin.cpp"

MainWin::MainWin(void)
{
	setupUI();
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
	label_4 = new QLabel("Physical Memory", centralwidget);

	verticalLayout_2->addWidget(label_4);

	phyMemWig = new QTableView(centralwidget);

	verticalLayout_2->addWidget(phyMemWig);

	gridLayout_2->addLayout(verticalLayout_2, 0, 1, 3, 1);

	gridLayout_2->addWidget(cWig, 1, 0, 1, 1);

	gridLayout = new QGridLayout();
	testRateHS = new QSlider(centralwidget);
	testRateHS->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(testRateHS, 0, 0, 1, 1);

	label_2 = new QLabel("Test Rate", centralwidget);

	gridLayout->addWidget(label_2, 0, 1, 1, 1);

	faultHS = new QSlider(centralwidget);
	faultHS->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(faultHS, 1, 0, 1, 1);

	label_3 = new QLabel("Random Fault Rate", centralwidget);

	gridLayout->addWidget(label_3, 1, 1, 1, 1);


	gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

	setCentralWidget(centralwidget);
	menubar = new QMenuBar(this);
	menubar->setGeometry(QRect(0, 0, 800, 26));
	setMenuBar(menubar);
	statusbar = new QStatusBar(this);
	setStatusBar(statusbar);
}