#pragma once

#include <boost/any.hpp>

class ItemWidgetBase : public QDialog
{
	Q_OBJECT
public:
	ItemWidgetBase(QWidget* parent):QDialog(parent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint){}
	virtual ~ItemWidgetBase(){}

	virtual void setData(const boost::any& adata) = 0;
	virtual boost::any getData() = 0;

	virtual int getParentId() = 0;
	virtual void setParentId(int val) = 0;

	virtual int getId() = 0;
	virtual void setId(int val) = 0;

protected slots:
	virtual void onHoverEnter() = 0;
	virtual void onHoverLeave() = 0;
};
