#include "StdAfx.h"
#include "AppInit.h"

#include <ctime>

static void message_handler(QtMsgType msg_type, const char * msg)
{
	QString level_string;

	switch ( msg_type )
	{
	case QtDebugMsg:
		level_string = "debug";
		break;
	case QtWarningMsg:
		level_string = "warning";
		break;
	case QtCriticalMsg:
		level_string = "error";
		break;
	case QtFatalMsg:
		level_string = "fatal";
		std::abort();
	default:
		level_string = "unknown";
		break;
	}

	fprintf(stderr, "%s: %s\n",
		level_string.toAscii().constData(), msg);
}

AppInit::AppInit(void)
{
	QTextCodec::setCodecForCStrings ( QTextCodec::codecForLocale ());
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale ());
	//
	qApp->setApplicationName("ICEditor");
	//
	qInstallMsgHandler(message_handler);
	//
	qsrand(std::time(0));
}

AppInit::~AppInit(void)
{
}
