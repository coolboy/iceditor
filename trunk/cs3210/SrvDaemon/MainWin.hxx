#pragma once

class Ui_MainWin;

class MainWin : public QMainWindow
{
	Q_OBJECT
public:
	MainWin(QWidget* parent = 0);
	~MainWin(void);

private Q_SLOTS:
	void on_actionExit_triggered();
	void on_actionListen_triggered();

	void slotOnNewConnection();
	void slotOnDisconnection();

protected:
	void timerEvent(QTimerEvent *);

private:
	Ui_MainWin* m_ui;

  QTcpServer *tcpServer;
	QSet<QTcpSocket*> conns;
};
