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
//	
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
#include "ConnectionOverlay.h"
#include "PinWidget.h"

#include <QPainter>
#include <QMouseEvent>

ConnectionOverlay::ConnectionOverlay (QWidget *parent):QWidget (parent) {
	setAttribute (Qt::WA_TransparentForMouseEvents, false);
	setAttribute (Qt::WA_NoSystemBackground, true);
	setAttribute (Qt::WA_TranslucentBackground, true);

	// IMPORTANT: l'overlay deve stare sopra
	raise ();
}

void ConnectionOverlay::registerTerminal (const QString & id, QWidget *w) {
	if (w != nullptr && id.isEmpty () == false) {
		m_terminals[id] = w;
		m_reverse[w]    = id;

		qDebug() << "REGISTER" << id << "w=" << w << (w ? w->metaObject()->className() : "NULL");
		
		/*
		if (auto *pin = dynamic_cast<PinWidget*>(w)) {

            
			connect (pin, &PinWidget::rightClicked, this,[this, id] () {
				qDebug() << __PRETTY_FUNCTION__ << ": PinWidget::rightClicked signal catched";
				
				if (m_pendingFrom.isEmpty()) {
					qDebug() << __PRETTY_FUNCTION__ << ": From = " << id;
					m_pendingFrom = id;
					update();
				} else {
					const QString from = m_pendingFrom;
					const QString to = id;
					qDebug() << __PRETTY_FUNCTION__ << ": To = " << to;
					m_pendingFrom.clear();
					if (from != to) {
						addConnection(from, to);
						emit connectionAdded(from, to);
					}
					update ();
				}
			});
		}
		*/
		bool ok = QObject::connect(w, SIGNAL(rightClicked()), this, SLOT(onTerminalRightClicked()));
		qCritical() << "CONNECT rightClicked ok=" << ok;
	}
}

void ConnectionOverlay::addConnection (const QString & fromId, const QString & toId) {
	if (fromId.isEmpty () || toId.isEmpty ()) {
		qCritical() << __PRETTY_FUNCTION__ << ": Invalid arguments";

	} else if (!m_terminals.contains(fromId) || !m_terminals.contains(toId)) {
		qWarning() << __PRETTY_FUNCTION__ << ": ??";

	} else {
		qDebug() << __PRETTY_FUNCTION__ << ": new connect append";
		m_connections.append ({fromId, toId});
		update ();
	}
}

void ConnectionOverlay::clearConnections () {
	m_connections.clear ();
	m_pendingFrom.clear ();
	update ();
}

QString ConnectionOverlay::idOfWidget (QObject *w) const {
	auto it = m_reverse.find (static_cast < QWidget * >(w));
	return (it == m_reverse.end ())? QString () : it.value ();
}

bool ConnectionOverlay::eventFilter (QObject *watched, QEvent *event) {
	bool handled = false;

	if (event != nullptr) {
		if (event->type () == QEvent::MouseButtonPress) {

			QWidget *mw = qobject_cast < QWidget * >(watched);
			if (mw != nullptr) {

				const QString id = idOfWidget (watched);
				if (id.isEmpty () == false) {

					if (m_pendingFrom.isEmpty ()) {
						// Primo endpoint
						m_pendingFrom = id;
						update ();
						handled = true;
						qDebug() << __PRETTY_FUNCTION__ << ": From = " << id;

					} else {
						// Secondo endpoint
						const QString from = m_pendingFrom;
						const QString to = id;

						qDebug() << __PRETTY_FUNCTION__ << ": To = " << to;
						m_pendingFrom.clear ();

						if (from != to) {
							addConnection (from, to);
							emit connectionAdded (from, to);
						}

						update ();
						handled = true;
					}
				}
			}
		}
	}
	// 2) Se un terminale si muove/resizea: ridisegna
	if (!handled) {
		if (event != nullptr) {
			if (event->type () == QEvent::Move || event->type () == QEvent::Resize) {
				update ();
				// qui puoi scegliere: handled=true o false.
				// Io lo lascio false, così l'evento prosegue normalmente.
			}
		}
	}
	// 3) Default: lascia fare a QWidget
	bool baseHandled = false;
	if (!handled) {
		baseHandled = QWidget::eventFilter (watched, event);
	}

	return handled ? true : baseHandled;
}

QPoint ConnectionOverlay::terminalCenter (QWidget *w) const {
	QPoint g;
	if (w) {
		const QPoint c(w->width()/2, w->height()/2);
		return w->mapTo(const_cast<ConnectionOverlay*>(this), c);
	}
	return (mapFromGlobal (g));
}

void ConnectionOverlay::paintEvent (QPaintEvent *) {
	QPainter p (this);
	p.setRenderHint (QPainter::Antialiasing, true);
	
	qCritical() << "PAINT overlay size=" << size()
		<< " connections=" << m_connections.size()
		<< " visible=" << isVisible();
		
	// disegna cavi esistenti
	for (const auto & c:m_connections) {
		QWidget *a = m_terminals.value (c.first, nullptr);
		QWidget *b = m_terminals.value (c.second, nullptr);
		qCritical() << "DRAW ids" << c.first << "->" << c.second << " a "
			<< (a ? a->metaObject()->className() : "NULL") << " b "
			<< (b ? b->metaObject()->className() : "NULL");
                    
		if (a != nullptr || b != nullptr) {
			const QPoint pa = terminalCenter (a);
			const QPoint pb = terminalCenter (b);

			qCritical() << "PTS" << pa << "->" << pb;

			// marker giganteschi
			p.drawEllipse(pa, 10, 10);
			p.drawEllipse(pb, 10, 10);
			
			p.drawLine (pa, pb);
		}
	}

	// disegna “cavo pendente” (solo come feedback: dal from verso il suo centro)
	if (!m_pendingFrom.isEmpty ()) {
		QWidget *a = m_terminals.value (m_pendingFrom, nullptr);
		if (a) {
			const QPoint pa = terminalCenter (a);
			// piccolo segno per indicare selezione
			p.drawEllipse (pa, 6, 6);
		}
	}
}

void ConnectionOverlay::resizeEvent (QResizeEvent *e) {
	QWidget::resizeEvent (e);
	update ();
}


void ConnectionOverlay::onTerminalRightClicked()
{
    QWidget *w = qobject_cast<QWidget*>(sender());
    const QString id = m_reverse.value(w);

    qCritical() << "RIGHTCLICK caught, sender id =" << id;

    if (id.isEmpty()) return;

    if (m_pendingFrom.isEmpty()) {
        m_pendingFrom = id;
        update();
    } else {
        const QString from = m_pendingFrom;
        const QString to   = id;
        m_pendingFrom.clear();
        if (from != to) {
            addConnection(from, to);
            emit connectionAdded(from, to);
        }
        update();
    }
}
