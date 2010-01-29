#pragma once

#include <qwt_slider.h>

class MainWin : public QMainWindow
{
	Q_OBJECT

public:
	MainWin(void);
	~MainWin(void);

private:
	void setupUI();

private:
	QWidget* cWig;

	QWidget *centralwidget;
	QGridLayout *gridLayout_2;
	QVBoxLayout *verticalLayout;
	QLabel *label;
	QLineEdit *pidLE;
	QVBoxLayout *verticalLayout_2;
	QLabel *label_4;
	QTableWidget *phyMemWig;
	QGridLayout *gridLayout;
	QwtSlider *testRateHS;
	QLabel *label_2;
	QwtSlider *faultHS;
	QLabel *label_3;
	QMenuBar *menubar;
	QStatusBar *statusbar;
};
