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

		
		for (int t=0; t<NUMOFBUTTS; t++) {
			// Mouse's LEFT button
			connect(m_buttons[t], &QPushButton::clicked, this, [this, t]() {
				m_pin->setValue(m_pin->getValue() ? 0 : 1);
			});

			// Mouse's RIGHT button
			connect(m_buttons[t], &ConnectButton::rightClicked, this, [this, t]() {
				if (m_selectedPin) {
					m_overlay->setEndpoints(m_buttons[t], m_selectedPin);
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

