/*
* Copyright (c) 2009, Sysu
* 保留所有权利。( All rights reserved. )
* 
* 文件名称：diagramitem.hxx
* 文件标识：
* 摘    要：
* 
* 当前版本：1.0
* 作    者：Coolboy
* 完成日期：2009年11月11日
*
* 取代版本：
* 原作者  ：
* 完成日期：
*/
#pragma once

class Arrow;
class CardEditor;

#include "ICCard.h"

//! [0]
class DiagramItem : public QGraphicsProxyWidget 
{
	Q_OBJECT
public:
	enum { Type = UserType + 15 };
	enum DiagramType { Step, Conditional, StartEnd, Io };

	DiagramItem(DiagramType diagramType, QMenu *contextMenu,
		QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~DiagramItem();

	void removeArrow(Arrow *arrow);
	void removeArrows();
	DiagramType diagramType() const
	{ return myDiagramType; }
	QPolygonF polygon() const
	{ return shape().toFillPolygon(); }
	void addArrow(Arrow *arrow);
	int type() const
	{ return Type;}

	//ic
	void setIC(const ICCard& icc);
	ICCard getIC();
	int getParentId();
	int getId();

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
	DiagramType myDiagramType;
	QMenu *myContextMenu;
	QList<Arrow *> arrows;

private://icinfo
	CardEditor* ce_;
};