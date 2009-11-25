/*
* Copyright (c) 2009, Sysu
* ��������Ȩ����( All rights reserved. )
* 
* �ļ����ƣ�diagramitem.hxx
* �ļ���ʶ��
* ժ    Ҫ��
* 
* ��ǰ�汾��1.0
* ��    �ߣ�Coolboy
* ������ڣ�2009��11��11��
*
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
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