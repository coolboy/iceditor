#pragma once

class Ui_MemoryAdjust;

class MemAdjustWig :
	public QWidget
{
	Q_OBJECT

public:
	MemAdjustWig(QWidget* parent);
	~MemAdjustWig(void);

private:
	void setUI();
	void setConnections();

private Q_SLOTS:
	void on_okPB_clicked();

private:
	Ui_MemoryAdjust* m_ui;

	int run_id;
	QSettings test_in;
};
