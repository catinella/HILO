#pragma once

#include <QString>
#include <QPointer>
#include "PinWidget.h"

class PinConnection {
	QString             aId;
	QString             bId;
	QPointer<PinWidget> aW;
	QPointer<PinWidget> bW;

public:
	// Constructors
	PinConnection   ();
	PinConnection   (QString a, PinWidget *aw);
	PinConnection   (QString a, QString b, PinWidget *aw, PinWidget *bw);

	bool operator== (const PinConnection &o) const noexcept;

	bool addItem    (const QString &x, PinWidget *xw);
	void getItems   (QString &xID, QString &yID, PinWidget *&xWidget, PinWidget *&yWidget) const;

	bool isValid    () const noexcept;

	bool involves   (const QString &pinId) const;
	bool involves   (const PinWidget *w) const;

private:
	void normalize ();
};
