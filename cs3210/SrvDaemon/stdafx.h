// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <cstdio>
#include <tchar.h>

//在此处引用程序需要的其他宏
//定义这些宏，只是为了连接信号时候的便利.
#define QConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::connect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))

#define QDisConnect(Sender_,Signal_,Receiver_,Member_)\
	QObject::disconnect(Sender_,SIGNAL(Signal_),Receiver_,SLOT(Member_))


//在此处引用程序需要的其他头文件
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <QtGui/QtGui>