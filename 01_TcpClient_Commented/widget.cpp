#include <thread>
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <opencv2/opencv.hpp>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 네트워크 초기화
    initClnt();

    // UI 이벤트 연결
    connect(ui->connectButton, &QPushButton::clicked, this, &Widget::slot_connectButton);

    videoLabel = new QLabel(this);
    videoLabel->setGeometry(100, 100, 640, 480); // 위치와 크기 설정
    videoLabel->setStyleSheet("background-color: black;"); // 배경색 설정
    videoLabel->setScaledContents(true); // 이미지 크기에 맞게 스케일 조정
    videoLabel->show(); // QLabel 표시
}

Widget::~Widget()
{
    delete ui;

    if (sendThread.joinable()) {
        sendThread.join();
    }
    if (receiveThread.joinable()) {
        receiveThread.join();
    }
}

void Widget::initClnt()
{
    tcpSocket = new QTcpSocket(this);

    // 서버에서 전송된 데이터가 있을 때 처리
    // connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::slot_readMessage);

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

    if (tcpSocket->waitForConnected(3000)) {
        ui->textEdit->append("<- 서버 연결 성공");
        qDebug() << "[INFO] 서버 연결 성공";

        // 멀티스레드 시작
        sendThread = std::thread(&Widget::sendVideo, this);
        receiveThread = std::thread(&Widget::receiveVideo, this);
    } else {
        ui->textEdit->append("<- 서버 연결 실패");
        qDebug() << "[ERROR] 서버 연결 실패";
    }
}

void Widget::sendVideo()
{
    QString videoFilePath = "/home/veda/Downloads/01_TcpClient_Commented/gizmo.mp4"; // 동영상 경로
    cv::VideoCapture capture(videoFilePath.toStdString());
    if (!capture.isOpened()) {
        qDebug() << "[ERROR] 동영상을 열 수 없습니다:" << videoFilePath;
        return;
    }

    while (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        cv::Mat frame;
        if (!capture.read(frame)) {
            qDebug() << "[INFO] 동영상 전송 완료.";
            break;
        }

        // 프레임을 JPEG로 인코딩
        std::vector<uchar> buffer;
        cv::imencode(".jpg", frame, buffer);

        // 크기 전송
        qint64 frameSize = buffer.size();
        tcpSocket->write(reinterpret_cast<const char*>(&frameSize), sizeof(frameSize));
        tcpSocket->waitForBytesWritten();

        // 데이터 전송
        QByteArray frameData(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        tcpSocket->write(frameData);
        tcpSocket->waitForBytesWritten();

        qDebug() << "[INFO] 프레임 전송 완료: 크기 =" << frameSize;

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 약 30 FPS
    }

    capture.release();
}

void Widget::receiveVideo()
{
    static QByteArray accumulatedData;

    while (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        if (tcpSocket->bytesAvailable() > 0) {
            accumulatedData.append(tcpSocket->readAll());

            // 프레임 크기 및 데이터 처리
            while (accumulatedData.size() >= sizeof(qint64)) {
                qint64 frameSize = *reinterpret_cast<const qint64*>(accumulatedData.constData());
                if (accumulatedData.size() >= sizeof(qint64) + frameSize) {
                    QByteArray frameData = accumulatedData.mid(sizeof(qint64), frameSize);
                    accumulatedData = accumulatedData.mid(sizeof(qint64) + frameSize);

                    // OpenCV로 디코딩
                    std::vector<uchar> buffer(frameData.begin(), frameData.end());
                    cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

                    if (!frame.empty()) {
                        qDebug() << "[INFO] 프레임 수신 및 디코딩 성공";
                        displayFrame(frame); // QLabel에 표시
                    }
                } else {
                    break; // 데이터가 부족한 경우 대기
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // CPU 점유율 조정
    }
}

void Widget::slot_disconnected()
{
    ui->textEdit->append("-> 서버로부터 연결 해제");
    qDebug() << "[INFO] 서버와 연결 종료.";
}

void Widget::displayFrame(const cv::Mat &frame)
{
    if (!videoLabel->isVisible()) {
        videoLabel->show(); // 첫 프레임이 들어오면 QLabel을 표시
    }

    // OpenCV Mat -> QImage 변환
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    QImage image(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    // QLabel에 이미지 설정
    videoLabel->setPixmap(QPixmap::fromImage(image));
}
