#include "StdAfx.h"
#include "IndexCellEditor.hxx"

#include "moc/moc_IndexCellEditor.cpp"

IndexCellEditor::IndexCellEditor(QWidget* parent):ItemWidgetBase(parent)
{
	mainLy_ = new QGridLayout(this);

	nameLab_ = new QLabel(this);

	mainLy_->addWidget(nameLab_, 0 , 0);

	resize(100, 100);
}

IndexCellEditor::~IndexCellEditor(void)
{
}

void IndexCellEditor::setData( const boost::any& adata )
{
	ic_ = boost::any_cast<IndexCell>(adata);
	nameLab_->setText(ic_.Name);
}

boost::any IndexCellEditor::getData()
{
	return ic_;
}

int IndexCellEditor::getParentId()
{
	return ic_.parentId;
}

void IndexCellEditor::setParentId( int val )
{
	ic_.parentId = val;
}

int IndexCellEditor::getId()
{
	return ic_.Id;
}

void IndexCellEditor::setId( int val )
{
	ic_.Id = val;
}