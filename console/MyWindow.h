#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>

class KeypadWidget;
class QLabel;

class MyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = nullptr);

private slots:
    void onEnterPressed(const QString &value);
    void onTextChanged(const QString &value);

private:
    KeypadWidget *m_keypad;
    QLabel *m_info;
};

#endif // MYWINDOW_H

