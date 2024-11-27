#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <opencv4/opencv2/opencv.hpp>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 네트워크 초기화
    initClnt();

    // UI 이벤트 연결
    connect(ui->connectButton, &QPushButton::clicked, this, &Widget::slot_connectButton);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initClnt()
{
    tcpSocket = new QTcpSocket(this);

    // 서버에서 전송된 데이터가 있을 때 처리
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::slot_readMessage);

    // 서버와의 연결 종료 시 처리
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Widget::slot_disconnected);
}

void Widget::slot_connectButton()
{
    QString serverIP = ui->serverIP->text().trimmed();
    QString serverPort = ui->serverPort->text().trimmed();
    QHostAddress serverAddress(serverIP);

    // 서버와 연결 요청
    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
    ui->textEdit->append("<- 서버에게 연결 요청");

    connect(tcpSocket, &QTcpSocket::connected, this, [=]() {
        QString videoFilePath = "/home/veda/Downloads/01_TcpClient_Commented/gizmo.mp4"; // 전송할 동영상 경로

        // OpenCV로 동영상 파일 열기
        cv::VideoCapture capture(videoFilePath.toStdString());
        if (!capture.isOpened()) {
            ui->textEdit->append("동영상을 열 수 없습니다: " + videoFilePath);
            return;
        }

        // QTimer를 이용한 비디오 프레임 전송
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            cv::Mat frame;
            if (!capture.read(frame)) {
                // 비디오가 끝나면 타이머 정지
                timer->stop();
                capture.release();
                ui->textEdit->append("-> 모든 프레임 전송 완료");
                tcpSocket->disconnectFromHost();
                return;
            }

            // 프레임을 JPEG로 인코딩
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);

            // 프레임 크기 전송
            qint64 frameSize = buffer.size();
            tcpSocket->write(reinterpret_cast<const char*>(&frameSize), sizeof(frameSize));
            tcpSocket->waitForBytesWritten();

            // 프레임 데이터 전송
            QByteArray frameData(reinterpret_cast<const char*>(buffer.data()), buffer.size());
            tcpSocket->write(frameData);
            tcpSocket->waitForBytesWritten();

            ui->textEdit->append("-> 프레임 전송 완료: 크기 = " + QString::number(frameSize));
        });

        timer->start(33); // 약 30 FPS로 전송
    });
}

void Widget::slot_readMessage()
{
    static QByteArray accumulatedData; // 수신된 데이터를 누적

    while (tcpSocket->bytesAvailable() > 0) {
        accumulatedData.append(tcpSocket->readAll());
    }

    // 합성된 프레임 크기 확인
    if (accumulatedData.size() >= sizeof(qint64)) {
        qint64 frameSize = *reinterpret_cast<const qint64*>(accumulatedData.constData());

        // 프레임 데이터를 모두 수신했는지 확인
        if (accumulatedData.size() >= sizeof(qint64) + frameSize) {
            // 프레임 데이터 추출
            QByteArray frameData = accumulatedData.mid(sizeof(qint64), frameSize);
            accumulatedData = accumulatedData.mid(sizeof(qint64) + frameSize);

            // OpenCV로 프레임 디코딩 및 표시
            std::vector<uchar> buffer(frameData.begin(), frameData.end());
            cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

        }
    }
}

void Widget::slot_disconnected()
{
    ui->textEdit->append("-> 서버로부터 연결 해제");
    qDebug() << Q_FUNC_INFO << "서버와 연결 종료.";
}

QHostAddress Widget::getMyIP()
{
    QHostAddress myAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // IPv4 주소 찾기
    for (const QHostAddress& address : ipAddressesList) {
        if (address != QHostAddress::LocalHost && address.toIPv4Address()) {
            myAddress = address;
            break;
        }
    }

    // 인터넷 연결이 없으면 localhost 사용
    if (myAddress.toString().isEmpty()) {
        myAddress = QHostAddress::LocalHost;
    }

    return myAddress;
}
