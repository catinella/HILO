#ifndef __CONNECTBUTTON__
#define __CONNECTBUTTON__

#include <QMouseEvent>
#include <QPushButton>

//
// Class SpecialButton
//	This special button manages also the right-click events used to connect the button to a pin
//
class ConnectButton : public QPushButton {
	Q_OBJECT

public:
	using QPushButton::QPushButton;

signals:
	void rightClicked();

protected:
	void mousePressEvent(QMouseEvent *event) override;
};

#endif
