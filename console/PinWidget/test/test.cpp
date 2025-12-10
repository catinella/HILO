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
//	This source file is a test for the PinWidget class. Compiling and executing this file, a graphic window will be
//	dysplaied. In the window you will see a list ob buttons and a single pin. You can connect the pin to every button
//	using the right button on the pin and on the target button. Then you can verify the pin's color will change when you
//	will push the connected button.
//
//
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "ConnectButton.h"
#include "PinWidget.h"
#include "ConnectionOverlay.h"

#define NUMOFBUTTS 3

class TestWidget : public QWidget {
public:
	explicit TestWidget(QWidget *parent = nullptr) : QWidget(parent) {
		auto *layout       = new QVBoxLayout(this);
		auto *buttonLayout = new QHBoxLayout(this);
		
		{
			ConnectButton *butt = nullptr;
			for (int t=0; t<NUMOFBUTTS; t++) {
		 		butt = new ConnectButton("Toggle / Connect", this);
				buttonLayout->addWidget(butt);
				m_buttons.append(butt);
			}
		}

		layout->addLayout(buttonLayout);

		m_pin = new PinWidget(this);
		layout->addWidget(m_pin, 0, Qt::AlignHCenter);

		// overlay sopra tutto
		m_overlay = new ConnectionOverlay(this);
		m_overlay->resize(size());
		m_overlay->raise();

		
		// Mouse's LEFT button
		connect(m_pin, &PinWidget::rightClicked, this, [this]() {
			m_selectedPin = m_pin;
		});

		for (int t=0; t<NUMOFBUTTS; t++) {
			
			// Mouse's RIGHT button
			connect(m_buttons[t], &ConnectButton::rightClicked, this, [this, t]() {
				if (m_selectedPin) {
					// Graphic link
					m_overlay->setEndpoints(m_buttons[t], m_selectedPin);

					// Logic link
					m_buttons[t]->setLinkedPin(m_selectedPin);
					
					m_selectedPin = nullptr;  // consumo la selezione
				}
			});
		}
		
		// DESTRO sul pin: lo seleziono per connessione
		connect(m_pin, &PinWidget::rightClicked, this, [this]() {
			m_selectedPin = m_pin;
			// volendo qui potresti evidenziare graficamente il pin selezionato
		});


		setWindowTitle("Right-click connect test");
		resize(300, 200);
	}

protected:
	void resizeEvent(QResizeEvent *event) override {
		QWidget::resizeEvent(event);
		if (m_overlay)
			m_overlay->resize(size());
	}

private:
	QVector <ConnectButton*> m_buttons;
	PinWidget               *m_pin         = nullptr;
	ConnectionOverlay       *m_overlay     = nullptr;
	PinWidget               *m_selectedPin = nullptr;
};

//------------------------------------------------------------------------------------------------------------------------------
//                                                        M A I N
//------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	TestWidget w;
	w.show();

	return app.exec();
}

