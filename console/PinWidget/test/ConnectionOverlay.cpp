#include <ConnectionOverlay.h>

void ConnectionOverlay::paintEvent(QPaintEvent *event) {
	QWidget::paintEvent(event);

	if (m_from && m_to) {
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing, true);

		QPen pen;
		pen.setWidth(2);
		pen.setColor(Qt::black);
		p.setPen(pen);

		QPoint p1 = m_from->mapTo(this, m_from->rect().center());
		QPoint p2 = m_to->mapTo(this,   m_to->rect().center());
		p.drawLine(p1, p2);
	}

	return;
}

ConnectionOverlay::ConnectionOverlay(QWidget *parent = nullptr) : QWidget(parent) {
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);
}

void ConnectionOverlay::setEndpoints(QWidget *from, QWidget *to) {
	m_from = from;
	m_to   = to;
	update();
}

void ConnectionOverlay::clear() {
	m_from = m_to = nullptr;
	update();
}
