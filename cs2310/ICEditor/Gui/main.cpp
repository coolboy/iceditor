#include "StdAfx.h"

#include "mainwindow.hxx"

#include "AppInit.h"

#include "qrc/qrc_iceditor.cpp"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(iceditor);

    QApplication app(argv, args);

		AppInit appi;

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
