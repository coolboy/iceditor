#include "StdAfx.h"
#include "CardEditor.hxx"

#include "moc/moc_CardEditor.cpp"
#include "uic/ui_CardEditor.h"

#include "ICCard.h"

CardEditor::CardEditor( QWidget* parent ) 
	:QDialog(parent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
	ui = new Ui_CardEditor;
	ui->setupUi(this);
	hideDetail();
}

CardEditor::~CardEditor( void )
{
	delete ui;
}

void CardEditor::setIC( const ICCard& icc )
{
	hideDetail();

	ui->name->setText(icc.Name);
	ui->description->setText(icc.Description);

	switch (icc.IntPattern)
	{
	case ICCard::QuietState:
		ui->quiet->setChecked(true);
		break;

	case ICCard::MyselfNone:
		ui->myselfnone->setChecked(true);
		break;

	case ICCard::MyselfWith:
		ui->myselfwith->setChecked(true);
		break;

	case ICCard::OthersNone:
		ui->othersnone->setChecked(true);
		break;

	case ICCard::OthersWith:
		ui->otherswith->setChecked(true);
		break;

	case ICCard::MixedState:
		ui->mixed->setChecked(true);
		break;

	case ICCard::Undef:
		break;

	default:
		assert(0);
	}

	ui->myTask->setText(icc.Task);
	ui->TimeCritical->setText(icc.TimeCriticalCondition);
	ui->otherName->setText(icc.OtherName);
	ui->otherMsg->setText(icc.OtherMessage);
	ui->otherTask->setText(icc.OtherTask);
	ui->num->setValue(icc.NumberCurrent);
	ui->totalNum->setValue(icc.NumberTotal);

	icc_ = icc;

	assert(icc.NumberCurrent <= icc.NumberTotal);
}

void CardEditor::accept()
{
	icc_.Name = ui->name->text();
	icc_.Description = ui->description->text();

	if (ui->quiet->isChecked())
		icc_.IntPattern = ICCard::QuietState;
	else if (ui->myselfnone->isChecked())
		icc_.IntPattern = ICCard::MyselfNone;
	else if (ui->myselfwith->isChecked())
		icc_.IntPattern = ICCard::MyselfWith;
	else if (ui->othersnone->isChecked())
		icc_.IntPattern = ICCard::OthersNone;
	else if (ui->otherswith->isChecked())
		icc_.IntPattern = ICCard::OthersWith;
	else if (ui->mixed->isChecked())
		icc_.IntPattern = ICCard::MixedState;
	else
		icc_.IntPattern = ICCard::Undef;

	icc_.Task = ui->myTask->text();
	icc_.TimeCriticalCondition = ui->TimeCritical->text();
	icc_.OtherName = ui->otherName->text();
	icc_.OtherMessage = ui->otherMsg->text();
	icc_.OtherTask = ui->otherTask->text();
	icc_.NumberCurrent = ui->num->value();
	icc_.NumberTotal = ui->totalNum->value();

	accepted();
}

void CardEditor::reject()
{
	rejected();
}

ICCard &CardEditor::getIC()
{
	return icc_;
}

void CardEditor::showAll( bool bShow )
{
	ui->label->setVisible(bShow);
	ui->label_2->setVisible(bShow);
	ui->name->setVisible(bShow);
	ui->label_3->setVisible(bShow);
	ui->description->setVisible(bShow);
	ui->label_4->setVisible(bShow);
	ui->quiet->setVisible(bShow);
	ui->myselfnone->setVisible(bShow);
	ui->myselfwith->setVisible(bShow);
	ui->othersnone->setVisible(bShow);
	ui->otherswith->setVisible(bShow);
	ui->mixed->setVisible(bShow);
	ui->label_5->setVisible(bShow);
	ui->myTask->setVisible(bShow);
	ui->label_8->setVisible(bShow);
	ui->TimeCritical->setVisible(bShow);
	ui->label_7->setVisible(bShow);
	ui->otherName->setVisible(bShow);
	ui->label_6->setVisible(bShow);
	ui->otherMsg->setVisible(bShow);
	ui->label_10->setVisible(bShow);
	ui->otherTask->setVisible(bShow);
	ui->label_9->setVisible(bShow);
	ui->num->setVisible(bShow);
	ui->label_11->setVisible(bShow);
	ui->totalNum->setVisible(bShow);
	ui->label_12->setVisible(bShow);
	adjustSize();
}

void CardEditor::hideDetail()
{
	showAll(false);
	ui->name->setVisible(true);
	ui->description->setVisible(true);
	ui->label_2->setVisible(true);
	ui->label_3->setVisible(true);
	adjustSize();
}