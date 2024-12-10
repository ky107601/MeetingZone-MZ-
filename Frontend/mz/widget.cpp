#include "widget.h"
#include "./ui_widget.h"

#include <QGraphicsDropShadowEffect> //for shadow effect
#include <QMessageBox>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsView>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    //this->showMaximized(); //full-size

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    // startStreaming();
    setUI();

<<<<<<< HEAD
    // string pipeline = setPipeline();
    // openCamera(pipeline);

=======
   // startStreaming();
=======
    //startStreaming();
>>>>>>> 232da33 (tmp)
    setUI();
    
    // string pipeline = setPipeline();
    // openCamera(pipeline);

>>>>>>> 05136ac (clframe write & read)
    //setTimerForFrame();
    setToggleVideo();
    setBackground();
}

Widget::~Widget()
{
    // Stop MediaMTX server before exiting
    NetworkManager::getInstance().stopMediaMTX();
    delete ui;
}

/* slots */
void Widget::connectServer()
{
    // 연결 확인
    QString serverIP = ui->inputServerAd->text();
    QString serverPort = "8888";
    QHostAddress serverAddress(serverIP);

    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());

    connect(tcpSocket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "서버 연결됨";
<<<<<<< HEAD
        qDebug() << "ui->inputServerAd = " <<ui->inputServerAd->text().toStdString();
        networkManager.set_ip_addr(ui->inputServerAd->text().toStdString());
        qDebug() <<"get ip = " << networkManager.get_ip_addr();
=======
>>>>>>> 05136ac (clframe write & read)
        ui->stackedWidget->setCurrentIndex(1);
        ui->inputServerAd->clear();
        ui->inputNickname->clear();

        QtConcurrent::run([this]()
        {
            getVideo(); //카메라 on/off와 상관없이 항상 받아옴
        });
    });

    connect(tcpSocket, &QTcpSocket::errorOccurred, this, [this]() {
        QMessageBox::warning(this, "실패", "서버 연결에 실패하였습니다.", QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(0);
    });
}

<<<<<<< HEAD
void Widget::changeIcon()
{
    if(!micFlag) //status : mic off -> on
        ui->micBtn->setIcon(micOnIcon);
    else        //status : mic on -> off
        ui->micBtn->setIcon(micOffIcon);
    micFlag = !micFlag;
    ui->micBtn->setIconSize(QSize(40, 40));
}

void Widget::updateFrame()
{
    // // GrabCut 상태 유지
    // static Mat prevMask, bgModel, fgModel;
    // static bool isInitialized = false;
    // static int frameCounter = 0;
    // static Mat lastBinaryMask, prevFrame;

    // Mat frame = captureNewFrame();
    // // 관심 영역 (ROI) 동적 설정
    // Rect roi(10, 10, frame.cols-20, frame.rows-20);
    // frameROI = frame(roi);
    // frame.copyTo(prevFrame);
    // // GrabCut 초기화
    // if (!isInitialized) {
    //     prevMask = Mat(frameROI.size(), CV_8UC1, Scalar(GC_BGD));
    //     Rect initRect(10, 10, frameROI.cols - 20, frameROI.rows - 20);
    //     prevMask(initRect).setTo(Scalar(GC_PR_FGD));
    //     grabCut(frameROI, prevMask, initRect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    //     lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
    //     isInitialized = true;
    // }

    // // 프레임 샘플링 (10프레임마다 GrabCut 실행)
    // if (frameCounter % 10 == 0) {
    //     grabCut(frameROI, prevMask, Rect(), bgModel, fgModel, 1, GC_INIT_WITH_MASK);
    //     lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
    // }

    // // ROI 기반 이진 마스크 축소/확대
    // Mat smallMask, binaryMask;
    // cv::resize(lastBinaryMask, smallMask, Size(), 0.5, 0.5, INTER_NEAREST);
    // cv::resize(smallMask, binaryMask, frame.size(), 0, 0, INTER_NEAREST);

    // // RGBA 이미지 생성
    // Mat transparentImg(frame.size(), CV_8UC4, Scalar(0, 0, 0, 0));
    // vector<Mat> bgrChannels;
    // split(frame, bgrChannels);

    // Mat alphaChannel;
    // binaryMask.convertTo(alphaChannel, CV_8UC1, 255.0);
    // bgrChannels.push_back(alphaChannel);
    // merge(bgrChannels, transparentImg);

    // // Convert to QImage with transparency
    // QImage qimg(transparentImg.data, transparentImg.cols, transparentImg.rows, transparentImg.step, QImage::Format_RGBA8888);

    // //라벨에 표시
    // cam->setFixedSize(transparentImg.cols / 5, transparentImg.rows / 5);
    // cam->setPixmap(QPixmap::fromImage(qimg).scaled(cam->size(), Qt::KeepAspectRatio));

    // // 프레임 카운터 증가
    // frameCounter++;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    ui->chatInput->clearFocus();
    qDebug("clear Focus");
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    //left : 16777234, down : 16777235, right : 16777236, up : 16777237
    if(!ui->chatInput->hasFocus()) //ui->chatInpt doesn't have focus(not entering something)
    {
        cam->moveByKey(event->key());
    }
    else //enter : 16777220
    {
        qDebug()<<"key = " <<event->key();
        if(event->key() == 16777220 || event->key() == 16777221)
        {
            // 여기서 채팅 보내야 함
            QString message = ui->chatInput->text();
            QByteArray messageData = message.toUtf8();
            int32_t messageSize = messageData.size();

            // 메시지 크기 먼저 전송 후 메시지 전송
            // tcpSocket->write(reinterpret_cast<const char*>(&messageSize), sizeof(messageSize));
            // tcpSocket->write(messageData);
            // tcpSocket->flush();

            qDebug() << "Message sent to server. Size:" << messageSize << ", Content:" << message;

            chatBox->setText(message);
            ui->chatInput->clear();
        }
    }
}

void Widget::selectPicture()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open Image File", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (filePath.isEmpty()) {
        qDebug() << "No file selected.";
        return;
    }

    ui->background->setPixmap(QPixmap(filePath));
    picture.addPicture(filePath.toStdString());
}


/* 리팩토링용 함수들 */

void Widget::startStreaming() {
    /* ========== NetworkManager ========== */

    //rtsp_url set_ip_addr로 설정

    qDebug() <<"ui->inpustServerAd->text().toStdString()" << ui->inputServerAd->text().toStdString();
    const std::string rtsp_url = "rtsp://" + networkManager.get_ip_addr() + ":8554/camera";
    qDebug() <<"rtsp_url = " <<rtsp_url;

=======

    /* ========== NetworkManager ========== */
<<<<<<< HEAD
    networkManager = NetworkManager::getInstance();
>>>>>>> ba0bac2 (add NM in widget)
=======
    NetworkManager& networkManager = NetworkManager::getInstance();
>>>>>>> 61c7ffa (widget update)
    // Register signal handler to clean up resources
    std::signal(SIGINT, [](int) {
        NetworkManager::getInstance().stopMediaMTX();
        exit(EXIT_SUCCESS);
    });

    // Start MediaMTX server
    networkManager.startMediaMTX();

    // Start RTSP streaming in a separate thread
<<<<<<< HEAD
    std::thread streaming_thread(&NetworkManager::startRTSP, &networkManager, rtsp_url);
=======
    std::thread streaming_thread(&NetworkManager::rtsp_streaming, &networkManager, rtsp_url);
>>>>>>> ba0bac2 (add NM in widget)

    // Wait for the streaming thread to complete
    streaming_thread.join();
    /* ==================================== */
<<<<<<< HEAD
    return;
}

void Widget::setUI() {
=======



>>>>>>> ba0bac2 (add NM in widget)
    /* Set the inputBox style */
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(5);         // Set blur radius for the shadow
    shadowEffect->setOffset(5, 5);           // Set shadow offset (x, y)
    shadowEffect->setColor(Qt::gray);       // Set shadow color
    ui->inputBox->setGraphicsEffect(shadowEffect);

    QWidget *page2 = new QWidget;
    ui->stackedWidget->addWidget(page2);

    connect(ui->enterBtn, SIGNAL(clicked()), this, SLOT(connectServer()));

    ui->changeBackBtn->setIcon(QIcon(":/resources/change.png"));
    ui->addBackBtn->setIcon(QIcon(":/resources/plus.png"));

    //통합된 비디오 출력 창
    videoWindow = new QLabel("", ui->backWidget);
    videoWindow->setGeometry(QRect(QPoint(ui->background->pos().x() + 10, ui->background->pos().y() + 10), QSize(780, 440))); // Offset 조금 + 크기 설정
    videoWindow->setStyleSheet("background-color: rgb(0, 0, 0, 0);");

    cam = new camViewer(ui->backWidget);
    cam->raise();
    cam->hide();

    ui->chatInput->setPlaceholderText("Enter your text here ...");
    chatBox = new QLabel("", ui->backWidget);
    chatBox->setGeometry(QRect(50,400,50,50));
    chatBox->setStyleSheet("background-color : rgb(0, 0, 0, 0);");
    chatBox->raise();

    micOffIcon = QIcon(":/resources/mic_off.png");
    micOnIcon = QIcon(":/resources/mic_on.png");
    ui->micBtn->setIcon(micOffIcon);
    ui->micBtn->setIconSize(QSize(40,40));

    videoOffIcon = QIcon(":/resources/video_off.png");
    videoOnIcon = QIcon(":/resources/video_on.png");
    ui->videoBtn->setIcon(videoOffIcon);
    ui->videoBtn->setIconSize(QSize(40,40));

    QIcon exitIcon(":/resources/exit.png");
    ui->exitBtn->setIcon(exitIcon);
    ui->exitBtn->setIconSize(QSize(40,40));

    connect(ui->exitBtn, &QPushButton::clicked, this, [&](){
        QMessageBox::StandardButton response;
        response = QMessageBox::question(this, "Disconnect", "Are you sure you want to end the call?!?!", QMessageBox::Yes | QMessageBox::No);

        if(response == QMessageBox::Yes) //End the call
        {
            tcpSocket->disconnectFromHost();
            ui->stackedWidget->setCurrentIndex(0);
        }
    });

    connect(ui->micBtn, SIGNAL(clicked()), this, SLOT(changeIcon()));
    return;
}

string& Widget::setPipeline() {
    width="640";
    height="480";
    string pipeline =
        "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
        "! video/x-raw,width="+width+",height="+height+",framerate=10/1,format=RGBx "
                                        "! videoconvert ! videoscale ! appsink";
    return pipeline;
}

void Widget::openCamera(string& pipeline) {
    if(!cap.open(pipeline, CAP_GSTREAMER)) {
        qDebug() << "Failed to open the camera!";
    }
}

//Set the timer to capture the frame
void Widget::setTimerForFrame() {
    captureTimer = new QTimer(this);
    connect(captureTimer, &QTimer::timeout, this, &Widget::updateFrame);
    return;
}

//ON/OFF the video
void Widget::setToggleVideo() {
    connect(ui->videoBtn, &QPushButton::clicked, this, [=](){
        if(!videoFlag)  //status : video off -> on
        {
            ui->videoBtn->setIcon(videoOnIcon);
            cam->setStyleSheet("background-color : white;");
            cam->show();
            //captureTimer->start(100);
            // width="640";
            // height="480";
            // string pipeline =
            //     "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
            //     "! video/x-raw,width="+width+",height="+height+",framerate=10/1,format=RGBx "
            //                                     "! videoconvert ! videoscale ! appsink";
            // if(!cap.open(pipeline, CAP_GSTREAMER)) {
            //     qDebug()<<"Failed to open the camera!";
            // }

            //sendVideo();
            startStreaming();

        }
        else            //status : video on -> off
        {
            ui->videoBtn->setIcon(videoOffIcon);
            //captureTimer->stop();
            cam->hide();
            //카메라 화면 투명화해서 전송

        }
        videoFlag = !videoFlag;
        ui->videoBtn->setIconSize(QSize(40, 40));
    });
    cam->setStyleSheet("background-color : rgb(0,0,0,0);");
    return;
}

void Widget::setBackground() {
    // change background picture
    connect(ui->changeBackBtn, &QPushButton::clicked, this, [this](){
        QString nextPath = QString::fromStdString(picture.getNextPicture());
        qDebug() << "nextPath = " << nextPath;
        ui->background->setPixmap(QPixmap(nextPath));
    });
    connect(ui->addBackBtn, SIGNAL(clicked()), this, SLOT(selectPicture()));
    return;
=======
    startStreaming();
    setUI();
    
    string pipeline = setPipeline();
    openCamera(pipeline);

    setTimerForFrame();
    setToggleVideo();
    setBackground();
>>>>>>> 5ec23fc (widget 리팩토링)
}

<<<<<<< HEAD
// 새로운 프레임 캡처
Mat& Widget::captureNewFrame() {
    Mat frame;
    // cap.read(frame);
    // if (frame.empty()) {
    //     qDebug() << "Unable to grab frame!";
    // }
    // networkManager::
    return frame;
=======
Widget::~Widget()
{
    // Stop MediaMTX server before exiting
    NetworkManager::getInstance().stopMediaMTX();
    delete ui;
>>>>>>> ba0bac2 (add NM in widget)
}

<<<<<<< HEAD
<<<<<<< HEAD
// 통합 비디오 수신 및 재생
void Widget::getVideo() {
    qDebug() << tcpSocket->state();
    while (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        // 프레임 크기 읽기
        qDebug() << "영상 수신 시도";
        int64_t frameSize = 0;
        if (tcpSocket->bytesAvailable() < sizeof(frameSize)) {
            if (!tcpSocket->waitForReadyRead()) { // Wait for the frame size
                cerr << "[ERROR] Waiting for frame size failed" << endl;
                //return;
            }
        }
        tcpSocket->read(reinterpret_cast<char*>(&frameSize), sizeof(frameSize));

        if (frameSize <= 0 || frameSize > 10 * 1024 * 1024) { // Sanity check
            cerr << "[ERROR] Invalid frame size received: " << frameSize << endl;
            return;
        }

        // 프레임 읽기
        QByteArray frameData;
        while (frameData.size() < frameSize) {
            if (!tcpSocket->waitForReadyRead()) { // Wait for more data
                cerr << "[ERROR] Frame data reception timed out" << endl;
                return;
            }
            frameData.append(tcpSocket->read(frameSize - frameData.size()));
        }
=======
=======
/* slots */
>>>>>>> 5ec23fc (widget 리팩토링)
=======
>>>>>>> 05136ac (clframe write & read)
void Widget::changeIcon()
{
    if(!micFlag) //status : mic off -> on
        ui->micBtn->setIcon(micOnIcon);
    else        //status : mic on -> off
        ui->micBtn->setIcon(micOffIcon);
    micFlag = !micFlag;
    ui->micBtn->setIconSize(QSize(40, 40));
}

void Widget::updateFrame()
{
    // // GrabCut 상태 유지
    // static Mat prevMask, bgModel, fgModel;
    // static bool isInitialized = false;
    // static int frameCounter = 0;
    // static Mat lastBinaryMask, prevFrame;
    
    // Mat frame = captureNewFrame();
    // // 관심 영역 (ROI) 동적 설정
    // Rect roi(10, 10, frame.cols-20, frame.rows-20);
    // frameROI = frame(roi);
    // frame.copyTo(prevFrame);
    // // GrabCut 초기화
    // if (!isInitialized) {
    //     prevMask = Mat(frameROI.size(), CV_8UC1, Scalar(GC_BGD));
    //     Rect initRect(10, 10, frameROI.cols - 20, frameROI.rows - 20);
    //     prevMask(initRect).setTo(Scalar(GC_PR_FGD));
    //     grabCut(frameROI, prevMask, initRect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    //     lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
    //     isInitialized = true;
    // }
    
    // // 프레임 샘플링 (10프레임마다 GrabCut 실행)
    // if (frameCounter % 10 == 0) {
    //     grabCut(frameROI, prevMask, Rect(), bgModel, fgModel, 1, GC_INIT_WITH_MASK);
    //     lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
    // }

    // // ROI 기반 이진 마스크 축소/확대
    // Mat smallMask, binaryMask;
    // cv::resize(lastBinaryMask, smallMask, Size(), 0.5, 0.5, INTER_NEAREST);
    // cv::resize(smallMask, binaryMask, frame.size(), 0, 0, INTER_NEAREST);

    // // RGBA 이미지 생성
    // Mat transparentImg(frame.size(), CV_8UC4, Scalar(0, 0, 0, 0));
    // vector<Mat> bgrChannels;
    // split(frame, bgrChannels);

    // Mat alphaChannel;
    // binaryMask.convertTo(alphaChannel, CV_8UC1, 255.0);
    // bgrChannels.push_back(alphaChannel);
    // merge(bgrChannels, transparentImg);
    
    // // Convert to QImage with transparency
    // QImage qimg(transparentImg.data, transparentImg.cols, transparentImg.rows, transparentImg.step, QImage::Format_RGBA8888);

    // //라벨에 표시
    // cam->setFixedSize(transparentImg.cols / 5, transparentImg.rows / 5);
    // cam->setPixmap(QPixmap::fromImage(qimg).scaled(cam->size(), Qt::KeepAspectRatio));

    // // 프레임 카운터 증가
    // frameCounter++;
}
>>>>>>> c45efa9 (Update Frame)

        // 프레임 디코드
        vector<uchar> buffer(frameData.begin(), frameData.end());
        recFrame = imdecode(buffer, IMREAD_COLOR);

        // OpenCV 처리해야하는 부분

        if (recFrame.empty()) {
            cerr << "[ERROR] Failed to decode the frame" << endl;
            continue;
        }

        // Mat -> QImage
        QImage img((const uchar*)recFrame.data, recFrame.cols, recFrame.rows, recFrame.step, QImage::Format_RGB888);
        img = img.rgbSwapped(); // Convert BGR to RGB

        //videoWindow->setFixedSize(img.width(), img.height());
        videoWindow->setPixmap(QPixmap::fromImage(img).scaled(videoWindow->size(), Qt::KeepAspectRatio));

        // 선택: Introduce a small delay for smoother playback
        QCoreApplication::processEvents();
    }
    else //enter : 16777220
    {
        qDebug() << "key = " << event->key();
        if(event->key() == 16777220 || event->key() == 16777221)
        {
            // 여기서 채팅 보내야 함
            QString message = ui->chatInput->text();
            QByteArray messageData = message.toUtf8();
            int32_t messageSize = messageData.size();

            // 메시지 크기 먼저 전송 후 메시지 전송
            // tcpSocket->write(reinterpret_cast<const char*>(&messageSize), sizeof(messageSize));
            // tcpSocket->write(messageData);
            // tcpSocket->flush();

            qDebug() << "Message sent to server. Size:" << messageSize << ", Content:" << message;

            chatBox->setText(message);
            ui->chatInput->clear();
        }
    }
}

void Widget::sendVideo()
{
    qDebug() << tcpSocket->state();
    /*
    while (tcpSocket->state() == QAbstractSocket::ConnectedState) {

        Mat frame;
        vector<uchar> buffer;
        qDebug() << "영상 송신 시도";
        while(cap.read(frame))
        {
            if(frame.empty())
            {
                qDebug() <<"Unable to grab frame!!";
                continue;
            }
            // Encode frame as JPEG
            imencode(".jpg", frame, buffer);

            // Send size of the buffer first
            qint64 bufferSize = buffer.size();
            tcpSocket->write(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));

            // Send the actual buffer data
            tcpSocket->write(reinterpret_cast<const char*>(buffer.data()), bufferSize);

            if (!tcpSocket->waitForBytesWritten(3000)) {
                qDebug() << "Error sending frame!";
            }

            QThread::msleep(30); // To maintain the frame rate
        }

        // 선택: Introduce a small delay for smoother playback
        QCoreApplication::processEvents();
        cap.release();
    }

    */
    QString videoFilePath = "/home/pi/MeetingZone-MZ-/01_TcpClient_Commented/test1.mp4"; // 동영상 경로
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


/* 리팩토링용 함수들 */

void Widget::startStreaming() {
    /* ========== NetworkManager ========== */

    //rtsp_url set_ip_addr로 설정
    networkManager.set_ip_addr(ui->inputServerAd->text().toStdString());
    const std::string rtsp_url = "rtsps://" + networkManager.get_ip_addr() + ":8322/camera";

    // Register signal handler to clean up resources
    std::signal(SIGINT, [](int) {
        NetworkManager::getInstance().stopMediaMTX();
        exit(EXIT_SUCCESS);
    });

    // Start MediaMTX server
    networkManager.startMediaMTX();

    // Start RTSP streaming in a separate thread
    std::thread streaming_thread(&NetworkManager::rtsp_streaming, &networkManager, rtsp_url);

    // Wait for the streaming thread to complete
    streaming_thread.join();
    /* ==================================== */
    return;
}

void Widget::setUI() {
    /* Set the inputBox style */
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(5);         // Set blur radius for the shadow
    shadowEffect->setOffset(5, 5);           // Set shadow offset (x, y)
    shadowEffect->setColor(Qt::gray);       // Set shadow color
    ui->inputBox->setGraphicsEffect(shadowEffect);

    QWidget *page2 = new QWidget;
    ui->stackedWidget->addWidget(page2);

    connect(ui->enterBtn, SIGNAL(clicked()), this, SLOT(connectServer()));

    ui->changeBackBtn->setIcon(QIcon(":/resources/change.png"));
    ui->addBackBtn->setIcon(QIcon(":/resources/plus.png"));

    //통합된 비디오 출력 창
    videoWindow = new QLabel("", ui->backWidget);
    videoWindow->setGeometry(QRect(QPoint(ui->background->pos().x() + 10, ui->background->pos().y() + 10), QSize(780, 440))); // Offset 조금 + 크기 설정
    videoWindow->setStyleSheet("background-color: rgb(0, 0, 0, 0);");

    cam = new camViewer(ui->backWidget);
    cam->raise();
    cam->hide();

    ui->chatInput->setPlaceholderText("Enter your text here ...");
    chatBox = new QLabel("", ui->backWidget);
    chatBox->setGeometry(QRect(50,400,50,50));
    chatBox->setStyleSheet("background-color : rgb(0, 0, 0, 0);");
    chatBox->raise();

    micOffIcon = QIcon(":/resources/mic_off.png");
    micOnIcon = QIcon(":/resources/mic_on.png");
    ui->micBtn->setIcon(micOffIcon);
    ui->micBtn->setIconSize(QSize(40,40));

    videoOffIcon = QIcon(":/resources/video_off.png");
    videoOnIcon = QIcon(":/resources/video_on.png");
    ui->videoBtn->setIcon(videoOffIcon);
    ui->videoBtn->setIconSize(QSize(40,40));

    QIcon exitIcon(":/resources/exit.png");
    ui->exitBtn->setIcon(exitIcon);
    ui->exitBtn->setIconSize(QSize(40,40));

    connect(ui->exitBtn, &QPushButton::clicked, this, [&](){
        QMessageBox::StandardButton response;
        response = QMessageBox::question(this, "Disconnect", "Are you sure you want to end the call?!?!", QMessageBox::Yes | QMessageBox::No);

        if(response == QMessageBox::Yes) //End the call
        {
            tcpSocket->disconnectFromHost();
            ui->stackedWidget->setCurrentIndex(0);
        }
    });

    connect(ui->micBtn, SIGNAL(clicked()), this, SLOT(changeIcon()));
    return;
}

string& Widget::setPipeline() {
    width="640";
    height="480";
    string pipeline =
        "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
        "! video/x-raw,width="+width+",height="+height+",framerate=10/1,format=RGBx "
        "! videoconvert ! videoscale ! appsink";
    return pipeline;
}

void Widget::openCamera(string& pipeline) {
    if(!cap.open(pipeline, CAP_GSTREAMER)) {
        qDebug()<<"Failed to open the camera!";
    }
}

//Set the timer to capture the frame
void Widget::setTimerForFrame() {
    captureTimer = new QTimer(this);
    connect(captureTimer, &QTimer::timeout, this, &Widget::updateFrame);
    return;
}

//ON/OFF the video
void Widget::setToggleVideo() {
    connect(ui->videoBtn, &QPushButton::clicked, this, [=](){
        if(!videoFlag)  //status : video off -> on
        {
            ui->videoBtn->setIcon(videoOnIcon);
            cam->setStyleSheet("background-color : white;");
            cam->show();
            //captureTimer->start(100);

            // width="640";
            // height="480";
            // string pipeline =
            //     "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
            //     "! video/x-raw,width="+width+",height="+height+",framerate=10/1,format=RGBx "
            //     "! videoconvert ! videoscale ! appsink";
            // if(!cap.open(pipeline, CAP_GSTREAMER)) {
            //     qDebug()<<"Failed to open the camera!";
            // }
            // sendVideo();

            startStreaming();
        }
        else            //status : video on -> off
        {
            ui->videoBtn->setIcon(videoOffIcon);
            //captureTimer->stop();
            cam->hide();
            //카메라 화면 투명화해서 전송

        }
        videoFlag = !videoFlag;
        ui->videoBtn->setIconSize(QSize(40, 40));
    });
    cam->setStyleSheet("background-color : rgb(0,0,0,0);");
    return;
}

void Widget::setBackground() {
    // change background picture
    connect(ui->changeBackBtn, &QPushButton::clicked, this, [this](){
        QString nextPath = QString::fromStdString(picture.getNextPicture());
        qDebug() << "nextPath = " << nextPath;
        ui->background->setPixmap(QPixmap(nextPath));
    });
    connect(ui->addBackBtn, SIGNAL(clicked()), this, SLOT(selectPicture()));
    return;
}

// 새로운 프레임 캡처
Mat& Widget::captureNewFrame() {
     Mat frame;
    // cap.read(frame);
    // if (frame.empty()) {
    //     qDebug() << "Unable to grab frame!";
    // }
    // networkManager::
     return frame;
}

// 통합 비디오 수신 및 재생
void Widget::getVideo() {
    qDebug() << tcpSocket->state();
   while (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        // 프레임 크기 읽기
       qDebug() << "영상 수신 시도";
        int64_t frameSize = 0;
        if (tcpSocket->bytesAvailable() < sizeof(frameSize)) {
            if (!tcpSocket->waitForReadyRead()) { // Wait for the frame size
                cerr << "[ERROR] Waiting for frame size failed" << endl;
                return;
            }
        }
        tcpSocket->read(reinterpret_cast<char*>(&frameSize), sizeof(frameSize));

        if (frameSize <= 0 || frameSize > 10 * 1024 * 1024) { // Sanity check
            cerr << "[ERROR] Invalid frame size received: " << frameSize << endl;
            return;
        }

        // 프레임 읽기
        QByteArray frameData;
        while (frameData.size() < frameSize) {
            if (!tcpSocket->waitForReadyRead()) { // Wait for more data
                cerr << "[ERROR] Frame data reception timed out" << endl;
                return;
            }
            frameData.append(tcpSocket->read(frameSize - frameData.size()));
        }

        // 프레임 디코드
        vector<uchar> buffer(frameData.begin(), frameData.end());
        Mat frame = imdecode(buffer, IMREAD_COLOR);

        if (frame.empty()) {
            cerr << "[ERROR] Failed to decode the frame" << endl;
            continue;
        }

        // Mat -> QImage
        QImage img((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        img = img.rgbSwapped(); // Convert BGR to RGB

        videoWindow->setFixedSize(img.width() / 5, img.height() / 5);
        videoWindow->setPixmap(QPixmap::fromImage(img).scaled(videoWindow->size(), Qt::KeepAspectRatio));

        // 선택: Introduce a small delay for smoother playback
        QCoreApplication::processEvents();
   }
}

void Widget::sendVideo()
{
    qDebug() << tcpSocket->state();
    while (tcpSocket->state() == QAbstractSocket::ConnectedState) {

        Mat frame;
        vector<uchar> buffer;
        qDebug() << "영상 송신 시도";
        while(cap.read(frame))
        {
            if(frame.empty())
            {
                qDebug() <<"Unable to grab frame!!";
                continue;
            }
            // Encode frame as JPEG
            imencode(".jpg", frame, buffer);

            // Send size of the buffer first
            qint64 bufferSize = buffer.size();
            tcpSocket->write(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));

            // Send the actual buffer data
            tcpSocket->write(reinterpret_cast<const char*>(buffer.data()), bufferSize);

            if (!tcpSocket->waitForBytesWritten(3000)) {
                qDebug() << "Error sending frame!";
            }

            QThread::msleep(30); // To maintain the frame rate
        }

        // 선택: Introduce a small delay for smoother playback
        QCoreApplication::processEvents();
        cap.release();
    }
}
