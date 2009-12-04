#include "StdAfx.h"
#include <QtGui>

#include "diagramitem.hxx"
#include "moc/moc_diagramitem.cpp"

#include "arrow.h"

#include "CardEditor.hxx"

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
												 QGraphicsItem *parent, QGraphicsScene *scene)
												 : QGraphicsProxyWidget(parent, Qt::Window), wig_(0)
{
	myDiagramType = diagramType;
	myContextMenu = contextMenu;

	switch (myDiagramType)
	{
	case D_ICCard:
		wig_ = new CardEditor(0);
		setWidget(wig_);
		break;

	case D_IndexCell:
		break;

	default:
		assert(0);
		break;
	}

	QConnect(this, sigHoverEnter(), wig_, onHoverEnter());
	QConnect(this, sigHoverLeave(), wig_, onHoverLeave());

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
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
			wig_->setParentId(-1);
			//wig_->getIC().parentId = -1;
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
		wig_->setParentId(arrow->endItem()->getId());
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

void DiagramItem::setData( const boost::any& adata )
{
	wig_->setData(adata);
}

int DiagramItem::getParentId()
{
	return wig_->getParentId();
}

int DiagramItem::getId()
{
	return wig_->getId();
}

void DiagramItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	QGraphicsProxyWidget::hoverEnterEvent(event);
	scene()->setActiveWindow(this);
	QPointF cPos = pos();
	//wig_->showAll(true);
	sigHoverEnter();
	wig_->setGeometry(cPos.x(), cPos.y(), wig_->width(), wig_->height());
}

void DiagramItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	QGraphicsProxyWidget::hoverLeaveEvent(event);
	QPointF cPos = pos();
	//wig_->hideDetail();
	sigHoverLeave();
	wig_->setGeometry(cPos.x(), cPos.y(), wig_->width(), wig_->height());
}

boost::any DiagramItem::getData()
{
	return wig_->getData();
}
//! [6]
