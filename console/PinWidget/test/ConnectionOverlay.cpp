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
// Filename: ConnectionOverlay.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	The objects belong to this class manages the graphic connection between the pin and the selected button
//
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
#include <ConnectionOverlay.h>

static QPoint anchorInOverlay(QWidget *w, QWidget *overlay, bool top) {
    QRect r = w->rect();
    QPoint local;

    if (top)
        // centro del lato superiore
        local = QPoint(r.center().x(), r.top());
    else
        // centro del lato inferiore
        local = QPoint(r.center().x(), r.bottom());

    QPoint global = w->mapToGlobal(local);
    return overlay->mapFromGlobal(global);
}

void ConnectionOverlay::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);

	if (m_from && m_to) {
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing, true);

		QPen pen;
		pen.setWidth(2);
		pen.setColor(Qt::black);
		p.setPen(pen);

		QPoint p1 = anchorInOverlay(m_from, this, false); 
		QPoint p2 = anchorInOverlay(m_to,   this, true); 
		p.drawLine(p1, p2);
	}

	return;
}

ConnectionOverlay::ConnectionOverlay(QWidget *parent = nullptr) : QWidget(parent) {
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);
}

void ConnectionOverlay::setEndpoints(QWidget *from, QWidget *to) {
	m_from = from;
	m_to   = to;
	update();
}

void ConnectionOverlay::clear() {
	m_from = m_to = nullptr;
	update();
}
