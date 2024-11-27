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

    width="640";
    height="480";
    string pipeline =
        "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
        "! video/x-raw,width="+width+",height="+height+",framerate=10/1,format=RGBx "
        "! videoconvert ! videoscale ! appsink";

    if(!cap.open(pipeline, CAP_GSTREAMER))
    {
        qDebug()<<"Failed to open the camera!";
    }

    //Set the timer to capture the frame
    captureTimer = new QTimer(this);
    connect(captureTimer, &QTimer::timeout, this, &Widget::updateFrame);

    //ON/OFF the video
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

    // change background picture
    connect(ui->changeBackBtn, &QPushButton::clicked, this, [this](){
        QString nextPath = QString::fromStdString(picture.getNextPicture());
        qDebug() << "nextPath = " << nextPath;
        ui->background->setPixmap(QPixmap(nextPath));
    });

    connect(ui->addBackBtn, SIGNAL(clicked()), this, SLOT(selectPicture()));
}

Widget::~Widget()
{
    delete ui;
}

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
    static Ptr<BackgroundSubtractor> bgSubtractor = createBackgroundSubtractorMOG2();

    Mat frame, fgMask, rgbaFrame;
    cap.read(frame); // Capture a new frame

    if (frame.empty()) {
        qDebug() << "Unable to grab frame!";
        return;
    }

    // 학습 형태
    //bgSubtractor->apply(frame, fgMask, 0.001);

    static int frameCount = 0;
    if (frameCount < 100) { // 초기 100프레임 동안만 학습
        bgSubtractor->apply(frame, fgMask, -1);
        frameCount++;
    } else {
        bgSubtractor->apply(frame, fgMask, 0); // 이후 학습 중지
    }

    // 가우시안-부드럽게 threshold-이진화
    GaussianBlur(fgMask, fgMask, Size(11, 11), 3.5, 3.5);
    threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);

    cvtColor(frame, rgbaFrame, COLOR_BGR2RGBA);

    // 마스크 병합
    for (int y = 0; y < fgMask.rows; ++y) {
        for (int x = 0; x < fgMask.cols; ++x) {
            if (fgMask.at<uchar>(y, x) == 0) {
                // Background: set alpha to 0 (transparent)
                rgbaFrame.at<Vec4b>(y, x)[3] = 255;
            } else {
                // Foreground: set alpha to 255 (opaque)
                rgbaFrame.at<Vec4b>(y, x)[3] = 255;
            }
        }
    }

    // Convert to QImage with transparency
    QImage qimg(rgbaFrame.data, rgbaFrame.cols, rgbaFrame.rows, rgbaFrame.step, QImage::Format_RGBA8888);

    //라벨에 표시
    cam->setFixedSize(rgbaFrame.cols / 5, rgbaFrame.rows / 5);
    cam->setPixmap(QPixmap::fromImage(qimg).scaled(cam->size(), Qt::KeepAspectRatio));
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
