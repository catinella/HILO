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
// Filename: test.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This folder hosts the test of the PinStrip widget.
//		The test will display a fake device with 16 pins. You can connect and disconnect every item belongs to the PinStrip to one
//		those 16 pins
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

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QCheckBox>
#include <QVector>
#include "PinStrip.h"

#define TEST_NUMOFPINS 8

class TestWindow:public QMainWindow {
public:
	explicit TestWindow (QWidget * parent = nullptr): QMainWindow (parent) {

		auto *central       = new QWidget(this);
		auto *layout        = new QVBoxLayout(central);
		auto *buttonsLayout = new QHBoxLayout;

		strip = new PinStrip(central);
		
		layout->addLayout(buttonsLayout);
		layout->addWidget(strip);
		layout->setContentsMargins(10, 10, 10, 10);
		setCentralWidget(central);
		setWindowTitle("PinStrip test window");
		resize(300, 500);

		// Buttons creation...
		for (int i = 0; i < TEST_NUMOFPINS; ++i) {
			auto *btn = new QPushButton(QString::number(i), this);
			btn->setCheckable(true);
			buttonsLayout->addWidget(btn);
			m_buttons.push_back(btn);
		}

		// Looking for the center points used for the graphical linking
		{
			for (int i = 0; i < TEST_NUMOFPINS; ++i) {
				m_pins.push_back(
					strip->findChild<QCheckBox*>(QString("pin%1").arg(i))
				);
			}
		}

		// Connections drawing...
		for (int i = 0; i < TEST_NUMOFPINS; ++i) {
			if (m_buttons[i] && m_pins[i]) {
				connect(m_buttons[i], &QPushButton::toggled, this, [this, i](bool checked) {
					if (m_pins[i]) m_pins[i]->setChecked(checked);
					update();   // ridisegna le linee (non strettamente necessario, ma esplicito)
				});
			}
		}

		layout->setContentsMargins(10, 10, 10, 10);
		layout->setSpacing(10);
	}

protected:
	void paintEvent(QPaintEvent *event) override {
		QMainWindow::paintEvent(event);

		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing, true);

		QPen pen;
		pen.setWidth(1);
		p.setPen(pen);

		int n = std::min(m_buttons.size(), m_pins.size());

		for (int i = 0; i < n; ++i) {
			if (!m_buttons[i] || !m_pins[i])
			continue;

			// centro del pulsante in coordinate della finestra
			QPoint bCenter = m_buttons[i]->mapTo(this,
			m_buttons[i]->rect().center());

			// centro del checkbox in coordinate della finestra
			QPoint pinCenter = m_pins[i]->mapTo(this,
			m_pins[i]->rect().center());

			p.drawLine(bCenter, pinCenter);
		}
	}

private:
	PinStrip *strip = nullptr;
	QVector<QPushButton*> m_buttons;
	QVector<QCheckBox*>   m_pins;
};

int main (int argc, char *argv[]) {
	QApplication app (argc, argv);

	TestWindow w;
	w.show ();

	return app.exec ();
}
