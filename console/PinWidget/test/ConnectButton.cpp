#include "ConnectButton.h"

void ConnectButton::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton)
		emit rightClicked();
/*
	else if (event->button() == Qt::LeftButton)
		QPushButton::mousePressEvent(event);
*/
	else
		QPushButton::mousePressEvent(event);

	return;
}
