#pragma once

#include <QObject>
#include <QWidget>
#include <QPoint>

typedef enum {
	HILOWIDGET_PASSIVE,
	HILOWIDGET_ACTIVE
} hiloControlMode_t;

class DragController : public QObject {
	Q_OBJECT
public:
	explicit          DragController(QWidget *owner);

	void              setEnabled(bool en);
	bool              isEnabled() const;
	hiloControlMode_t getControlMode() const;
	void              setControlMode(hiloControlMode_t mode);

signals:
	void dragStarted (const QPoint &startPosInParent);
	void dragging    (const QPoint &deltaInParent);
	void dragFinished();

protected:
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
	QWidget           *m_owner = nullptr;

	bool              m_enabled  = true;
	bool              m_dragging = false;
	
	hiloControlMode_t m_controlMode = HILOWIDGET_ACTIVE;
	QPoint            m_pressGlobal;
	QPoint            m_startPosInParent;
};

