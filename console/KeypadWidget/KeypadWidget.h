//------------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//
//
//
//
//------------------------------------------------------------------------------------------------------------------------------

#ifndef KEYPADWIDGET_H
#define KEYPADWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>

#define KEYPAD_COLS     3
#define KEYPAD_ROWS     4
#define KEYPAD_XBTNSIZE 32
#define KEYPAD_YBTNSIZE 32
#define KEYPAD_BTNSLABS {"1","2","3","4","5","6","7","8","9","CLR","0","DEL","OK"};

class QLineEdit;

namespace Ui {
	class KeypadWidget;
}

class KeypadWidget:public QWidget {
    Q_OBJECT

public:
	explicit KeypadWidget (QWidget * parent = nullptr);
	~KeypadWidget ();

	signals:
		void digitPressed (int digit);
		void textChanged  (const QString & text);
		void enterPressed (const QString & text);

	private slots:
		void onDigitClicked ();
		void onClearClicked ();
		void onBackspaceClicked ();
		void onOkClicked ();
private:
	// Defined by ui_KeypadWidget.h
	Ui::KeypadWidget *ui;

	QVector<QPushButton*> digitButtons;

	void appendDigit (const QString &d);
};

#endif // KEYPADWIDGET_H
