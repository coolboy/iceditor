#include "StdAfx.h"
#include "IndexCell.h"

extern QRegExp regexp/*("((\"\\s+\")|\")")*/;

IndexCell::IndexCell(void):Id(-1)
{
}

IndexCell::~IndexCell(void)
{
}

IndexCell::IndexCells IndexCell::Load( QStringList strLs )
{
	IndexCells ret;

	foreach (QString str, strLs)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		IndexCell ic;

		ic.Id = sls[1].toInt();
		ic.MaxLifeTime = sls[2];
		ic.Name = sls[3];

		ret.push_back(ic);
	}

	return ret;
}