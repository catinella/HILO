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
// Filename: MyWindow.cpp
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
#include "MyWindow.h"
#include "KeypadWidget.h"

#include <QVBoxLayout>
#include <QLabel>

MyWindow::MyWindow (QWidget *parent) : QWidget(parent) {
	m_keypad = new KeypadWidget (this);
	m_info = new QLabel ("Inserisci un numero e premi OK", this);

	m_keypad->setMinimumSize(200, 200);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(m_keypad);
	layout->addWidget(m_info);

	setLayout (layout);

	connect (m_keypad, &KeypadWidget::enterPressed, this, &MyWindow::onEnterPressed);

	connect (m_keypad, &KeypadWidget::textChanged, this, &MyWindow::onTextChanged);
}

void MyWindow::onEnterPressed (const QString & value) {
	m_info->setText (QString ("Hai confermato: %1").arg (value));
}

void MyWindow::onTextChanged (const QString & value) {
	m_info->setText (QString ("Digitando: %1").arg (value));
}
