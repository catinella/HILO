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
// Filename: HILO_console.cpp
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
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include "KeypadWidget.h"

void onEnterPressed (const unsigned int &value) {
	qDebug() << "Typed value: " << value;
}

void onTextChanged (const QString &value) {
	qDebug() << "You typed " << value;
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	auto window = new QWidget();
	auto keypad = new KeypadWidget(window);

	QVBoxLayout *layout = new QVBoxLayout(window);
	layout->addWidget(keypad->getMyPins());
	layout->addWidget(keypad);
	
	window->setMinimumSize(640, 480);
	window->show();

	QObject::connect(keypad, &KeypadWidget::enterPressed, &onEnterPressed);
	QObject::connect(keypad, &KeypadWidget::textChanged,  &onTextChanged);

	QObject::connect(keypad->getMyPins(), &PinStrip::dragging, [keypad, window](const QPoint &delta) {
		auto dutStrip = keypad->getMyPins();
            QPoint p = dutStrip->position() + delta;

            const int maxX = window->width()  - dutStrip->width();
            const int maxY = window->height() - dutStrip->height();

            p.setX(std::clamp(p.x(), 0, maxX));
            p.setY(std::clamp(p.y(), 0, maxY));

            dutStrip->moveTo(p);

     //       overlay->paintNow();
      });

	return(app.exec());
}

