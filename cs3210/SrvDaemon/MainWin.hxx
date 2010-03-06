#pragma once

#include <QtGui/QtGui>

class Ui_MainWin;

class MainWin : public QMainWindow
{
	Q_OBJECT
public:
	MainWin(QWidget* parent = 0);
	~MainWin(void);

private:
	Ui_MainWin* m_ui;
};
