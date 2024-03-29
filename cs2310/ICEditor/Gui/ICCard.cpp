#include "StdAfx.h"
#include "ICCard.h"

#include <cassert>

QRegExp regexp("((\"\\s+\")|\")");

ICCard::Pattern Str2Pattern(QString str)
{
	if (str == "quietstate")
		return ICCard::QuietState;
	else if (str == "myselfnone")
		return ICCard::MyselfNone;
	else if (str == "myselfwith")
		return ICCard::MyselfWith;
	else if (str == "othersnone")
		return ICCard::OthersNone;
	else if (str == "otherswith")
		return ICCard::OthersWith;
	else if (str == "mixedstate")
		return ICCard::MixedState;
	else
	{
		assert(0);
		return ICCard::Undef;
	}
}

ICCard::ICCard(void)
{
	Id = abs(qrand());
	parentId = -1;
	IntPattern = Undef;
	NumberCurrent = -1;
	NumberTotal = -1;
	otherId = -1;
}

ICCard::~ICCard(void)
{
}

ICCard::ICCards ICCard::Load( QStringList icdb, QStringList icci )
{
	ICCards iccards;

	foreach (QString str, icdb)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;
		ICCard iccard;
		iccard.Id = sls[1].toInt();
		iccard.Name = sls[2]; 
		iccard.Description = sls[3];
		iccard.IntPattern = Str2Pattern(sls[4]);
		iccard.Task = sls[5];
		iccard.TimeCriticalCondition = sls[6];
		iccard.NumberCurrent = sls[7].toInt();
		iccard.NumberTotal = sls[8].toInt();
		iccard.otherId = sls[9].toInt();
		iccard.OtherName = sls[10];
		iccard.OtherMessage = sls[11];
		iccard.OtherTask = sls[12];
		//icentry
		iccard.icEntryId = sls[13];
		iccard.icEntryName = sls[14];

		iccards[iccard.Id] = iccard;
	}

	foreach (QString str, icci)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		iccards[sls[1].toInt()].parentId = sls[2].toInt();
		iccards[sls[1].toInt()].scenario = sls[3];
		iccards[sls[1].toInt()].icSystem = sls[4];
		iccards[sls[1].toInt()].icGroup = sls[5];
	}

	return iccards;
}

QString ICCard::getSimpleStr() const
{
	return QString("%1 %2").arg(Id).arg(Name);
}