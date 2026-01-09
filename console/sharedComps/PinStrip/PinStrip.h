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
// Filename: PinStrip.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This class allows you to create stripes of pins. These objects are using to connect the virtual-instruments
//		to the DUT
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
#ifndef __PINSTRIP__
#define __PINSTRIP__

#include <QWidget>
#include "PinWidget.h"
#include "DragController.h"

class PinStrip:public QWidget {
	Q_OBJECT
public:
	explicit PinStrip (compPinSide_t side, int pinCount = 8, QWidget * parent = nullptr);

	unsigned int    getSize     () const;
	void            setValue    (uint8_t value);
	void            getValue    (uint8_t &value) const;
	void            setValue    (uint8_t index, bool value);
	void            getValue    (uint8_t index, bool &value) const;
	PinWidget*      getPin      (int i) const;
	DragController* getDragger  () const;
	QPoint          getPosition () const;
	
signals:
	void valuesChanged (uint8_t newValues);

private:
	void setPosition (QPoint currPosition);
	
	QVector<PinWidget*> m_pins;
	compPinSide_t       m_side;
	DragController      *m_dragger = nullptr;
	QPoint              m_position;
};

#endif
