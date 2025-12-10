#ifndef __CONNECTIONOVERLAY__
#define __CONNECTIONOVERLAY__

#include <QWidget>
#include <QPainter>

class ConnectionOverlay : public QWidget
{
	Q_OBJECT
public:
	explicit ConnectionOverlay(QWidget *parent);
	void     setEndpoints(QWidget *from, QWidget *to);
	void     clear();

protected:
	void     paintEvent(QPaintEvent *event) override; 

private:
	QWidget *m_from = nullptr;
	QWidget *m_to   = nullptr;
};

#endif
