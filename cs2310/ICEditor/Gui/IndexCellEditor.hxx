#pragma once

#include "ItemWidgetBase.hxx"

#include "IndexCell.h"

class IndexCellEditor : public ItemWidgetBase
{
	Q_OBJECT
public:
	IndexCellEditor(QWidget* parent);
	~IndexCellEditor(void);

	virtual void setData(const boost::any& adata);

	virtual boost::any getData();

	virtual int getParentId();
	virtual void setParentId(int val);

	virtual int getId();
	virtual void setId(int val);

protected:
	virtual void onHoverEnter(QGraphicsSceneHoverEvent* event);
	virtual void onHoverLeave(QGraphicsSceneHoverEvent* event);

private:
	IndexCell ic_;

private:
	QGridLayout* mainLy_;
	QLabel* nameLab_;
};
