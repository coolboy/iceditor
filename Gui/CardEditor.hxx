#pragma once

class Ui_CardEditor;

#include "ICCard.h"

class CardEditor : public QDialog
{
	Q_OBJECT
public:
	CardEditor(QWidget* parent);
	~CardEditor(void);

	void setIC(const ICCard& icc);
	ICCard &getIC();

//private:
	void showAll(bool bShow);
	void hideDetail();

signals:
	void accepted();
	void rejected();

private slots:
	void accept();
	void reject();

private:
	Ui_CardEditor* ui;
	ICCard icc_;
};
