#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>

class PinWidget: public QWidget {
	Q_OBJECT 
public:
	explicit PinWidget (QWidget * parent = nullptr);

	bool getValue () const;
	void setValue (bool v);

signals:
	void valueChanged (bool value);
	void leftClicked();
	void rightClicked();

protected:
	void  paintEvent      (QPaintEvent * event) override;
	void  mousePressEvent (QMouseEvent * event) override;

private:
	bool m_value = false;
};

#endif // PINWIDGET_H
