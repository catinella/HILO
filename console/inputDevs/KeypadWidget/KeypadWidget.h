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
// Filename: KeypadWidget.h
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

#ifndef KEYPADWIDGET_H
#define KEYPADWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <PinStrip.h>

#define KEYPAD_COLS     3
#define KEYPAD_ROWS     4
#define KEYPAD_XBTNSIZE 32
#define KEYPAD_YBTNSIZE 32
#define KEYPAD_BTNSLABS {"1","2","3","4","5","6","7","8","9","CLR","0","DEL","OK"};
#define KEYPAD_DATABITS 8

class QLineEdit;

namespace Ui {
	class KeypadWidget;
}

class KeypadWidget:public QWidget {
    Q_OBJECT

public:
	explicit  KeypadWidget (QWidget * parent = nullptr);
	         ~KeypadWidget ();
	PinStrip* getMyPins    ();
	
signals:
	void digitPressed (int digit);
	void textChanged  (const QString & text);
	void enterPressed (const QString & text);

private slots:
	void onDigitClicked ();
	void onClearClicked ();
	void onBackspaceClicked ();
	void onOkClicked ();
	
private:
	// Defined by ui_KeypadWidget.h
	Ui::KeypadWidget *ui;

	QVector<QPushButton*> digitButtons;
	PinStrip              *myPins;
	
	void appendDigit (const QString &d);
};

#endif // KEYPADWIDGET_H
