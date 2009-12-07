#pragma once

typedef QMap<int, int> Relations;//id parid

class TreeLayout
{
public:
	TreeLayout(QRectF rec);
	~TreeLayout(void);

	void setTree(const Relations& rels);
	QPointF getPos(int id);

private://treeHelper
	int getDepth(int id, const Relations& rels, int dep);
	int getParentId(int id, const Relations& rels);

private:
	QRectF rct_;
	typedef QVector<QVector<int> > TreeLevels;
	TreeLevels tls_;
};
