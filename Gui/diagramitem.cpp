#include "StdAfx.h"
#include <QtGui>

#include "diagramitem.hxx"
#include "moc/moc_diagramitem.cpp"

#include "arrow.h"

#include "CardEditor.hxx"

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
												 QGraphicsItem *parent, QGraphicsScene *scene)
												 : QGraphicsProxyWidget(parent, Qt::Window), ce_(0)
{
	myDiagramType = diagramType;
	myContextMenu = contextMenu;
	ce_ = new CardEditor(0);
	setWidget(ce_);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}
//! [0]

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
	int index = arrows.indexOf(arrow);

	if (index != -1)
	{
		arrows.removeAt(index);
		if (arrow->startItem() == this)
			ce_->getIC().parentId = -1;
	}
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
	foreach (Arrow *arrow, arrows) {
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		scene()->removeItem(arrow);
		delete arrow;
	}
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
	if (arrow->startItem() == this)
		ce_->getIC().parentId = arrow->endItem()->getId();
	arrows.append(arrow);
}
//! [3]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);
	myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change,
																 const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) {
		foreach (Arrow *arrow, arrows) {
			arrow->updatePosition();
		}
	}

	return value;
}

DiagramItem::~DiagramItem()
{ }

void DiagramItem::setIC( const ICCard& icc )
{
	ce_->setIC(icc);
}

int DiagramItem::getParentId()
{
	return ce_->getIC().parentId;
}

int DiagramItem::getId()
{
	return ce_->getIC().Id;
}

void DiagramItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	QGraphicsProxyWidget::hoverEnterEvent(event);
	scene()->setActiveWindow(this);
	QPointF cPos = pos();
	ce_->showAll(true);
	ce_->setGeometry(cPos.x(), cPos.y(), ce_->width(), ce_->height());
}

void DiagramItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	QGraphicsProxyWidget::hoverLeaveEvent(event);
	QPointF cPos = pos();
	ce_->hideDetail();
	ce_->setGeometry(cPos.x(), cPos.y(), ce_->width(), ce_->height());
}

ICCard DiagramItem::getIC()
{
	return ce_->getIC();
}
//! [6]
