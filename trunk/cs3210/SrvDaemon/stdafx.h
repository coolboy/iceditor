// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <cstdio>
#include <tchar.h>

//�ڴ˴����ó�����Ҫ��������
//������Щ�ֻ꣬��Ϊ�������ź�ʱ��ı���.
#define QConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::connect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))

#define QDisConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::disconnect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))


//�ڴ˴����ó�����Ҫ������ͷ�ļ�
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <QtGui/QtGui>