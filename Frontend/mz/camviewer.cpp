#include "camviewer.h"
#include "widget.h"
#include <QThread>

camViewer::camViewer(QWidget*parent) : QLabel(parent)
{
    qDebug()<<"camViewer constructor";
    position = QPoint(10, 10); //start position
}

void camViewer::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoveEvent()";
    if (event->buttons() & Qt::LeftButton) { //drag
        QPoint newPos = event->globalPos() - offset; //실제로 이동
        qDebug() << "pos = " << newPos;
        if(newPos.x() > 10 && newPos.x()< 661 && newPos.y() > 10 && newPos.y() < 355)
        {
            position = newPos;
            qDebug() << "(" << position.x() << ", " << position.y() << ") 로 이동";
            this->move(newPos);

          //  XYToJson();
        }
    }
}

void camViewer::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mousePressEvent()";
    if (event->button() == Qt::LeftButton) { //when clicked
        offset = position - this->geometry().topLeft(); //original positon
    }
}

void camViewer::moveByKey(int key)
{
    //left : 16777234, down : 16777235, right : 16777236, up : 16777237
    //left : 65, down : 83, right : 68, up: 87
    switch(key)
    {
     case 65: //left
        if(this->pos().x() - 5 > 5) //limit
        {
            this->move(this->pos().x() - 5, this->pos().y());
            qDebug() << "(" << this->pos().x() << ", " << this->pos().y() << ") 로 이동";
           // XYToJson();
        }
        break;
     case 87: //up
         if(this->pos().y() - 5 > 5)
         {
            this->move(this->pos().x(), this->pos().y() - 5);
              qDebug() << "(" << this->pos().x() << ", " << this->pos().y() << ") 로 이동";
            // XYToJson();
         }
        break;
     case 68: //right
         if(this->pos().x() + 5 < 666)
         {
            this->move(this->pos().x() + 5, this->pos().y());
              qDebug() << "(" << this->pos().x() << ", " << this->pos().y() << ") 로 이동";
            // XYToJson();
         }
        break;
     case 83: //down
         if(this->pos().y() + 5 < 360)
         {
             this->move(this->pos().x(), this->pos().y() + 5);
              qDebug() << "(" << this->pos().x() << ", " << this->pos().y() << ") 로 이동";
            // XYToJson();
         }
         break;
    }
}

QByteArray camViewer::XYToJson()
{
    // JSON으로 변환
    QJsonObject json;
    json["x"] = this->pos().x();
    json["y"] = this->pos().y();
    QJsonDocument doc(json);
    QByteArray serializedData = doc.toJson(QJsonDocument::Compact);

    qDebug()<<"json 으로 serialize 완료";

    return serializedData;

    //sendDataToWidget(serializedData);
}
