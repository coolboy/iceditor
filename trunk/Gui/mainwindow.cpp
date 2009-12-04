#include "StdAfx.h"

#include "diagramitem.hxx"
#include "mainwindow.hxx"
#include "diagramscene.hxx"
#include "diagramtextitem.hxx"

#include <boost/bind.hpp>

#include "XmlHelper.h"
#include "ICCard.h"

#include "version.h"

#include "moc/moc_mainwindow.cpp"

const int InsertTextButton = 10;

MainWindow::MainWindow():itemMenu(0)
{
	createActions();
	createToolBox();
	createMenus();
	createStatusbar();

	scene = new DiagramScene(itemMenu);
	scene->setSceneRect(QRectF(0, 0, 5000, 5000));
	connect(scene, SIGNAL(itemInserted(DiagramItem *)),
		this, SLOT(itemInserted(DiagramItem *)));
	connect(scene, SIGNAL(textInserted(QGraphicsTextItem *)),
		this, SLOT(textInserted(QGraphicsTextItem *)));
	connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
		this, SLOT(itemSelected(QGraphicsItem *)));
	createToolbars();

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(toolBox);
	view = new QGraphicsView(scene);
	layout->addWidget(view);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	setCentralWidget(widget);
	setWindowTitle(qApp->applicationName());
}
//! [0]

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{
	QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
	foreach (QAbstractButton *myButton, buttons) {
		if (myButton != button)
			button->setChecked(false);
	}
	QString text = button->text();
	if (text == tr("Blue Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
	else if (text == tr("White Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
	else if (text == tr("Gray Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
	else
		scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

	scene->update();
	view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
	QList<QAbstractButton *> buttons = buttonGroup->buttons();
	foreach (QAbstractButton *button, buttons) {
		if (buttonGroup->button(id) != button)
			button->setChecked(false);
	}
	if (id == InsertTextButton) {
		scene->setMode(DiagramScene::InsertText);
	} else {
		scene->setItemType(DiagramItem::DiagramType(id));
		scene->setMode(DiagramScene::InsertItem);
	}
}
//! [2]

//! [3]
void MainWindow::deleteItem()
{
	foreach (QGraphicsItem *item, scene->selectedItems()) {
		if (item->type() == DiagramItem::Type) {
			qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
		}
		scene->removeItem(item);
		delete item;
	}
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
	if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem *selectedItem = scene->selectedItems().first();
	QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	foreach (QGraphicsItem *item, overlapItems) {
		if (item->zValue() >= zValue &&
			item->type() == DiagramItem::Type)
			zValue = item->zValue() + 0.1;
	}
	selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
	if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem *selectedItem = scene->selectedItems().first();
	QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	foreach (QGraphicsItem *item, overlapItems) {
		if (item->zValue() <= zValue &&
			item->type() == DiagramItem::Type)
			zValue = item->zValue() - 0.1;
	}
	selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem *item)
{
	pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
	buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
	buttonGroup->button(InsertTextButton)->setChecked(false);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
	handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
	handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged( int scale )
{
	double newScale = scale / 100.0;
	QMatrix oldMatrix = view->matrix();
	view->resetMatrix();
	view->translate(oldMatrix.dx(), oldMatrix.dy());
	view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
	textAction = qobject_cast<QAction *>(sender());
	fontColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/textpointer.png",
		qVariantValue<QColor>(textAction->data())));
	textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
	fillAction = qobject_cast<QAction *>(sender());
	fillColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/floodfill.png",
		qVariantValue<QColor>(fillAction->data())));
	fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
	lineAction = qobject_cast<QAction *>(sender());
	lineColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/linecolor.png",
		qVariantValue<QColor>(lineAction->data())));
	lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
	scene->setTextColor(qVariantValue<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
	scene->setItemColor(qVariantValue<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
	scene->setLineColor(qVariantValue<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
	QFont font = fontCombo->currentFont();
	font.setPointSize(fontSizeCombo->currentText().toInt());
	font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
	font.setItalic(italicAction->isChecked());
	font.setUnderline(underlineAction->isChecked());

	scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
	DiagramTextItem *textItem =
		qgraphicsitem_cast<DiagramTextItem *>(item);

	QFont font = textItem->font();
	QColor color = textItem->defaultTextColor();
	fontCombo->setCurrentFont(font);
	fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
	boldAction->setChecked(font.weight() == QFont::Bold);
	italicAction->setChecked(font.italic());
	underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
	QMessageBox mb(this);
	mb.setWindowTitle("About " + QApplication::applicationName() );
	mb.setText(tr("<p><a href=\"mailto:CoolCute@gmail.com\">EMail</a></p>"
		"<p><font color=black>Build Number: " VERSION_ANSI "</font></p>"
		"<p><a href=\"http://www.cs.pitt.edu/~huyang\"><font color=\"#00FF00\">Webpage</font></a></p>"
		"<font color=\"#FF0000\">Build Time: " __DATE__ " " __TIME__"</font>"));
	mb.exec ();
}
//! [20]

//! [21]
void MainWindow::createToolBox()
{
	buttonGroup = new QButtonGroup;
	buttonGroup->setExclusive(false);
	connect(buttonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(buttonGroupClicked(int)));
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(createCellWidget(tr("ICCard"),
		DiagramItem::Conditional), 0, 0);

	QToolButton *textButton = new QToolButton;
	textButton->setCheckable(true);
	buttonGroup->addButton(textButton, InsertTextButton);
	textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
		.scaled(30, 30)));
	textButton->setIconSize(QSize(50, 50));
	QGridLayout *textLayout = new QGridLayout;
	textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
	textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
	QWidget *textWidget = new QWidget;
	textWidget->setLayout(textLayout);
	layout->addWidget(textWidget, 0, 1);

	layout->setRowStretch(3, 10);
	layout->setColumnStretch(2, 10);

	QWidget *itemWidget = new QWidget;
	itemWidget->setLayout(layout);

	backgroundButtonGroup = new QButtonGroup;
	connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton *)),
		this, SLOT(backgroundButtonGroupClicked(QAbstractButton *)));

	QGridLayout *backgroundLayout = new QGridLayout;
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
		":/images/background1.png"), 0, 0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
		":/images/background2.png"), 0, 1);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
		":/images/background3.png"), 1, 0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
		":/images/background4.png"), 1, 1);

	backgroundLayout->setRowStretch(2, 10);
	backgroundLayout->setColumnStretch(2, 10);

	QWidget *backgroundWidget = new QWidget;
	backgroundWidget->setLayout(backgroundLayout);


	//! [22]
	toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	toolBox->setMinimumWidth(itemWidget->sizeHint().width());
	toolBox->addItem(itemWidget, tr("Basic Shapes"));
	toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}
//! [22]

//! [23]
void MainWindow::createActions()
{
	toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
		tr("Bring to &Front"), this);
	toFrontAction->setShortcut(tr("Ctrl+F"));
	toFrontAction->setStatusTip(tr("Bring item to front"));
	connect(toFrontAction, SIGNAL(triggered()),
		this, SLOT(bringToFront()));
	//! [23]

	sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
		tr("Send to &Back"), this);
	sendBackAction->setShortcut(tr("Ctrl+B"));
	sendBackAction->setStatusTip(tr("Send item to back"));
	connect(sendBackAction, SIGNAL(triggered()),
		this, SLOT(sendToBack()));

	deleteAction = new QAction(QIcon(":/images/delete.png"),
		tr("&Delete"), this);
	deleteAction->setShortcut(tr("Delete"));
	deleteAction->setStatusTip(tr("Delete item from diagram"));
	connect(deleteAction, SIGNAL(triggered()),
		this, SLOT(deleteItem()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+X"));
	exitAction->setStatusTip("Quit " + qApp->applicationName());
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	loadAction = new QAction(tr("L&oad"), this);
	loadAction->setShortcut(tr("Ctrl+L"));
	loadAction->setStatusTip(tr("Load icdb and icci xml file"));
	connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

	grossAction = new QAction(tr("Gross"), this);
	connect(grossAction, SIGNAL(triggered()), this, SLOT(loadGross()));

	fineAction = new QAction(tr("Fine"), this);
	connect(fineAction, SIGNAL(triggered()), this, SLOT(loadFine()));

	saveAction = new QAction(tr("S&ave"), this);
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("Save the project"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	openAction = new QAction(tr("O&pen"), this);
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("Open project file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	boldAction = new QAction(tr("Bold"), this);
	boldAction->setCheckable(true);
	QPixmap pixmap(":/images/bold.png");
	boldAction->setIcon(QIcon(pixmap));
	boldAction->setShortcut(tr("Ctrl+B"));
	connect(boldAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	italicAction = new QAction(QIcon(":/images/italic.png"),
		tr("Italic"), this);
	italicAction->setCheckable(true);
	italicAction->setShortcut(tr("Ctrl+I"));
	connect(italicAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	underlineAction = new QAction(QIcon(":/images/underline.png"),
		tr("Underline"), this);
	underlineAction->setCheckable(true);
	underlineAction->setShortcut(tr("Ctrl+U"));
	connect(underlineAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	aboutAction = new QAction(tr("A&bout"), this);
	aboutAction->setShortcut(tr("Ctrl+B"));
	connect(aboutAction, SIGNAL(triggered()),
		this, SLOT(about()));
}

//! [24]
void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(loadAction);
	fileMenu->addAction(fineAction);
	fileMenu->addAction(grossAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);

	itemMenu = menuBar()->addMenu(tr("&Item"));
	itemMenu->addAction(deleteAction);
	itemMenu->addSeparator();
	itemMenu->addAction(toFrontAction);
	itemMenu->addAction(sendBackAction);

	aboutMenu = menuBar()->addMenu(tr("&Help"));
	aboutMenu->addAction(aboutAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
	//! [25]
	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(deleteAction);
	editToolBar->addAction(toFrontAction);
	editToolBar->addAction(sendBackAction);

	fontCombo = new QFontComboBox();
	fontSizeCombo = new QComboBox();
	connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
		this, SLOT(currentFontChanged(const QFont &)));

	fontSizeCombo = new QComboBox;
	fontSizeCombo->setEditable(true);
	for (int i = 8; i < 30; i = i + 2)
		fontSizeCombo->addItem(QString().setNum(i));
	QIntValidator *validator = new QIntValidator(2, 64, this);
	fontSizeCombo->setValidator(validator);
	connect(fontSizeCombo, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(fontSizeChanged(const QString &)));

	fontColorToolButton = new QToolButton;
	fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
		Qt::black));
	textAction = fontColorToolButton->menu()->defaultAction();
	fontColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/textpointer.png", Qt::black));
	fontColorToolButton->setAutoFillBackground(true);
	connect(fontColorToolButton, SIGNAL(clicked()),
		this, SLOT(textButtonTriggered()));

	//! [26]
	fillColorToolButton = new QToolButton;
	fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()),
		Qt::white));
	fillAction = fillColorToolButton->menu()->defaultAction();
	fillColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/floodfill.png", Qt::white));
	connect(fillColorToolButton, SIGNAL(clicked()),
		this, SLOT(fillButtonTriggered()));
	//! [26]

	lineColorToolButton = new QToolButton;
	lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()),
		Qt::black));
	lineAction = lineColorToolButton->menu()->defaultAction();
	lineColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/linecolor.png", Qt::black));
	connect(lineColorToolButton, SIGNAL(clicked()),
		this, SLOT(lineButtonTriggered()));

	textToolBar = addToolBar(tr("Font"));
	textToolBar->addWidget(fontCombo);
	textToolBar->addWidget(fontSizeCombo);
	textToolBar->addAction(boldAction);
	textToolBar->addAction(italicAction);
	textToolBar->addAction(underlineAction);

	colorToolBar = addToolBar(tr("Color"));
	colorToolBar->addWidget(fontColorToolButton);
	colorToolBar->addWidget(fillColorToolButton);
	colorToolBar->addWidget(lineColorToolButton);

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(QIcon(":/images/pointer.png"));
	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

	pointerTypeGroup = new QButtonGroup;
	pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
	pointerTypeGroup->addButton(linePointerButton,
		int(DiagramScene::InsertLine));
	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(pointerGroupClicked(int)));

	sceneScaleSP = new QSpinBox;
	sceneScaleSP->setMinimum(1);
	sceneScaleSP->setMaximum(500);
	sceneScaleSP->setValue(100);

	connect(sceneScaleSP, SIGNAL(valueChanged ( int )),
		this, SLOT(sceneScaleChanged(int)));

	pointerToolbar = addToolBar(tr("Pointer type"));
	pointerToolbar->addWidget(pointerButton);
	pointerToolbar->addWidget(linePointerButton);
	pointerToolbar->addWidget(sceneScaleSP);
	pointerToolbar->addWidget(new QLabel(" %"));
	//! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
																								const QString &image)
{
	QToolButton *button = new QToolButton;
	button->setText(text);
	button->setIcon(QIcon(image));
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	backgroundButtonGroup->addButton(button);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text,
																			DiagramItem::DiagramType type)
{

	QIcon icon(":/images/iccard.png");

	QToolButton *button = new QToolButton;
	button->setIcon(icon);
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	buttonGroup->addButton(button, int(type));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}
//! [29]

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
	QList<QColor> colors;
	colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
	QStringList names;
	names << tr("black") << tr("white") << tr("red") << tr("blue")
		<< tr("yellow");

	QMenu *colorMenu = new QMenu;
	for (int i = 0; i < colors.count(); ++i) {
		QAction *action = new QAction(names.at(i), this);
		action->setData(colors.at(i));
		action->setIcon(createColorIcon(colors.at(i)));
		connect(action, SIGNAL(triggered()),
			this, slot);
		colorMenu->addAction(action);
		if (colors.at(i) == defaultColor) {
			colorMenu->setDefaultAction(action);
		}
	}
	return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile,
																						QColor color)
{
	QPixmap pixmap(50, 80);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPixmap image(imageFile);
	QRect target(0, 0, 50, 60);
	QRect source(0, 0, 42, 42);
	painter.fillRect(QRect(0, 60, 50, 80), color);
	painter.drawPixmap(target, image, source);

	return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
	QPixmap pixmap(20, 20);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.fillRect(QRect(0, 0, 20, 20), color);

	return QIcon(pixmap);
}

void MainWindow::createStatusbar()
{
	statusBar();
}

#include <fstream>
#include "QPointF.hpp"
#include "QString.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>

void MainWindow::save()
{
	QString icePath;
	icePath = QFileDialog::getSaveFileName(this, "Save the project file to where?", 
		"",  tr("ICEditor Project File (*.ice)"));
	if (icePath.isEmpty())
		return;

	QFile file(icePath);
	if (file.open(QIODevice::WriteOnly) == false)
		return;

	ICCardexs cards;
	TextExs texts;

	foreach (QGraphicsItem *item, scene->items()) {
		if (item->type() == DiagramItem::Type)
		{
			DiagramItem* currItm = qgraphicsitem_cast<DiagramItem *>(item);
			if (currItm == 0)
				continue;

			ICCardex ice;
			ice.ic_ = currItm->getIC();
			ice.pos_ = currItm->scenePos();

			cards<<ice;
		}
		else if (item->type() == DiagramTextItem::Type)
		{
			DiagramTextItem* currItm = qgraphicsitem_cast<DiagramTextItem *>(item);
			TextEx te;
			te.content_ = currItm->toHtml();
			te.pos_ = currItm->scenePos();

			texts<<te;
		}
	}

	// make an archive
	std::wofstream ofs(icePath.utf16());
	assert(ofs.good());
	boost::archive::xml_woarchive oa(ofs);
	std::vector<ICCardex> scards = cards.toStdVector();
	std::vector<TextEx> stexts = texts.toStdVector();
	oa << BOOST_SERIALIZATION_NVP(scards);
	oa << BOOST_SERIALIZATION_NVP(stexts);
}

void MainWindow::load()
{
	QString icdbPath, icciPath;
	icdbPath = QFileDialog::getOpenFileName(this, "Where is the icdb.xml?", 
		"",  tr("Xml (*.xml)"));
	if (icdbPath.isEmpty())
		return;

	QFile srcIcdbXmlFile(icdbPath);
	QStringList icCards = XQuery2(srcIcdbXmlFile, QUrl::fromLocalFile(qApp->applicationDirPath() + "/icdb.xq"));
	qDebug()<< icCards;

	icciPath = QFileDialog::getOpenFileName(this, "Where is the icci.xml?", "",  tr("Xml (*.xml)"));
	if (icciPath.isEmpty())
		return;

	QFile srcIcciXmlFile(icciPath);
	QStringList iccis = XQuery2(srcIcciXmlFile, QUrl::fromLocalFile(qApp->applicationDirPath() + "/icci.xq"));
	qDebug()<< iccis;

	ICCard::ICCards cards = ICCard::Load(icCards, iccis);

	scene->clear();
	scene->AddCards(cards);
}

void MainWindow::open()
{
	QString icePath;
	icePath = QFileDialog::getOpenFileName(this, "Where is the project file?", 
		"",  tr("ICEditor Project File (*.ice)"));
	if (icePath.isEmpty())
		return;

	QFile file(icePath);
	if (file.open(QIODevice::ReadOnly) == false)
		return;

	// open the archive
	std::wifstream ifs(icePath.utf16());
	assert(ifs.good());
	boost::archive::xml_wiarchive ia(ifs);

	std::vector<ICCardex> scards;
	std::vector<TextEx> stexts;
	// restore the schedule from the archive
	ia >> BOOST_SERIALIZATION_NVP(scards);
	ia >> BOOST_SERIALIZATION_NVP(stexts);

	ICCardexs cards = ICCardexs::fromStdVector(scards);
	TextExs texts = TextExs::fromStdVector(stexts);
	
	scene->clear();
	scene->AddCards(cards);
	scene->AddTexts(texts);
}

#include "IndexSystem.h"
#include "IndexCell.h"
#include "Transition.h"

void MainWindow::loadGross()
{
	QString grossPath = QFileDialog::getOpenFileName(this, "Where is the gross.xml?", 
		"",  tr("Xml (*.xml)"));
	if (grossPath.isEmpty())
		return;

	QFile srcGrossFile(grossPath);
	QStringList icCards = XQuery2(srcGrossFile, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/gross.xq"));
	qDebug()<< icCards;
}

void MainWindow::loadFine()
{
	QString finePath = QFileDialog::getOpenFileName(this, "Where is the fine.xml?", 
		"",  tr("Xml (*.xml)"));
	if (finePath.isEmpty())
		return;

	QFile srcFineXmlFile(finePath);
	QStringList isLs = XQuery2(srcFineXmlFile, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/indexSystem.xq"));
	qDebug()<< isLs;

	IndexSystem::IndexSystems is = IndexSystem::Load(isLs);

	srcFineXmlFile.seek(0);
	QStringList tLs = XQuery2(srcFineXmlFile, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/fTransition.xq"));
	qDebug()<< tLs;

	Transition::Transitions trans = Transition::load(tLs);

	srcFineXmlFile.seek(0);
	QStringList icLs = XQuery2(srcFineXmlFile, QUrl::fromLocalFile(
		qApp->applicationDirPath() + "/fIndexCell.xq"));
	qDebug()<< icLs;

	IndexCell::IndexCells ics = IndexCell::Load(icLs, trans);

	scene->clear();
	scene->AddIndexCells(ics);
}
//! [32]
