#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <thread>

    namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    Ui::Widget *ui;
    QTcpSocket *tcpSocket;             // 서버와의 통신 소켓
    QLabel *videoLabel;                // QLabel for displaying video frames
    bool isVideoLabelInitialized = false; // QLabel 초기화 여부 체크

    std::thread sendThread;            // 송신 스레드
    std::thread receiveThread;         // 수신 스레드


    void initClnt();                // 클라이언트 초기화
    QHostAddress getMyIP();            // 로컬 IP 주소 가져오기
    void displayFrame(const cv::Mat &frame); // QLabel에 프레임 표시
    QImage MatToQImage(const cv::Mat &mat);  // cv::Mat -> QImage 변환

    void sendVideo();                  // 영상을 서버로 송신
    void receiveVideo();               // 서버에서 영상 수신


    void slot_connectButton();         // 연결 버튼 클릭 처리
    void slot_disconnected();          // 서버 연결 종료 처리
};

#endif // WIDGET_H
