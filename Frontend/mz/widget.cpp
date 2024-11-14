#include "widget.h"
#include "./ui_widget.h"

#include <QGraphicsDropShadowEffect> //for shadow effect

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->showMaximized();

    /* Set the inputBox style */
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(5);         // Set blur radius for the shadow
    shadowEffect->setOffset(5, 5);           // Set shadow offset (x, y)
    shadowEffect->setColor(Qt::gray);       // Set shadow color
    ui->inputBox->setGraphicsEffect(shadowEffect);

    QWidget *page2 = new QWidget;
    ui->stackedWidget->addWidget(page2);

    connect(ui->enterBtn, &QPushButton::clicked, this, [&](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    ui->chatInput->setPlaceholderText("Enter your text here ...");

    QIcon micIcon(":/resources/mic_off.png");
    ui->micBtn->setIcon(micIcon);
    ui->micBtn->setIconSize(QSize(35,35));

    QIcon videoIcon(":/resources/video_off.png");
    ui->videoBtn->setIcon(videoIcon);
    ui->videoBtn->setIconSize(QSize(35,35));

    QIcon exitIcon(":/resources/exit.png");
    ui->exitBtn->setIcon(exitIcon);
    ui->exitBtn->setIconSize(QSize(35,35));

    connect(ui->exitBtn, &QPushButton::clicked, this, [&](){
        ui->stackedWidget->setCurrentIndex(0);
    });

}

Widget::~Widget()
{
    delete ui;
}
