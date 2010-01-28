#include "StdAfx.h"
#include <QtGui/QtGui>

#include "cpuplot.hxx"

int main(int argc, char **argv)
{
	QApplication app(argc, argv); 

	QWidget vBox;
	vBox.setWindowTitle("Cpu Plot");

	CpuPlot *plot = new CpuPlot(&vBox);
	plot->setTitle("History");
	plot->setMargin(5);

	QString info("Press the legend to en/disable a curve");

	QLabel *label = new QLabel(info, &vBox);

	QVBoxLayout *layout = new QVBoxLayout(&vBox);
	layout->addWidget(plot);
	layout->addWidget(label);

	vBox.resize(600,400);
	vBox.show();

	return app.exec();  
}   
