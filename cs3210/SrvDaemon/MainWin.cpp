#include "StdAfx.h"
#include "MainWin.hxx"

#include "../shared.h"

#include "uic/ui_mainwin.h"
#include "moc/moc_mainwin.cpp"

MainWin::MainWin( QWidget* parent /*= 0*/ )
{
	m_ui = new Ui_MainWin();
	m_ui->setupUi(this);

	tcpServer = new QTcpServer(this);

	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotOnNewConnection()));
}

MainWin::~MainWin(void)
{
	delete m_ui;
}

void MainWin::on_actionExit_triggered()
{
	qApp->quit();
}

void MainWin::on_actionListen_triggered()
{
	if (!tcpServer->listen(QHostAddress::Any, defPort)) {
		QMessageBox::critical(this, tr("Guard Server"),
			tr("Unable to start the server: %1.")
			.arg(tcpServer->errorString()));
	}

	startTimer(1000);//1s
}

void MainWin::timerEvent( QTimerEvent * )
{
	if (conns.size() == 0)
		return;

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (quint16)0;
	out << m_ui->spinBox->text();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));

	qDebug() << m_ui->spinBox->text();

	foreach (QTcpSocket* clientConnection, conns)
	{
		clientConnection->write(block);
	}
}

void MainWin::slotOnNewConnection()
{
	QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

	conns.insert(clientConnection);

	connect(clientConnection, SIGNAL(disconnected()),
		this, SLOT(slotOnDisconnection()));
}

void MainWin::slotOnDisconnection()
{
	QTcpSocket *clientConnection = (QTcpSocket*)sender();
	if (clientConnection == 0)
		return;

	conns.remove(clientConnection);

	clientConnection->deleteLater();
}