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
// Filename: DragController.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This class allows you to move the HILO's widgets around the window. The widget's movement (dragging) can be 
//		performed in two way: active or passive mode. 
//
//		Passive mode:
//		=============
//
//		USER               DragController           app             widget
//		  |                       |                  |                |
//		  |      left-button push |                  |                |
//		  +======================>|   dragStarted(P0)|                |
//		  |                       +----------------->|                |
//		  |                       |                  +---+            |
//		  |                       |                  |   | store      |
//		  |                       |                  |   | X0         |
//		  |                       |                  |<--+            |
//		  |        mouse movement |                  |                |
//		  +======================>|   dragging(delta)|                |
//		  |                       +----------------->|  move(P0+delta)|
//		  |                       |                  +~~~~~~~~~~~~~~~>|
//		  |                       |                  |                |
//		///////////////////////////////////////////////////////////////////
//		  |                       |                  |                |
//		  |   left-button release |                  |                |
//		  +======================>|                  |                |
//		  |                       +---+              |                |
//		  |                       |   | flag         |                |
//		  |                       |   | resetting    |                |
//		  |                       |<--+              |                |
//		  |                       |                  |                |
//
//		+--------+----------------------+
//		| Symbol | Description          |
//		+--------+----------------------+
//		|   ===  | user activity        |
//		|   ---  | QT signal            |
//		|   ~~~  | Fuction/method call  |
//		+--------+----------------------+
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

#include "DragController.h"
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

DragController::DragController(QWidget *owner) : QObject(owner), m_owner(owner) {
	Q_ASSERT(m_owner);
	m_owner->installEventFilter(this);
}

bool DragController::eventFilter(QObject *obj, QEvent *ev) {
	bool out = false;

	if (m_enabled && obj == m_owner) {
		QMouseEvent *e = nullptr;
		QPoint gpos;

		switch (ev->type()) {
			case QEvent::MouseButtonPress: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (e->button() == Qt::LeftButton) {
					m_dragging         = true;
					m_pressGlobal      = gpos;
					m_startPosInParent = m_owner->pos();

					// Receiving mouse move/release events aoutside the widget's area
					m_owner->grabMouse();

					// Signal sending
					emit dragStarted(m_startPosInParent);
					
					out = true;
				}
				break;;
			}

			case QEvent::MouseMove: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (m_dragging) {
					const QPoint delta = gpos - m_pressGlobal;

					if (getControlMode() == HILOWIDGET_ACTIVE)
						m_owner->move(m_startPosInParent + delta);

					// Signal sending
					emit dragging(delta);
					
					out = true;
				}
				break;;
			}

			case QEvent::MouseButtonRelease: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (m_dragging && e->button() == Qt::LeftButton) {
					m_dragging = false;
					m_owner->releaseMouse();

					// Signal sending
					emit dragFinished();
					
					out = true;
				}
				break;;
			}

			default:
				break;
		}

		//if (out == false) out = QObject::eventFilter(obj, ev);
		out = QObject::eventFilter(obj, ev);
	}

	return(out);
}



void DragController::setEnabled(bool en) {
	m_enabled = en;
}


bool DragController::isEnabled() const {
	return(m_enabled);
}

hiloControlMode_t DragController::getControlMode() const {
	return(m_controlMode);
}

void DragController:: setControlMode(hiloControlMode_t mode) {
	m_controlMode = mode;
}
