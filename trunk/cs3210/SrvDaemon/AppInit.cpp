#include "StdAfx.h"
#include "AppInit.h"

AppInit::AppInit(void)
{
	QTextCodec::setCodecForCStrings ( QTextCodec::codecForLocale ());
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale ());
}

AppInit::~AppInit(void)
{
}
