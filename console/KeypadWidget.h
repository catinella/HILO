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

#define KEYPAD_COLS     3
#define KEYPAD_ROWS     4
#define KEYPAD_XBTNSIZE 32
#define KEYPAD_YBTNSIZE 32
#define KEYPAD_BTNSLABS {"1","2","3","4","5","6","7","8","9","CLR","0","DEL","OK"};


class QLineEdit;
class QPushButton;

class KeypadWidget : public QWidget {
	
	Q_OBJECT public:
	
	explicit KeypadWidget (QWidget * parent = nullptr);

	QString text () const;

public slots:
	void setText (const QString & t);
	void clear ();
	void backspace ();

signals: 
	void textChanged (const QString & text);
	void enterPressed (const QString & text);	// quando premi OK

private slots:void onDigitClicked ();
	void onClearClicked ();
	void onBackspaceClicked ();
	void onOkClicked ();

private:
	QLineEdit * m_display;

	void appendDigit (const QString & d);
};

#endif // KEYPADWIDGET_H
