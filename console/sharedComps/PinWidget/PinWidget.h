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
// Filename: PinWidget.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This is the PinWidget class implementation. This class allows you to create pins used to connect the virtual HILO's
//	GUI to the DUT's connector trouch the HILO pins.
//
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
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
#ifndef __PINWIDGET__
#define __PINWIDGET__

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

typedef enum {
	PWDG_DUTSIDE,
	PWDG_TOOLSIDE
} compPinSide_t;

class PinWidget: public QWidget {
	Q_OBJECT 
public:
	explicit PinWidget (compPinSide_t side, QWidget * parent = nullptr);

	bool          getValue () const;
	void          setValue (bool value, bool propagate = true);
	compPinSide_t getSide  ();

signals:
	void valueChanged (bool value);
	void leftClicked();
	void rightClicked();

protected:
	void  paintEvent      (QPaintEvent * event) override;
	void  mousePressEvent (QMouseEvent * event) override;

private:
	bool          m_value = false;
	compPinSide_t m_side;
};

#endif // PINWIDGET_H
