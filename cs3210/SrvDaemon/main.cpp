#include "stdafx.h"

#include "AppInit.h"

#include "MainWin.hxx"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AppInit init;

	MainWin mw;

	mw.show();

	return app.exec();
}