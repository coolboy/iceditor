/*
 * Copyright (c) 2009, Sysu
 * ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�diagramtextitem.hxx
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