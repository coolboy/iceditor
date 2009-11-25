/*
 * Copyright (c) 2009, Sysu
 * 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：diagramtextitem.hxx
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

#include <QGraphicsTextItem>
#include <QPen>

class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    DiagramTextItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }

signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};