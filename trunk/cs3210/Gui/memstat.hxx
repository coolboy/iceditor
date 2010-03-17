#pragma once

class MemStat : public QObject
{
	Q_OBJECT
public:
	MemStat(QObject* parent, QString srvHost, quint16 srvPort);

	QVariant getData(const QString &key);
	void grapMsg();

	void setDebugOutput(QStatusBar* bar);

//private:

private Q_SLOTS:
	void slotOnError(QAbstractSocket::SocketError socketError);
	void slotReadMsg();

private:
	QString host;
	quint16 port;

	QTcpSocket *tcpSocket;
	QString currentFortune;//多线程保护
	quint16 blockSize;

	//gui
	QStatusBar *qsb;
};
