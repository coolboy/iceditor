#pragma once

class Ui_CardEditor;

#include "ICCard.h"
#include "ItemWidgetBase.hxx"

class CardEditor : public ItemWidgetBase
{
	Q_OBJECT
public:
	CardEditor(QWidget* parent);
	~CardEditor(void);

	virtual void setData(const boost::any& adata);
	virtual boost::any getData();

	virtual int getParentId();
	virtual void setParentId(int val);

	virtual int getId();
	virtual void setId(int val);

signals:
	void accepted();
	void rejected();

private slots:
	void accept();
	void reject();

protected:
	virtual void onHoverEnter(QGraphicsSceneHoverEvent* event);
	virtual void onHoverLeave(QGraphicsSceneHoverEvent* event);

private:
	void showAll(bool bShow);
	void showAll();
	void hideDetail();

private:
	void setIC(const ICCard& icc);

private:
	Ui_CardEditor* ui;
	ICCard icc_;
};
