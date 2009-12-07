#include "StdAfx.h"

#include "IndexCell.h"
#include "XmlHelper.h"

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
		if (tran.Type == "internal")//betwenn indexcell inside the index system
			ret[tran.targetId].parentId = tran.srcId; 
	}

	return ret;
}

IndexCell::IndexCells IndexCell::Load( QIODevice& src )
{
	bool ret = src.open(QIODevice::ReadOnly);
	assert (ret);

	QStringList cellLst = XQuery2(src, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/IndexCell.xq"));
	qDebug()<< cellLst;

	src.seek(0);
	QStringList tLs = XQuery2(src, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/CellTransition.xq"));
	qDebug()<< tLs;

	Transition::Transitions trans = Transition::load(tLs);

	IndexCells cells = Load(cellLst, trans);

	for (IndexCells::iterator iter = cells.begin(); iter != cells.end(); ++iter)
	{
		src.seek(0);
		QString xq4tran = QString("doc($inputDocument)//indexSystem/indexCell[@id=\"%1\"]/transition/concat('\"', @id, '\" \"', @type, '\" \"', @source, '\" \"', @target, '\"')").arg(iter->Id);
		QString xq4state = QString("doc($inputDocument)//indexSystem/indexCell[@id=\"%1\"]/state/concat('\"', @id, '\" \"', @type, '\" \"', @name, '\"')").arg(iter->Id);
		QStringList ctLs = XQuery2(src, xq4tran);
		qDebug()<< ctLs;

		iter->strans = Transition::load(ctLs);

		src.seek(0);
		QStringList cstLs = XQuery2(src, xq4state);
		qDebug()<< cstLs;

		iter->states = State::Load(cstLs);
	}

	return cells;
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