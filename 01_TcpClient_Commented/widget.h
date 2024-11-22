#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:

    // [02.1]
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    // [02.7]
    QHostAddress getMyIP();

private:
    Ui::Widget *ui;

    // [02.3]
    void initClnt();
    QTcpSocket *tcpSocket;

private slots:

    // [02.4]
    void slot_connectButton();

    // [02.5]
    void slot_readMessage(); // 서버로부터 메세지를 받을 때 호출 됨

    // [02.6]
    void slot_disconnected();
};

#endif // WIDGET_H
