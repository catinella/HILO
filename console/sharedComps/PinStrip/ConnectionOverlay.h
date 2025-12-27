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
#include "pinConnection.h"

class ConnectionOverlay:public QWidget {
	Q_OBJECT
public:
	explicit ConnectionOverlay (QWidget *parent = nullptr);

	// Single PIN registration
	void registerTerminal (const QString &id, PinWidget *w);

	// Connections resetting
	void clearConnections ();

	void paintNow ();

signals:
	void connectionAdded (const QString &fromId, const QString &toId);

protected:
	bool eventFilter (QObject *watched, QEvent* event) override;
	void paintEvent  (QPaintEvent *event)              override;
	void resizeEvent (QResizeEvent *event)             override;

private:
	bool                             delConnection  (const QString &fromId, const QString &toId);
	void                             addConnection  (const QString &fromId, const QString &toId);
	QPoint                           terminalCenter (QWidget *w) const;
	QString                          idOfWidget     (QObject *w) const;
	QVector<PinConnection>::iterator linkFinder     (QString key);

	QVector<PinConnection>           m_linksPool;
	QVector<PinConnection>::iterator selectedItem = m_linksPool.end();

private slots:
	void onTerminalRightClicked (const QString &key, PinWidget *w);
	void onTerminalValueChanged (const QString &id, PinWidget *src, bool v);
};
