#include "StdAfx.h"
#include "TreeLayout.h"

TreeLayout::TreeLayout( QRectF rec ):rct_(rec)
{}

TreeLayout::~TreeLayout( void )
{}

void TreeLayout::setTree( const Relations& rels )
{
	foreach(int id, rels.keys())
	{
		int dep = getDepth(id, rels, 0);
		if (dep + 1 > tls_.size())
			tls_.resize(dep + 1);

		tls_[dep].push_back(id);
	}
}

int TreeLayout::getDepth( int id, const Relations& rels, int dep )
{
	int parId = getParentId(id, rels);
	if (parId == -1 ||
		parId == id)//malform of xml
		return dep;
	else
		return getDepth(parId, rels, dep + 1);
}

int TreeLayout::getParentId( int id, const Relations& rels )
{
	if (rels.find(id) != rels.end	())
		return rels[id];
	else
		return -1;
}

QPointF TreeLayout::getPos( int id )
{
	int dep = -1;
	int index = -1;
	for (int i = 0; i != tls_.size(); ++i)
	{
		const QVector<int>& elems = tls_[i];
		for (int j = 0; j != elems.size(); ++j)
		{
			if (elems[j] == id)
			{
				dep = i;
				index = j;
				break;
			}
		}
	}

	if (dep == -1)
	{
		return QPointF(20.0, 20.0);
	}

	int maxDep = tls_.size();

	int hVal = rct_.height() / maxDep;
	int wVal = rct_.width() / tls_[dep].size();

	int xVal = wVal * index;
	int yVal = hVal * dep;

	if (xVal == 0)
		xVal += 20;

	if (yVal == 0)
		yVal += 20;

	return QPointF(xVal, yVal);
}