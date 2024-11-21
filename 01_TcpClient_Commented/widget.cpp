#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>

#include "widget.h"
#include "ui_widget.h"

// [02.1] Widget 생성자 실행
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // [02.3] Client Network 초기화
    initClnt();

    // [02.1.1]
    // UI connectButton 시그널과, slot_connectButton 슬롯 연결
    //// [2] 연결
    connect(ui->connectButton, SIGNAL(clicked()),
            this,                SLOT(slot_connectButton()));
}

// Widget class 파괴자
Widget::~Widget()
{
    delete ui;
}

// [02.3]
void Widget::initClnt()
{

    // [02.3.1] tcpCocket 클라이언트 소켓 생성
    //// [1] 생성 : 클라이언트 소켓 생성
    tcpSocket = new QTcpSocket(this);

    // [02.3.2] 서버에서 전송된 데이터가 있으면 slot_readMessage() 슬롯 연결
    //// [3] 송수신 : 데이터 수신
    connect(tcpSocket, SIGNAL(readyRead()),
            this,      SLOT(slot_readMessage()));

    // [02.3.3] 서버에서 연결 종료 시그널이 전달되면 slot_disconnected 슬롯 연결
    connect(tcpSocket, SIGNAL(disconnected()),
            this,      SLOT(slot_disconnected()));
}

// [02.4]
//// [2] 연결 : 서버 IP로 연결 요청, Port 는 qint16 자료형
void Widget::slot_connectButton()
{
    // [02.4.1] QLineEdit 에 입력된 Server IP, Server Port를 가져온다.
    QString serverIP = ui->serverIP->text().trimmed();
    QString serverPort = ui->serverPort->text().trimmed();
    QHostAddress serverAddress(serverIP);

    // [02.4.2] 서버로 연결 요청을 보낸다.
    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
    ui->textEdit->append("<- 서버에게 연결 요청");
}

// [02.5] 서버에서 전송 받은 데이터를 읽어들인다.
//// [3] 송수신 : 데이터 송수신
void Widget::slot_readMessage()
{
    // 바이트를 읽어들여 textEdit에 출력한다.
    if(tcpSocket->bytesAvailable() >= 0)
    {
        QByteArray readData = tcpSocket->readAll();
        ui->textEdit->append("-> "+readData);
    }
}

// [02.6] 소켓 연결 종료 정보를 표시한다.
void Widget::slot_disconnected()
{
    ui->textEdit->append("-> 서버로부터 연결 해제");
    qDebug() << Q_FUNC_INFO << "서버로부터 접속 종료.";
}

// [02.7]
QHostAddress Widget::getMyIP()
{

    QHostAddress myAddress;
    // 장치에 연결된 모든 ip를 가져와서
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용하기 위해 찾음
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            //qDebug() << ipAddressesList.at(i);
            myAddress = ipAddressesList.at(i);
            break;
        }
    }

    // 인터넷이 연결되어 있지 않다면, localhost(127.0.0.1) 사용
    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);

    return myAddress;
}


