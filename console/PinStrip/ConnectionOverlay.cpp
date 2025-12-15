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
	//
	// Description:
	//	It is the lonely class' constructor
	//

	// [!] Because the Overlay is put on the top ot the graphich layers stack. WA_TransparentForMouseEvents property
	//     must be set to FALSE, else every mouse events will be disabled
	setAttribute (Qt::WA_TransparentForMouseEvents, true);

	setAttribute (Qt::WA_NoSystemBackground, true);

	setAttribute (Qt::WA_TranslucentBackground, true);

	// IMPORTANT: Overlay must be on all graphic elements
	raise ();
}


void ConnectionOverlay::registerTerminal (const QString & id, PinWidget *w) {
	//
	// Description:
	//	This method associate the argument defined object's signal rightClicked to the onTerminalRightClicked()
	//	slot.
	//
	if (w != nullptr && id.isEmpty () == false) {
		qDebug() << "REGISTER" << id << "w=" << w << (w ? w->metaObject()->className() : "NULL");
		
		connect(w, &PinWidget::rightClicked, this, [this, key=id, w](auto...){
			onTerminalRightClicked(key, w);
		});
	}
}


void ConnectionOverlay::clearConnections () {
	//
	// Description:
	//	It clears all the sored connections
	//
	m_linksPool.clear ();
	update ();
}


bool ConnectionOverlay::eventFilter (QObject *watched, QEvent *event) {
/*
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

	return(handled ? true : baseHandled);
*/
	return(true);
}


void ConnectionOverlay::paintEvent (QPaintEvent *) {
	//
	// Description:
	//	This method is called by QT internal loop, and draws the link between the selected items.
	//	[!] All the linked items and the selected item, are stored in the m_connections property
	//
	QPainter  p(this);
	PinWidget *a = nullptr;
	PinWidget *b = nullptr;
	QString   aid = "";
	QString   bid = "";
	
	p.setRenderHint(QPainter::Antialiasing, true);
	
	// All links drawing...
	for (const auto &item : m_linksPool) {
		if (item.isValid()) {
                  item.getItems(aid, bid, a, b);
                    
			const QPoint pa = terminalCenter(a);
			const QPoint pb = terminalCenter(b);

			// marker giganteschi
			p.drawEllipse(pa, 10, 10);
			p.drawEllipse(pb, 10, 10);
			
			p.drawLine(pa, pb);
			
		} else {
			QWidget *ptr = nullptr;
                  item.getItems(aid, bid, a, b);
                  if (a != nullptr) ptr = a;
                  else              ptr = b;
			if (ptr != nullptr) {
				const QPoint pa = terminalCenter (ptr);
				// Selected item marker
				p.drawEllipse (pa, 6, 6);
			}
		}
	}

	return;
}


void ConnectionOverlay::resizeEvent (QResizeEvent *e) {
	QWidget::resizeEvent(e);
	update();
}


//------------------------------------------------------------------------------------------------------------------------------
//                                          P R I V A T E   M E T H O D S
//------------------------------------------------------------------------------------------------------------------------------
/*
void ConnectionOverlay::addConnection (const QString &fromId, const QString &toId) {
	//
	// Description:
	//	It checks for the argument defined Object-IDs and if they are ok then it stores them in the m_connections property.
	//	The property will be used by paintEvent() method to draw all stored links 
	//
	if (fromId.isEmpty() || toId.isEmpty()) {
		qCritical() << __PRETTY_FUNCTION__ << ": Invalid arguments";

	} else if (!m_terminals.contains(fromId) || !m_terminals.contains(toId)) {
		qWarning() << __PRETTY_FUNCTION__ << ": ??";

	} else {
		qDebug() << __PRETTY_FUNCTION__ << ": new connect append";
		m_connections.append({fromId, toId});
		update ();
	}
}
*/

QPoint ConnectionOverlay::terminalCenter(QWidget *w) const {
	//
	// Description:
	//	This methos accepts a widget as argument and returns coordinates of the widget's center point
	//
	auto out = QPoint();
	if (w) {
		const QPoint c(w->width() / 2, w->height() / 2);
		const QPoint g = w->mapToGlobal(c);
		out = this->mapFromGlobal(g);
	}
	return(out);
}

/*
QString ConnectionOverlay::idOfWidget (QObject *w) const {
	auto it = m_reverse.find (static_cast < QWidget * >(w));
	return (it == m_reverse.end ())? QString () : it.value ();
}
*/

/*
bool ConnectionOverlay::delConnection (const QString & fromId, const QString & toId) {
	m_connections.remove({fromId, toId});
}
*/

//------------------------------------------------------------------------------------------------------------------------------
//                                                     S L O T S
//------------------------------------------------------------------------------------------------------------------------------
void ConnectionOverlay::onTerminalRightClicked(const QString &key, PinWidget *w) {
	//
	// Description:
	//	This is the slot associated to the rightClicked signal emitted by virtual tools' pin or by DUT's stripe pins
	//
	
	qDebug() << "RIGHTCLICK caught, sender id =" << key;

	if (selectedItem == m_linksPool.end()) {
		selectedItem = std::find_if(
			m_linksPool.begin(), m_linksPool.end(), [&](const PinConnection &c){return c.involves(key);}
		);
		
		if (selectedItem != m_linksPool.end()) {
			m_linksPool.erase(selectedItem);
			selectedItem = m_linksPool.end();
			
		} else {
			// New item creation
			m_linksPool.append(PinConnection(key, w));
			selectedItem = std::prev(m_linksPool.end());
		}
	} else {
		// The link is completed
		selectedItem->addItem(key, w);
		selectedItem = m_linksPool.end();
	}
	
	update();
	
	return;
}
