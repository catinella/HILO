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
// Filename: DragController_test.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
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

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "movableButton.h"

#define TEST_DISPLAY_WIDTH  640
#define TEST_DISPLAY_HEIGHT 480

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	auto canvas = new QWidget();
	auto mvBtn  = new movableButton("Movable-button", canvas);

	canvas->setMinimumSize(TEST_DISPLAY_WIDTH, TEST_DISPLAY_HEIGHT);

	mvBtn->move((TEST_DISPLAY_WIDTH/2), (TEST_DISPLAY_HEIGHT/2));
	mvBtn->show();
	mvBtn->raise();

#ifdef TEST_ACTIVEMODE
	mvBtn->m_drag->setControlMode(HILOWIDGET_ACTIVE);

#elif TEST_PASSIVEMODE
	mvBtn->m_drag->setControlMode(HILOWIDGET_PASSIVE);

	QPoint dragStartPos;

	QObject::connect(mvBtn->m_drag, &DragController::dragStarted, canvas, [&](const QPoint &p) {
		dragStartPos = p;
		qDebug() << "start position: X =" << p.x() << "; Y =" << p.y();
	});

	QObject::connect(mvBtn->m_drag, &DragController::dragging, [mvBtn, canvas, &dragStartPos](const QPoint &delta) {
		QPoint p = dragStartPos + delta;

		const int maxX = canvas->width()  - mvBtn->width();
		const int maxY = canvas->height() - mvBtn->height();

		p.setX(std::clamp(p.x(), 0, maxX));
		p.setY(std::clamp(p.y(), 0, maxY));

		mvBtn->move(p);
	});

#endif
	canvas->show();

	return(app.exec());
}
