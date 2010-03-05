#pragma once

#include <QtGui/QtGui>

#include <qwt.h>

//在此处引用程序需要的其他宏
//定义这些宏，只是为了连接信号时候的便利.
#define QConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::connect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))

#define QDisConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::disconnect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))