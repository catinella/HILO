//------------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//
//
//
//
//------------------------------------------------------------------------------------------------------------------------------
#include "KeypadWidget.h"
#include "ui_KeypadWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

KeypadWidget::KeypadWidget (QWidget *parent) : QWidget(parent), ui(new Ui::KeypadWidget) {
	ui->setupUi (this);
	digitButtons = {
		ui->pushButton_0,
		ui->pushButton_1,
		ui->pushButton_2,
		ui->pushButton_3,
		ui->pushButton_4,
		ui->pushButton_5,
		ui->pushButton_6,
		ui->pushButton_7,
		ui->pushButton_8,
		ui->pushButton_9
	};

	//
	// Numeric buttons
	//
	for (auto btn : digitButtons)
		connect (btn, &QPushButton::clicked, this, &KeypadWidget::onDigitClicked);
	
	
	//
	// Special buttons
	//
	connect (ui->pushButton_canc, &QPushButton::clicked, this, &KeypadWidget::onClearClicked);
	connect (ui->pushButton_del,  &QPushButton::clicked, this, &KeypadWidget::onBackspaceClicked);
	connect (ui->pushButton_ack,  &QPushButton::clicked, this, &KeypadWidget::onOkClicked);
}

KeypadWidget::~KeypadWidget () {
	delete ui;
}

void KeypadWidget::appendDigit (const QString & d) {
	auto txt = ui->displayEdit->text ();	// <--- al posto di m_display
	txt.append (d);
	ui->displayEdit->setText (txt);
	emit textChanged (txt);
}

void KeypadWidget::onDigitClicked () {
	auto *btn = qobject_cast < QPushButton * >(sender ());
	if (!btn)
		return;
	const QString d = btn->text ();
	appendDigit (d);
	bool ok = false;
	int val = d.toInt (&ok);
	if (ok)
		emit digitPressed (val);
}

void KeypadWidget::onClearClicked () {
	ui->displayEdit->clear ();
	emit textChanged (QString ());
}

void KeypadWidget::onBackspaceClicked () {
	auto txt = ui->displayEdit->text ();
	if (!txt.isEmpty ()) {
		txt.chop (1);
		ui->displayEdit->setText (txt);
		emit textChanged (txt);
	}
}

void KeypadWidget::onOkClicked () {
	emit enterPressed (ui->displayEdit->text ());
}
