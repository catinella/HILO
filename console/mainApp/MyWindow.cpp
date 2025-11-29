#include "MyWindow.h"
#include "KeypadWidget.h"

#include <QVBoxLayout>
#include <QLabel>

MyWindow::MyWindow(QWidget *parent)
    : QWidget(parent)
{
    m_keypad = new KeypadWidget(this);
    m_info   = new QLabel("Inserisci un numero e premi OK", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_keypad);
    layout->addWidget(m_info);

    setLayout(layout);

    connect(m_keypad, &KeypadWidget::enterPressed,
            this,     &MyWindow::onEnterPressed);

    connect(m_keypad, &KeypadWidget::textChanged,
            this,     &MyWindow::onTextChanged);
}

void MyWindow::onEnterPressed(const QString &value)
{
    m_info->setText(QString("Hai confermato: %1").arg(value));
}

void MyWindow::onTextChanged(const QString &value)
{
    m_info->setText(QString("Digitando: %1").arg(value));
}

