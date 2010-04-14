#include "StdAfx.h"

#include "MainWin.hxx"
#include "TestConf.hxx"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	//TestConf tc;

	MainWin mw;
	mw.show();

	return app.exec();  
}   
