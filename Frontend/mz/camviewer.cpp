#include "camviewer.h"

camViewer::camViewer(QWidget*parent) : QLabel(parent)
{
    qDebug()<<"camViewer constructor";
    this->move(10, 10); //start position
}

void camViewer::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoveEvent()";
    if (event->buttons() & Qt::LeftButton) { //drag
        QPoint newPos = event->globalPos() - offset;
        qDebug()<<"pos = "<<newPos;
        if(newPos.x() > 10 && newPos.x()< 661 && newPos.y() > 10 && newPos.y() < 355)
            this->move(newPos);
    }
}

void camViewer::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mousePressEvent()";
    if (event->button() == Qt::LeftButton) { //when clicked
        offset = event->globalPos() - this->geometry().topLeft(); //original positon
    }
}

void camViewer::moveByKey(int key)
{
    //left : 16777234, down : 16777235, right : 16777236, up : 16777237
    switch(key)
    {
     case 16777234: //left
        if(this->pos().x() - 5 > 5) //limit
            move(this->pos().x() - 5, this->pos().y());
         break;
     case 16777235: //up
         if(this->pos().y() - 5 > 5)
            move(this->pos().x(), this->pos().y() - 5);
         break;
     case 16777236: //right
         if(this->pos().x() + 5 < 666)
             move(this->pos().x() + 5, this->pos().y());
         break;
     case 16777237: //down
         if(this->pos().y() + 5 < 360)
             move(this->pos().x(), this->pos().y() + 5);
         break;

    }
}