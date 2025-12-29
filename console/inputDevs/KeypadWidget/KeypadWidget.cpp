/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
//
//
// Filename: KeypadWidget.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//
//
//
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include "KeypadWidget.h"
#include "ui_KeypadWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

void KeypadWidget::appendDigit (const QString & d) {
	auto txt = ui->displayEdit->text();	// <--- al posto di m_display
	txt.append(d);
	ui->displayEdit->setText(txt);
	emit textChanged(txt);
}

//------------------------------------------------------------------------------------------------------------------------------
//                                          P U B L I C   M E T H O D S
//------------------------------------------------------------------------------------------------------------------------------
KeypadWidget::KeypadWidget (QWidget *parent): QWidget(parent), ui(new Ui::KeypadWidget) {
	ui->setupUi(this);
	digitButtons = {
		ui->pushButton_0, ui->pushButton_1, ui->pushButton_2, ui->pushButton_3, ui->pushButton_4,
		ui->pushButton_5, ui->pushButton_6, ui->pushButton_7, ui->pushButton_8, ui->pushButton_9
	};

	//
	// Numeric buttons
	//
	for (auto btn : digitButtons)
		connect(btn, &QPushButton::clicked, this, &KeypadWidget::onDigitClicked);
	
	//
	// Special buttons
	//
	connect(ui->pushButton_canc, &QPushButton::clicked, this, &KeypadWidget::onClearClicked);
	connect(ui->pushButton_ok,  &QPushButton::clicked, this, &KeypadWidget::onOkClicked);

	// Pin strip creation
	myPins = new PinStrip(PWDG_TOOLSIDE, KEYPAD_DATABITS, parent);
}

KeypadWidget::~KeypadWidget () {
	delete ui;
}

PinStrip* KeypadWidget::getMyPins () {
	return(myPins);
}


//------------------------------------------------------------------------------------------------------------------------------
//                                              P R I V A T E   S L O T S
//------------------------------------------------------------------------------------------------------------------------------

void KeypadWidget::onDigitClicked () {
	//
	// Description:
	//	This slot is connected to every numeric button. It add the typed digit to the number you see on the Keypad's display
	//
	auto *btn = qobject_cast<QPushButton*>(sender());
	if (btn) {
		const QString d = btn->text();
		appendDigit(d);
		bool ok = false;
		int val = d.toInt(&ok);
		if (ok)
			emit digitPressed(val);
	}
}

void KeypadWidget::onClearClicked () {
	ui->displayEdit->clear();
	emit textChanged(QString());
}

void KeypadWidget::onOkClicked() {
	unsigned int currValue = ui->displayEdit->text().toInt();
	if (currValue < std::pow(2.0, KEYPAD_DATABITS) && oldValue != currValue) {
		oldValue = currValue;
		emit enterPressed(currValue);
	} else {
		qCritical() << "Syntax ERROR!";
		ui->displayEdit->setText(QString::number(oldValue));
	}
}
