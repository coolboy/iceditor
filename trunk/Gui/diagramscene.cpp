#include "StdAfx.h"

#include "diagramscene.hxx"

#include "arrow.h"
#include "TreeLayout.h"

#include "moc/moc_diagramscene.cpp"

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
: QGraphicsScene(parent)
{
	myItemMenu = itemMenu;
	myMode = MoveItem;
	myItemType = DiagramItem::D_ICCard;
	line = 0;
	textItem = 0;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
	myLineColor = color;
	if (isItemChange(Arrow::Type)) {
		Arrow *item =
			qgraphicsitem_cast<Arrow *>(selectedItems().first());
		item->setColor(myLineColor);
		update();
	}
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
	myTextColor = color;
	if (isItemChange(DiagramTextItem::Type)) {
		DiagramTextItem *item =
			qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
		item->setDefaultTextColor(myTextColor);
	}
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
	myItemColor = color;
	if (isItemChange(DiagramItem::Type)) {
		DiagramItem *item =
			qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
		//item->setBrush(myItemColor);
	}
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
	myFont = font;

	if (isItemChange(DiagramTextItem::Type)) {
		QGraphicsTextItem *item =
			qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
		//At this point the selection can change so the first selected item might not be a DiagramTextItem
		if (item)
			item->setFont(myFont);
	}
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
	myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
	myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
	QTextCursor cursor = item->textCursor();
	cursor.clearSelection();
	item->setTextCursor(cursor);

	if (item->toPlainText().isEmpty()) {
		removeItem(item);
		item->deleteLater();
	}
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	DiagramItem *item;
	switch (myMode) {
				case InsertItem:
					item = new DiagramItem(myItemType, myItemMenu);
					//item->setBrush(myItemColor);
					addItem(item);
					item->setPos(mouseEvent->scenePos());
					emit itemInserted(item);
					break;
					//! [6] //! [7]
				case InsertLine:
					line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
						mouseEvent->scenePos()));
					line->setPen(QPen(myLineColor, 2));
					addItem(line);
					break;
					//! [7] //! [8]
				case InsertText:
					textItem = new DiagramTextItem();
					textItem->setFont(myFont);
					textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
					textItem->setZValue(1000.0);
					connect(textItem, SIGNAL(lostFocus(DiagramTextItem *)),
						this, SLOT(editorLostFocus(DiagramTextItem *)));
					connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
						this, SIGNAL(itemSelected(QGraphicsItem *)));
					addItem(textItem);
					textItem->setDefaultTextColor(myTextColor);
					textItem->setPos(mouseEvent->scenePos());
					emit textInserted(textItem);
					//! [8] //! [9]
				default:
					;
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (myMode == InsertLine && line != 0) {
		QLineF newLine(line->line().p1(), mouseEvent->scenePos());
		line->setLine(newLine);
	} else if (myMode == MoveItem) {
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (line != 0 && myMode == InsertLine) {
		QList<QGraphicsItem *> startItems = items(line->line().p1());
		if (startItems.count() && startItems.first() == line)
			startItems.removeFirst();
		QList<QGraphicsItem *> endItems = items(line->line().p2());
		if (endItems.count() && endItems.first() == line)
			endItems.removeFirst();

		removeItem(line);
		delete line;
		//! [11] //! [12]

		if (startItems.count() > 0 && endItems.count() > 0 &&
			startItems.first()->type() == DiagramItem::Type &&
			endItems.first()->type() == DiagramItem::Type &&
			startItems.first() != endItems.first()) {
				DiagramItem *startItem =
					qgraphicsitem_cast<DiagramItem *>(startItems.first());
				DiagramItem *endItem =
					qgraphicsitem_cast<DiagramItem *>(endItems.first());
				Arrow *arrow = new Arrow(startItem, endItem);
				arrow->setColor(myLineColor);
				startItem->addArrow(arrow);
				endItem->addArrow(arrow);
				arrow->setZValue(-1000.0);
				addItem(arrow);
				arrow->updatePosition();
		}
	}
	//! [12] //! [13]
	line = 0;
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
	foreach (QGraphicsItem *item, selectedItems()) {
		if (item->type() == type)
			return true;
	}
	return false;
}

void DiagramScene::AddCards( const ICCard::ICCards& cards )
{
	if (Verify(cards) == false)
		return;

	TreeLayout tl(sceneRect());

	QList<DiagramItem*> ls;
	Relations rels;

	foreach(const ICCard& ic, cards) {
		DiagramItem* item = new DiagramItem(DiagramItem::D_ICCard, myItemMenu);
		item->setData(ic);
		addItem(item);

		ls << item;
		rels[item->getId()] = item->getParentId();
	}

	tl.setTree(rels);

	foreach(DiagramItem* item, ls) {
		QPointF pos = tl.getPos(item->getId());
		item->setPos(pos.x(), pos.y());
	}

	foreach (QGraphicsItem *item, items()) {
		if (item->type() != DiagramItem::Type)
			continue;

		DiagramItem* currItm = qgraphicsitem_cast<DiagramItem *>(item);
		if (currItm == 0)
			continue;

		int parId = currItm->getParentId();
		if (parId == -1)
			continue;

		DiagramItem* parItm = ID2Item(parId);
		if (parItm == 0)
			continue;

		Arrow *arrow = new Arrow(currItm, parItm);
		arrow->setColor(myLineColor);
		currItm->addArrow(arrow);
		parItm->addArrow(arrow);
		arrow->setZValue(-1000.0);
		addItem(arrow);
		arrow->updatePosition();
	}
}

void DiagramScene::AddCards( const ICCardexs& cards )
{
	foreach(const ICCardex& ice, cards)
	{
		DiagramItem* item = new DiagramItem(DiagramItem::D_ICCard, myItemMenu);
		item->setData(ice.ic_);
		addItem(item);
		item->setPos(ice.pos_);
	}

	foreach (QGraphicsItem *item, items()) {
		if (item->type() != DiagramItem::Type)
			continue;

		DiagramItem* currItm = qgraphicsitem_cast<DiagramItem *>(item);
		if (currItm == 0)
			continue;

		int parId = currItm->getParentId();
		if (parId == -1)
			continue;

		DiagramItem* parItm = ID2Item(parId);
		if (parItm == 0)
			continue;

		Arrow *arrow = new Arrow(currItm, parItm);
		arrow->setColor(myLineColor);
		currItm->addArrow(arrow);
		parItm->addArrow(arrow);
		arrow->setZValue(-1000.0);
		addItem(arrow);
		arrow->updatePosition();
	}
}

DiagramItem* DiagramScene::ID2Item( int id )
{
	foreach (QGraphicsItem *item, items()) {
		if (item->type() == DiagramItem::Type) {
			DiagramItem* currItm = qgraphicsitem_cast<DiagramItem *>(item);
			int Id = currItm->getId();
			if (Id == id)
				return currItm;
		}
	}

	return 0;
}

void DiagramScene::AddTexts( const TextExs& texts )
{
	foreach(const TextEx& te, texts)
	{
		textItem = new DiagramTextItem();
		textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
		textItem->setZValue(1000.0);
		connect(textItem, SIGNAL(lostFocus(DiagramTextItem *)),
			this, SLOT(editorLostFocus(DiagramTextItem *)));
		connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
			this, SIGNAL(itemSelected(QGraphicsItem *)));
		addItem(textItem);
		textItem->setPos(te.pos_);
		textItem->setHtml(te.content_);
		//emit textInserted(textItem);
	}
}

bool DiagramScene::Verify( const ICCard::ICCards& cards )
{
	//check if there is no groupname of icindexname
	foreach(const ICCard& ic, cards) {
		if (ic.indexCellName.isEmpty() ||
			ic.groupName.isEmpty())
		{
			QString errMsg = ic.getSimpleStr() + " can't find groupname and icindexname!";
			QMessageBox::critical(0, "Xml format error!", errMsg, QMessageBox::Ok);
			return false;
		}
	}

	return true;
}

void DiagramScene::AddIndexCells( const IndexCell::IndexCells& cells )
{
	TreeLayout tl(sceneRect());

	QList<DiagramItem*> ls;
	Relations rels;

	foreach(const IndexCell& ic, cells) {
		DiagramItem* item = new DiagramItem(DiagramItem::D_IndexCell, myItemMenu);
		//IndexCell icc = ic;
		//IndexCell icc;
		//icc.Id = ic.Id;
		//icc.parentId = ic.parentId;
		//icc.Name = ic.Name;
		item->setData(ic);
		addItem(item);

		ls << item;
		rels[item->getId()] = item->getParentId();
	}

	tl.setTree(rels);

	foreach(DiagramItem* item, ls) {
		QPointF pos = tl.getPos(item->getId());
		item->setPos(pos.x(), pos.y());
	}

	foreach (QGraphicsItem *item, items()) {
		if (item->type() != DiagramItem::Type)
			continue;

		DiagramItem* currItm = qgraphicsitem_cast<DiagramItem *>(item);
		if (currItm == 0)
			continue;

		int parId = currItm->getParentId();
		if (parId == -1)
			continue;

		DiagramItem* parItm = ID2Item(parId);
		if (parItm == 0)
			continue;

		Arrow *arrow = new Arrow(currItm, parItm);
		arrow->setColor(myLineColor);
		currItm->addArrow(arrow);
		parItm->addArrow(arrow);
		arrow->setZValue(-1000.0);
		addItem(arrow);
		arrow->updatePosition();
	}
}
//! [14]