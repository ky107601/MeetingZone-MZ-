#include "widget.h"
#include "./ui_widget.h"

#include <QGraphicsDropShadowEffect> //for shadow effect

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

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

    //ui->chatInput->setAlignment(Qt::AlignVCenter);
    ui->chatInput->setPlaceholderText("Enter your text here ...");
}

Widget::~Widget()
{
    delete ui;
}
