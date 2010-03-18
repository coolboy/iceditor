#include "StdAfx.h"

#include "memstat.hxx"
//#include "../shared.h"

#include "moc/moc_memstat.cpp"

MemStat::MemStat(QObject* parent, QString srvHost, quint16 srvPort)
	:QObject(parent), currentFortune("0")
{
	host = srvHost;
	port = srvPort;

	qsb = 0;

	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadMsg()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(slotOnError(QAbstractSocket::SocketError)));
}

QVariant MemStat::getData(const QString &key)
{
	return currentFortune.toUInt();//decode the data
}

void MemStat::grapMsg()
{
	blockSize = 0;
	tcpSocket->abort();
	tcpSocket->connectToHost(host, port);
}

void MemStat::slotReadMsg()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_0);

	if (blockSize == 0) {
		if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
			return;

		in >> blockSize;
	}

	if (tcpSocket->bytesAvailable() < blockSize)
		return;

	QString nextFortune;
	in >> nextFortune;

	blockSize = 0; //clear

	//if (nextFortune == currentFortune) {
		//QTimer::singleShot(0, this, SLOT(requestNewMsg()));
		//return;
	//}

	currentFortune = nextFortune;
}

void MemStat::slotOnError(QAbstractSocket::SocketError socketError)
{
	if (qsb == 0)
		return;

	switch (socketError) {
		case QAbstractSocket::RemoteHostClosedError:
			qsb->showMessage(tr("The remote host closed the connection"));
				break;
		case QAbstractSocket::HostNotFoundError:
			qsb->showMessage(tr("The host was not found. Please check the "
				"host name and port settings."));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			qsb->showMessage(tr("The connection was refused by the peer. "
				"Make sure the guard server is running, "
				"and check that the host name and port "
				"settings are correct."));
			break;
		default:
			qsb->showMessage( tr("The following error occurred: %1.")
				.arg(tcpSocket->errorString()));
	}
}

void MemStat::setDebugOutput( QStatusBar* bar )
{
	qsb = bar;
}