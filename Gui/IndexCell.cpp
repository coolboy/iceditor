#include "StdAfx.h"
#include "IndexCell.h"

extern QRegExp regexp/*("((\"\\s+\")|\")")*/;

IndexCell::IndexCell(void):Id(-1), parentId(-1)
{
}

IndexCell::~IndexCell(void)
{
}

IndexCell::IndexCells IndexCell::Load( QStringList strLs, Transition::Transitions trans )
{
	IndexCells ret;

	foreach (QString str, strLs)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		ret[sls[1].toInt()].Id = sls[1].toInt();
		ret[sls[1].toInt()].MaxLifeTime = sls[2];
		ret[sls[1].toInt()].Name = sls[3];
	}

	foreach (Transition tran, trans)
	{
		ret[tran.targetId].parentId = tran.srcId; 
	}

	return ret;
}