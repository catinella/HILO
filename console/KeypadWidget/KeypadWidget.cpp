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
#include "KeypadWidget.h"

#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>

KeypadWidget::KeypadWidget (QWidget *parent):QWidget (parent) {
	QVector < QPushButton * >keypad;

	m_display = new QLineEdit(this);
	m_display->setReadOnly(true);
	m_display->setAlignment(Qt::AlignRight);
	m_display->setMaxLength(3);
	m_display->setFixedSize(120, 30);
	{
		QFont dispFont = m_display->font ();
		dispFont.setPointSize (20);
		m_display->setFont (dispFont);
	}

	//
	// Buttons creation...
	{
		QStringList buttsLabels = KEYPAD_BTNSLABS
		keypad.resize (buttsLabels.length ());

		for (int t = 0; t < buttsLabels.length (); t++) {
			keypad[t] = new QPushButton (buttsLabels[t], this);
			keypad[t]->setFixedSize(KEYPAD_XBTNSIZE, KEYPAD_YBTNSIZE);
			QFont f = keypad[t]->font ();
			f.setPointSize (9);
			keypad[t]->setFont (f);
		}
	}

	{
		QGridLayout *grid = nullptr;
		QVBoxLayout *mainLayout = nullptr;

		// Layout a griglia 4x3
		grid = new QGridLayout ();
		for (int y = 0; y < KEYPAD_ROWS; y++) {
			for (int x = 0; x < KEYPAD_COLS; x++) {
				grid->addWidget (keypad[(y * KEYPAD_COLS) + x], y, x);
			}
		}
		grid->setHorizontalSpacing(2);
		grid->setVerticalSpacing(2);


		// Layout principale verticale
		mainLayout = new QVBoxLayout (this);
		mainLayout->addWidget(m_display);
		mainLayout->addLayout(grid);
		mainLayout->addWidget(keypad[12]);
		mainLayout->setContentsMargins(2,2,2,2);
		grid->setHorizontalSpacing(0);
		grid->setVerticalSpacing(0);

		setLayout (mainLayout);
	}

	for (int t = 0; t < (keypad.length () - 1); t++) {
		if (t < 9)
			connect (keypad[t], &QPushButton::clicked, this, &KeypadWidget::onDigitClicked);
		else if (t == 9)
			connect (keypad[t], &QPushButton::clicked, this, &KeypadWidget::onClearClicked);
		else if (t == 10)
			connect (keypad[t], &QPushButton::clicked, this, &KeypadWidget::onDigitClicked);
		else if (t == 11)
			connect (keypad[t], &QPushButton::clicked, this, &KeypadWidget::backspace);
		else if (t == 12)
			connect (keypad[t], &QPushButton::clicked, this, &KeypadWidget::onOkClicked);
	}

	return;
}

QString KeypadWidget::text () const {
	return m_display->text ();
}

void KeypadWidget::setText (const QString & t) {
	if (m_display->text () != t) {
		m_display->setText (t);
		emit textChanged (m_display->text ());
	}
	return;
}

void KeypadWidget::clear () {
	setText (QString ());
}

void KeypadWidget::backspace () {
	QString t = m_display->text ();
	if (!t.isEmpty ()) {
		t.chop (1);		  // elimina ultimo carattere
		setText (t);
	}
}

void KeypadWidget::appendDigit (const QString & d) {
	setText (m_display->text () + d);
}

void KeypadWidget::onDigitClicked () {
	if (auto * btn = qobject_cast < QPushButton * >(sender ())) {
		appendDigit (btn->text ());
	}
}

void KeypadWidget::onClearClicked () {
	clear ();
}

void KeypadWidget::onBackspaceClicked () {
	backspace ();
}

void KeypadWidget::onOkClicked () {
	emit enterPressed (m_display->text ());
}
