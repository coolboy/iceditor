/*
* Copyright (c) 2009, Sysu
* ��������Ȩ����( All rights reserved. )
* 
* �ļ����ƣ�diagramscene.hxx
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

#include <QGraphicsScene>
#include "diagramitem.hxx"
#include "diagramtextitem.hxx"

#include "ICCard.h"
#include "IndexCell.h"

#include <boost/serialization/nvp.hpp>

class ICCardex{
public:
	ICCard ic_;
	QPointF pos_;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar  & BOOST_SERIALIZATION_NVP(ic_);
		ar  & BOOST_SERIALIZATION_NVP(pos_);
	}
};

typedef QVector<ICCardex> ICCardExs;

class TextEx{
public:
	QString content_;
	QPointF pos_;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(content_);
		ar & BOOST_SERIALIZATION_NVP(pos_);
	}
};

typedef QVector<TextEx> TextExs;

class IndexCellex{
public:
	IndexCell ic_;
	QPointF pos_;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(ic_);
		ar & BOOST_SERIALIZATION_NVP(pos_);
	}
};

typedef QVector<IndexCellex> IndexCellExs;

//! [0]
class DiagramScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

	DiagramScene(QMenu *itemMenu, QObject *parent = 0);
	QFont font() const
	{ return myFont; }
	QColor textColor() const
	{ return myTextColor; }
	QColor itemColor() const
	{ return myItemColor; }
	QColor lineColor() const
	{ return myLineColor; }
	void setLineColor(const QColor &color);
	void setTextColor(const QColor &color);
	void setItemColor(const QColor &color);
	void setFont(const QFont &font);

public:
	//ic
	void AddCards(const ICCard::ICCards& cards);
	void AddCards(const ICCardExs& cards);
	bool Verify(const ICCard::ICCards& cards);
	//text
	void AddTexts(const TextExs& texts);
	//icindex
	void AddIndexCells(const IndexCell::IndexCells& cells);
	void AddIndexCells(const IndexCellExs& cells);

protected:
	DiagramItem* ID2Item(int id);

public slots:
	void setMode(Mode mode);
	void setItemType(DiagramItem::DiagramType type);
	void editorLostFocus(DiagramTextItem *item);

signals:
	void itemInserted(DiagramItem *item);
	void textInserted(QGraphicsTextItem *item);
	void itemSelected(QGraphicsItem *item);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
	bool isItemChange(int type);

	DiagramItem::DiagramType myItemType;
	QMenu *myItemMenu;
	Mode myMode;
	bool leftButtonDown;
	QPointF startPoint;
	QGraphicsLineItem *line;
	QFont myFont;
	DiagramTextItem *textItem;
	QColor myTextColor;
	QColor myItemColor;
	QColor myLineColor;
};
//! [0]