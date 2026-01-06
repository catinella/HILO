#include "DragController.h"

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

DragController::DragController(QWidget *owner) : QObject(owner), m_owner(owner) {
	Q_ASSERT(m_owner);
	m_owner->installEventFilter(this);
}

bool DragController::eventFilter(QObject *obj, QEvent *ev) {
	bool out = false;

	if (m_enabled && obj == m_owner) {
		QMouseEvent *e = nullptr;
		QPoint gpos;

		switch (ev->type()) {
			case QEvent::MouseButtonPress: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (e->button() == Qt::LeftButton) {
					m_dragging         = true;
					m_pressGlobal      = gpos;
					m_startPosInParent = m_owner->pos();

					// Receiving mouse move/release events aoutside the widget's area
					m_owner->grabMouse();

					// Signal sending
					emit dragStarted(m_startPosInParent);
					
					out = true;
				}
				break;;
			}

			case QEvent::MouseMove: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (m_dragging) {
					const QPoint delta = gpos - m_pressGlobal;

					if (getControlMode() == HILOWIDGET_ACTIVE)
						m_owner->move(m_startPosInParent + delta);

					// Signal sending
					emit dragging(delta);
					
					out = true;
				}
				break;;
			}

			case QEvent::MouseButtonRelease: {
				e = static_cast<QMouseEvent*>(ev);
				gpos = e->globalPosition().toPoint();
				if (m_dragging && e->button() == Qt::LeftButton) {
					m_dragging = false;
					m_owner->releaseMouse();

					// Signal sending
					emit dragFinished();
					
					out = true;
				}
				break;;
			}

			default:
				break;
		}

		if (out == false) out = QObject::eventFilter(obj, ev);
	}

	return(out);
}



void DragController::setEnabled(bool en) {
	m_enabled = en;
}


bool DragController::isEnabled() const {
	return(m_enabled);
}

hiloControlMode_t DragController::getControlMode() const {
	return(m_controlMode);
}

void DragController:: setControlMode(hiloControlMode_t mode) {
	m_controlMode = mode;
}
