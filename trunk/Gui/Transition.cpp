#include "StdAfx.h"
#include "Transition.h"

extern QRegExp regexp/*("((\"\\s+\")|\")")*/;

Transition::Transition(void):Id(-1),srcId(-1),targetId(-1)
{
}

Transition::~Transition(void)
{
}

Transition::Transitions Transition::load( QStringList strLs )
{
	Transitions ret;

	foreach (QString str, strLs)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		Transition tran;

		tran.Id = sls[1].toInt();
		tran.Type = sls[2];
		tran.srcId = sls[3].toInt();
		tran.targetId = sls[4].toInt();

		ret.push_back(tran);
	}

	return ret;
}