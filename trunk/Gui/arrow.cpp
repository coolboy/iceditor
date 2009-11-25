#include "StdAfx.h"
#include <QtGui>

#include "arrow.h"
#include <math.h>

const qreal Pi = 3.14;

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem,
						 QGraphicsItem *parent, QGraphicsScene *scene)
						 : QGraphicsLineItem(parent, scene)
{
	myStartItem = startItem;
	myEndItem = endItem;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor = Qt::black;
	setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
	qreal extra = (pen().width() + 100) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
	QPainterPath path = QGraphicsLineItem::shape();
	path.addPolygon(arrowHead);
	return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
	QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
	setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
									QWidget *)
{
	if (myStartItem->collidesWithItem(myEndItem))
		return;

	QPen myPen = pen();
	myPen.setColor(myColor);
	qreal arrowSize = 20;
	painter->setPen(myPen);
	painter->setBrush(myColor);
	//! [4] //! [5]

	QPointF startPos = myStartItem->pos();//起始位置
	QPointF offset(myStartItem->rect().width()/2, myStartItem->rect().height()/2);
	startPos += offset;

	QPointF endPos = myEndItem->pos();//arrow 位置
	offset = QPointF(myEndItem->rect().width()/2, myEndItem->rect().height()/2);
	endPos += offset;

	QLineF centerLine(startPos, endPos);
	QPolygonF endPolygon = myEndItem->polygon();
	QPointF p1 = endPolygon.first() + myEndItem->pos();
	QPointF p2;
	QPointF endIntersectPoint;
	QLineF polyLine;
	for (int i = 1; i < endPolygon.count(); ++i) {
		p2 = endPolygon.at(i) + myEndItem->pos();
		polyLine = QLineF(p1, p2);
		QLineF::IntersectType intersectType =
			polyLine.intersect(centerLine, &endIntersectPoint);
		if (intersectType == QLineF::BoundedIntersection)
			break;
		p1 = p2;
	}

	QPolygonF startPolygon = myStartItem->polygon();
	p1 = startPolygon.first() + myStartItem->pos();
	QPointF startIntersectPoint;
	for (int i = 1; i < startPolygon.count(); ++i) {
		p2 = startPolygon.at(i) + myStartItem->pos();
		polyLine = QLineF(p1, p2);
		QLineF::IntersectType intersectType =
			polyLine.intersect(centerLine, &startIntersectPoint);
		if (intersectType == QLineF::BoundedIntersection)
			break;
		p1 = p2;
	}

	if (line().p1() != endIntersectPoint || line().p2() != startIntersectPoint)
	{
		QGraphicsItem::prepareGeometryChange();
		setLine(QLineF(endIntersectPoint, startIntersectPoint));
	}
	//! [5] //! [6]

	double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (Pi * 2) - angle;

	QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
		cos(angle + Pi / 3) * arrowSize);
	QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
		cos(angle + Pi - Pi / 3) * arrowSize);

	arrowHead.clear();
	arrowHead << line().p1() << arrowP1 << arrowP2;
	//! [6] //! [7]
	painter->drawLine(line());
	painter->drawPolygon(arrowHead);
	//QPainterPath path;
	//path.addRect(boundingRect());
	//painter->drawPath(path);
	//painter->drawRect(boundingRect());
	//painter->draw
	if (isSelected()) {
		painter->setPen(QPen(myColor, 1, Qt::DashLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0,-8.0);
		painter->drawLine(myLine);
	}
}
//! [7]
