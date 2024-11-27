#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QHostAddress getMyIP();
private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;

    void initClnt();

private slots:
    void slot_connectButton();
    void slot_readMessage();
    void slot_disconnected();

};

#endif // WIDGET_H
