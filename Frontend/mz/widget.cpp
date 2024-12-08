#include "widget.h"
#include "./ui_widget.h"

#include <QGraphicsDropShadowEffect> //for shadow effect
#include <QMessageBox>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsView>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->showMaximized(); //full-size

    startStreaming();
    setUI();
    
    string pipeline = setPipeline();
    openCamera(pipeline);

    setTimerForFrame();
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
void Widget::changeIcon()
{
    if(!micFlag) //status : mic off -> on
        ui->micBtn->setIcon(micOnIcon);
    else        //status : mic on -> off
        ui->micBtn->setIcon(micOffIcon);
    micFlag = !micFlag;
    ui->micBtn->setIconSize(QSize(40, 40));
}

void Widget::updateFrame() {
    // GrabCut 상태 유지
    static Mat prevMask, bgModel, fgModel;
    static bool isInitialized = false;
    static int frameCounter = 0;
    static Mat lastBinaryMask, prevFrame;
    
    Mat frame = captureNewFrame();
    setROI(frameROI); // 관심 영역 (ROI) 동적 설정
    initGrabCut(frameROI, isInitialized); // GrabCut 초기화

    // 프레임 샘플링 (10프레임마다 GrabCut 실행)
    if (frameCounter % 10 == 0) {
        grabCut(frameROI, prevMask, Rect(), bgModel, fgModel, 1, GC_INIT_WITH_MASK);
        lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
    }

    // ROI 기반 이진 마스크 축소/확대
    Mat smallMask, binaryMask;
    cv::resize(lastBinaryMask, smallMask, Size(), 0.5, 0.5, INTER_NEAREST);
    cv::resize(smallMask, binaryMask, frame.size(), 0, 0, INTER_NEAREST);

    // RGBA 이미지 생성
    Mat transparentImg(frame.size(), CV_8UC4, Scalar(0, 0, 0, 0));
    vector<Mat> bgrChannels;
    split(frame, bgrChannels);

    Mat alphaChannel;
    binaryMask.convertTo(alphaChannel, CV_8UC1, 255.0);
    bgrChannels.push_back(alphaChannel);
    merge(bgrChannels, transparentImg);
    
    // Convert to QImage with transparency
    QImage qimg(transparentImg.data, transparentImg.cols, transparentImg.rows, transparentImg.step, QImage::Format_RGBA8888);

    //라벨에 표시
    cam->setFixedSize(transparentImg.cols / 5, transparentImg.rows / 5);
    cam->setPixmap(QPixmap::fromImage(qimg).scaled(cam->size(), Qt::KeepAspectRatio));

    // 프레임 카운터 증가
    frameCounter++;
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
    NetworkManager& networkManager = NetworkManager::getInstance();
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

    connect(ui->enterBtn, &QPushButton::clicked, this, [&](){
        //Check the server address and the nickname is written
        ui->stackedWidget->setCurrentIndex(1);
    });

    ui->changeBackBtn->setIcon(QIcon(":/resources/change.png"));
    ui->addBackBtn->setIcon(QIcon(":/resources/plus.png"));
    cam = new camViewer(ui->backWidget);
    cam->hide();

    ui->chatInput->setPlaceholderText("Enter your text here ...");

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
            ui->stackedWidget->setCurrentIndex(0);
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
            cam->show();
            captureTimer->start(100);
        }
        else            //status : video on -> off
        {
            ui->videoBtn->setIcon(videoOffIcon);
            captureTimer->stop();
            cam->hide();
        }
        videoFlag = !videoFlag;
        ui->videoBtn->setIconSize(QSize(40, 40));
    });
    cam->setStyleSheet("background-color : rgba(0, 0, 0, 0);");
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
    cap.read(frame);
    if (frame.empty()) {
        qDebug() << "Unable to grab frame!";
    }
    return frame;
}

// 관심 영역 (ROI) 동적 설정
void Widget::setROI(Mat& frameROI) {
    Rect roi(10, 10, frame.cols-20, frame.rows-20);
    frameROI = frame(roi);
    frame.copyTo(prevFrame);
    return;
}

// GrabCut 초기화
void Widget::initGrabCut(Mat& frameROI, bool& isInitialized) {
    if (!isInitialized) {
        prevMask = Mat(frameROI.size(), CV_8UC1, Scalar(GC_BGD));
        Rect initRect(10, 10, frameROI.cols - 20, frameROI.rows - 20);
        prevMask(initRect).setTo(Scalar(GC_PR_FGD));
        grabCut(frameROI, prevMask, initRect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
        lastBinaryMask = (prevMask == GC_FGD) | (prevMask == GC_PR_FGD);
        isInitialized = true;
    }
    return;
}
