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
		if (tran.Type == "external")
			ret[tran.targetId].parentId = tran.srcId; 
	}

	return ret;
}

IndexCell::IndexCells IndexCell::Load( QStringList strLs, 
																			Transition::Transitions trans, 
																			State::States states )
{
	assert (strLs.size() == 1);
	IndexCells ret = Load(strLs, trans);
	assert (ret.size() == 1);
	ret.begin().value().states = states;
	return ret;
}

IndexCell::State::States IndexCell::State::Load( QStringList strLs )
{
	IndexCell::State::States ret;

	foreach (QString str, strLs)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		IndexCell::State st;

		st.Id = sls[1].toInt();
		st.Type= sls[2];
		st.Name = sls[3];

		ret.push_back(st);
	}

	return ret;
}