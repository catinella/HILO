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
// Filename: DragController.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This class allows you to move the HILO's widgets around the window. The widget's movement (dragging) can be 
//		performed in two way: active or passive mode. The active mode is the simpliest one, the DragController's
//		object manage everithing for you. But for this reason you cannot force the widget to be moved just inside a
//		screen area, only. Instead in the PASSIVE mode, the DragController's object will just emit one signal for every
//		important mouse event, and you will be free to move the widget as you want. To switch the function mode, use
//		the setControlMode({HILOWIDGET_ACTIVE | HILOWIDGET_PASSIVE}) object's method.
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

#pragma once

#include <QObject>
#include <QWidget>
#include <QPoint>

typedef enum {
	HILOWIDGET_PASSIVE,
	HILOWIDGET_ACTIVE
} hiloControlMode_t;

class DragController : public QObject {
	Q_OBJECT
public:
	explicit          DragController(QWidget *owner);

	void              setEnabled(bool en);
	bool              isEnabled() const;
	hiloControlMode_t getControlMode() const;
	void              setControlMode(hiloControlMode_t mode);

signals:
	void dragStarted (const QPoint &startPosInParent);
	void dragging    (const QPoint &deltaInParent);
	void dragFinished();

protected:
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
	QWidget           *m_owner = nullptr;

	bool              m_enabled  = true;
	bool              m_dragging = false;
	
	hiloControlMode_t m_controlMode = HILOWIDGET_ACTIVE;
	QPoint            m_pressGlobal;
	QPoint            m_startPosInParent;
};

