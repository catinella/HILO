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
// Filename: ToolWidget.cpp
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
#include "ToolWidget.h"
#include <QVBoxLayout>
#include <QString>

ToolWidget::ToolWidget (int n, QWidget *parent): QWidget(parent) {
	QString label = "Tool " + QString::number(n);
	m_button = new QPushButton(label, this);
	m_pin    = new PinStrip(PWDG_TOOLSIDE, 1, this);

	auto layout = new QVBoxLayout (this);
	layout->addWidget(m_button);
	layout->addWidget(m_pin);
	setLayout (layout);

	qDebug()
		<< __PRETTY_FUNCTION__ << " : "
		<< " pin" << m_pin
		<< " parent=" << m_pin->parent()
		<< " parentWidget=" << m_pin->parentWidget()
		<< " meta=" << m_pin->metaObject()->className();


	connect (m_button, &QPushButton::pressed, this,[this] () {
		uint8_t cval = 0;
		m_pin->getValue(cval);
		cval = (cval == 0) ? 1 : 0;
		m_pin->setValue(cval);
	});
}

PinStrip* ToolWidget::pinStrip() const {
	return(m_pin);
}
