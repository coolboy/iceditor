#include "StdAfx.h"
#include "IndexSystem.h"

extern QRegExp regexp/*("((\"\\s+\")|\")")*/;

IndexSystem::IndexSystems IndexSystem::Load( QStringList strLs )
{
	IndexSystems ret;

	foreach (QString str, strLs)
	{
		QStringList sls = str.split(regexp);
		qDebug()<<sls;

		IndexSystem is;

		is.Id = sls[1].toInt();
		is.Name = sls[2];
		is.Description = sls[3];

		ret.push_back(is);
	}

	return ret;
}