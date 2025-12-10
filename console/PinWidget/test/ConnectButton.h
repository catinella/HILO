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
// Filename: ConnectButton.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This is the ConnectButton class' definition. This class is used by the test-app to create special buttons where
//	they can manage the mouse's right-button to set the graphical link.
//
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#ifndef __CONNECTBUTTON__
#define __CONNECTBUTTON__

#include <QMouseEvent>
#include <QPushButton>
#include "PinWidget.h"

//
// Class SpecialButton
//	This special button manages also the right-click events used to connect the button to a pin
//
class ConnectButton : public QPushButton {
	Q_OBJECT

public:
	using      QPushButton::QPushButton;
	void       setLinkedPin(PinWidget *pin);
	PinWidget* getLinkedPin() const;
	
signals:
	void       rightClicked();

protected:
	void       mousePressEvent(QMouseEvent *event) override;

private:
	PinWidget  *m_linkedPin = nullptr;
};

#endif
