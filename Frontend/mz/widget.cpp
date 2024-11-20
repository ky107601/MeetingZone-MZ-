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

    connect(ui->videoBtn, &QPushButton::clicked, this, [&](){
        if(!videoFlag)  //status : video off -> on
            ui->videoBtn->setIcon(videoOnIcon);
        else            //status : video on -> off
            ui->videoBtn->setIcon(videoOffIcon);
        videoFlag = !videoFlag;
        ui->videoBtn->setIconSize(QSize(40, 40));
    });

    string pipeline =
        "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
        "! video/x-raw,width=640,height=480,framerate=10/1,format=RGBx "
        "! videoconvert ! videoscale ! appsink";

    if(!cap.open(pipeline, CAP_GSTREAMER))
    {
        qDebug()<<"Failed to open the camera!";
    }


    QTimer *captureTimer = new QTimer(this);
    connect(captureTimer, &QTimer::timeout, this, &Widget::updateFrame);

    captureTimer->start(100);

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

    Mat frame;
    cap.read(frame); // Capture a new frame

    if (frame.empty()) {
        qDebug() << "Unable to grab frame!";
        return;
    }
    else{
        qDebug() << "I got the frameeeeeeee!!";

        cvtColor(frame, frame, COLOR_BGR2RGB); // Convert to RGB format
        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        ui->videoLabel->setFixedSize(frame.cols, frame.rows);
        ui->videoLabel->setPixmap(QPixmap::fromImage(qimg).scaled(ui->videoLabel->size(), Qt::KeepAspectRatio));
    }

}
