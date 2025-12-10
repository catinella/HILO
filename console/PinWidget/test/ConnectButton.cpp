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
// Filename: ConnectButton.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This is the ConnectButton class' implementation. This class is used by the test-app to create special buttons where
//	they can manage the mouse's right-button to set the graphical link.
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
#include "ConnectButton.h"

void ConnectButton::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton)
		emit rightClicked();

	else if (event->button() == Qt::LeftButton) {
		QPushButton::mousePressEvent(event);
		if (m_linkedPin)
			// PIN's color chaging
			m_linkedPin->setValue(m_linkedPin->getValue() ? 0 : 1);
	} else
		QPushButton::mousePressEvent(event);

	return;
}

void ConnectButton::setLinkedPin(PinWidget *pin) {
	m_linkedPin = pin;
	return;
}

PinWidget* ConnectButton::getLinkedPin() const {
	return m_linkedPin;
}
