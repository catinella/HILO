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
// Filename: ConnectionOverlay.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	The objects belong to this class manages the graphic connection between the pin and the selected button
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
#ifndef __CONNECTIONOVERLAY__
#define __CONNECTIONOVERLAY__

#include <QWidget>
#include <QPoint>
#include <QPainter>

class ConnectionOverlay: public QWidget {
	Q_OBJECT
public:
	explicit ConnectionOverlay(QWidget *parent);
	void     setEndpoints(QWidget *from, QWidget *to);
	void     clear();

protected:
	void     paintEvent(QPaintEvent *event) override; 

private:
	QWidget *m_from = nullptr;
	QWidget *m_to   = nullptr;
};

#endif
