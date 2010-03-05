#pragma once

class Ui_MemAdjust;

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

private:
	Ui_MemAdjust* m_ui;
};
