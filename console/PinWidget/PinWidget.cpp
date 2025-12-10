#include "PinWidget.h"
#include <QPainter>
#include <QMouseEvent>

PinWidget::PinWidget (QWidget *parent): QWidget (parent) {
	//
	// Description:
	//	This is the Object builder method
	//
	setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Minimum);
	setFixedSize(8, 16);
}

bool PinWidget::getValue () const {
	//
	// Description:
	//	It returns the pin's value
	//
	return(m_value);
}

void PinWidget::setValue (bool v) {
	//
	// Description:
	//	It sets the pin value
	//
	if (m_value != v) {
		m_value = v;
		update ();
		emit valueChanged (m_value);
	}
}

void PinWidget::paintEvent (QPaintEvent *) {
	//
	// Description:
	//	Ii draws the pin.
	//	This method overrides the virtual one that is inherited by QWidget. 
	//	update() → [Qt events manager] →  paintEvent()
	//
	QPainter p(this);
	p.setRenderHint (QPainter::Antialiasing, true);

	p.fillRect (rect(), Qt::transparent);

	QRect r = rect().adjusted (2, 2, -2, -2);

	QColor fill = m_value ? QColor ("#00ff00") : QColor ("#303030");

	p.setPen (QColor ("#202020"));
	p.setBrush (fill);
	p.drawRoundedRect (r, 3, 3);
}

void PinWidget::mousePressEvent (QMouseEvent *event) {
	//
	// Description:
	//	It dispatches the event-signals
	//	This method overrides the virtual one that is inherited by QWidget.
	//	mouse-click → [Qt event loop] → mousePressEvent()
	//
	if (event->button() == Qt::LeftButton) {
		// NOT USED! at the moment
		emit leftClicked();
		
	} else if (event->button() == Qt::RightButton) {
		// Pin linking
		emit rightClicked();
	}

	QWidget::mousePressEvent(event);
}
