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
// Filename: PinWidget.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This is the PinWidget class implementation. This class allows you to create pins used to connect the virtual HILO's
//	GUI to the DUT's connector trouch the HILO pins.
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
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
#include "PinWidget.h"

PinWidget::PinWidget (QWidget *parent): QWidget (parent) {
	//
	// Description:
	//	This is the Object builder method
	//
	setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Minimum);
	setFixedSize(8, 16);
}

bool PinWidget::getValue () const {
	//
	// Description:
	//	It returns the pin's value
	//
	return(m_value);
}

void PinWidget::setValue (bool v) {
	//
	// Description:
	//	It sets the pin value
	//
	if (m_value != v) {
		m_value = v;
		update ();
		emit valueChanged (m_value);
	}
}

void PinWidget::paintEvent (QPaintEvent *) {
	//
	// Description:
	//	Ii draws the pin.
	//	This method overrides the virtual one that is inherited by QWidget. 
	//	update() → [Qt events manager] →  paintEvent()
	//
	QPainter p(this);
	p.setRenderHint (QPainter::Antialiasing, true);

	p.fillRect (rect(), Qt::transparent);

	QRect r = rect().adjusted (2, 2, -2, -2);

	QColor fill = m_value ? QColor ("#00ff00") : QColor ("#303030");

	p.setPen (QColor ("#202020"));
	p.setBrush (fill);
	p.drawRoundedRect (r, 3, 3);
}

void PinWidget::mousePressEvent (QMouseEvent *event) {
	//
	// Description:
	//	It dispatches the event-signals
	//	This method overrides the virtual one that is inherited by QWidget.
	//	mouse-click → [Qt event loop] → mousePressEvent()
	//
	if (event->button() == Qt::LeftButton) {
		// NOT USED! at the moment
		emit leftClicked();
		
	} else if (event->button() == Qt::RightButton) {
		// Pin linking
		emit rightClicked();
	}

	QWidget::mousePressEvent(event);
}
