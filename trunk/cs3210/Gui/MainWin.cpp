#include "StdAfx.h"

#include <cassert>
#include <qwt_slider.h>

#include "../shared.h"

#include "MainWin.hxx"
#include "memplot.hxx"
#include "memstat.hxx"
#include "MemAdjustWig.hxx"

#include "moc/moc_MainWin.cpp"

MainWin::MainWin(void):memAdjustWig(0)
{
	setupUI();
	setConnections();
}

MainWin::~MainWin(void)
{
	delete memAdjustWig;
}

void MainWin::setupUI()
{
	setWindowTitle("Guard");

	plot = new MemPlot();
	plot->setTitle("History");
	plot->setMargin(5);

	plotLayout = new QVBoxLayout();
	plotLayout->addWidget(plot);

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
	bool ok = false;
	QString srvAddr = QInputDialog::getText(this, tr("Input server address:"),
		tr("ip:port"), QLineEdit::Normal,	defHost + ':' + QString::number(defPort), &ok);
	if (ok && !srvAddr.isEmpty()){
		QStringList lst = srvAddr.split(":", QString::SkipEmptyParts);
		if (lst.size() != 2)
			return;

		QString host = lst[0];
		quint16 port = lst[1].toUInt();

		MemStat *memStat = new MemStat(this, host, port);
		memStat->setDebugOutput(statusBar());
		memStat->grapMsg();

		plot->setDataSrc(memStat);
	}
}