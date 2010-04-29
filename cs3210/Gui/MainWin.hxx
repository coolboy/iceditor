#pragma once

class QwtSlider;
class MemAdjustWig;
class MemPlot;

class MainWin : public QMainWindow
{
	Q_OBJECT

public:
	MainWin(void);
	~MainWin(void);

private Q_SLOTS:
	void slotOnMemAdjustBut();

	//menu
	void slotOnConnect();

	//Physical Layout Double Clicked
	void slotOnCellDoubleClicked( int row, int column );

	//Physical User Enter
	void slotOnAddressSpinEnter();

private:
	void setupUI();
	void setConnections();

private:
	int beginAddress;
	QTableWidgetItem* lastItem;

private:
	QWidget *centralwidget;

	QVBoxLayout *plotLayout;
	MemPlot *memPlot;

	QGridLayout *gridLayout_2;
	QVBoxLayout *verticalLayout;
	QLabel *label;
	QLineEdit *pidLE;
	QVBoxLayout *verticalLayout_2;
	QLabel *label_4;

	QLabel* AddressInfoLab;
	QSpinBox* addressSpin;
	QTableWidget *phyMemWig;

	QGridLayout *gridLayout;
	QwtSlider *testRateHS;
	QLabel *label_2;
	QwtSlider *faultHS;
	QLabel *label_3;
	//QStatusBar *statusbar;

	QPushButton *memAdjustBut;

	MemAdjustWig *memAdjustWig;

	//menu
	QMenu* fileMenu;
	QAction* connectAct;
	QAction* exitAct;
};
