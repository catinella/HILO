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
#pragma once

#include <QWidget>
#include <QPointer>
#include <QHash>
#include <QVector>

class ConnectionOverlay:public QWidget {
	Q_OBJECT
public:
	explicit ConnectionOverlay (QWidget * parent = nullptr);

	// Registra un "terminale" (es. PinWidget o un QPushButton che rappresenta un pin)
	void registerTerminal (const QString & id, QWidget * w);

	// Gestione connessioni
	void addConnection    (const QString & fromId, const QString & toId);
	void clearConnections ();

signals:
	void connectionAdded (const QString & fromId, const QString & toId);

protected:
	bool eventFilter (QObject * watched, QEvent * event) override;
	void paintEvent  (QPaintEvent * event)               override;
	void resizeEvent (QResizeEvent * event)              override;

private:
	QPoint terminalCenter (QWidget * w) const;
	QString idOfWidget    (QObject * w) const;

	QHash<QString, QPointer<QWidget>> m_terminals;    // id -> widget
	QHash<QWidget*, QString>          m_reverse;      // widget -> id
	QVector<QPair <QString, QString>> m_connections;  // lista cavi
	QString                           m_pendingFrom;  // primo click

private slots:
	void onTerminalRightClicked();
};
