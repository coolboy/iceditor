#pragma once

#include <QtGui/QtGui>

#include <qwt.h>

//�ڴ˴����ó�����Ҫ��������
//������Щ�ֻ꣬��Ϊ�������ź�ʱ��ı���.
#define QConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::connect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))

#define QDisConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::disconnect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))