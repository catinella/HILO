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
// Filename: PinStrip.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This class allows you to create stripes of pins. These objects are used to connect the virtual-instruments
//		to the DUT
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
#include "PinStrip.h"
#include <QHBoxLayout>
#include <QFrame>

//#define MAXNUMOFPINS 8

PinStrip::PinStrip (compPinSide_t side, int pinCount, QWidget *parent): QWidget(parent) {
	//
	// Description:
	//	This is the PinStrip class' constructor.
	//	It creates its own pinCount argument defined pin items and associates their valueChanged signals to a Lambda
	//	function. Everytime a pin's value will change, the associated Lambda will send a message with the new value
	//	of the whole PinStrip.
	//
	// Arguments:
	//	side       The pin connected device side {PWDG_DUTSIDE | PWDG_TOOLSIDE}
	//	pinCount   The number of PINs in the strip
	//	QWidget    The parent QWidget
	//
	auto root   = new QVBoxLayout(this);
	auto frame  = new QFrame(this);
	auto layout = new QHBoxLayout(frame);

	root->addWidget(frame, 0, Qt::AlignCenter);
	frame->setObjectName("pinStripFrame");
	frame->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	if (pinCount > 1) {
		QPalette pal = frame->palette();
		//frame->setFrameStyle(QFrame::Box | QFrame::Plain);
		frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
		frame->setLineWidth(3);
		layout->setSpacing(15);
		layout->setContentsMargins(10, 10, 10, 10);
		layout->setAlignment(Qt::AlignCenter);
		frame->setAutoFillBackground(true);
		pal.setColor(QPalette::Window, QColor(255, 204, 153));
		frame->setPalette(pal);
		
	} else {
		frame->setFrameStyle(QFrame::NoFrame);
		frame->setLineWidth(0);
	}
	
	for (int i = 0; i < pinCount; ++i) {
		auto pin = new PinWidget(side, frame);
		m_pins.append(pin);
		layout->addWidget(pin);

/*
		qDebug()
			<< __PRETTY_FUNCTION__ << " : "
			<< " pin"           << pin
			<< " parent="       << pin->parent()
			<< " parentWidget=" << pin->parentWidget()
			<< " meta="         << pin->metaObject()->className();
*/

		connect(pin, &PinWidget::valueChanged, this, [this](void) {
			uint8_t v = 0;
			getValue(v);
			emit valuesChanged(v);
		});
	}

	setLayout (layout);

	return;
}

int PinStrip::pinSrtipSize () const {
	//
	// Description:
	//	It returns the number of PINs belong to the PinStripke
	
	//
	return m_pins.size();
}

void PinStrip::setValue (uint8_t value) {
	//
	// Description:
	//	It sets the PinStrip's value.
	//	[!] When every bit is set to a new value, it emits a new PinWidget::valueChanged signal that will execute
	//	    the associated Lambda that will call PinStrip::setValue() method! It will generate an endless loop.
	//	    To avoid this trouble, this function stops all pin's signal before to set it with a new value.
	//
	for (int i = 0; i < m_pins.size(); i++) {
		bool bitValue = (value & (1 << i)) > 0 ? 1 : 0;
		//printf("%s: %d)%d\n", __PRETTY_FUNCTION__, i, bitValue);
		//QSignalBlocker b(m_pins[i]);
		m_pins[i]->setValue(bitValue);
	}
	return;
}

void PinStrip::getValue (uint8_t &value) {
	//
	// Description:
	//	It allows you to get the PinStrip's current value
	//
	value = 0;
	for (int i = 0; i < m_pins.size(); i++) {
		if (m_pins[i]->getValue() == 1)
			value |= (1 << i);
	}
	return;
}

void PinStrip::setValue (uint8_t index, bool value) {
	//
	// Description:
	//	It sets the pin's value for a pin belong to the PinStrip.
	//	[!] Read PinStrip::setValue description for the reason of the signal blocking
	//
	QSignalBlocker b(m_pins[index]);
	m_pins[index]->setValue(value);
	return;
}

void PinStrip::getValue (uint8_t index, bool &value) {
	//
	// Description:
	//	It allows you to get the index argument defined pin's value.
	//
	value = m_pins[index]->getValue();
	return;
}

PinWidget* PinStrip::getPin(int i) const {
	//
	// Description:
	//	It returns the address of the PIN in the argument defined position
	//	This funzion is used to catch the PIN's signal
	//
	PinWidget *w = nullptr;
	if (i >= 0 && i < m_pins.size())
		w = m_pins[i];
	else {
		// ERROR!
	}
	return w;
}
