#include "camviewer.h"

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
        qDebug()<<"pos = "<<newPos;
        if(newPos.x() > 10 && newPos.x()< 661 && newPos.y() > 10 && newPos.y() < 355)
        {
            position = newPos;
            qDebug() << "(" << position.x() <<", "<<position.y() <<") 로 이동";
            this->move(newPos);
            //서버에 newPos 전송
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
    switch(key)
    {
     case 16777234: //left
        if(position.x() - 5 > 5) //limit
        {
             position.setX(position.x() - 5);
            qDebug() << "(" << position.x() <<", "<<position.y() <<") 로 이동";
            move(this->pos().x() - 5, this->pos().y());
            //서버에 좌표 전송(this->pos().x() - 5, this->pos().y());
        }
        break;
     case 16777235: //up
        if(position.y() - 5 > 5)
         {
             position.setY(position.y() - 5);
            qDebug() << "(" << position.x() <<", "<<position.y() <<") 로 이동";
            move(this->pos().x(), this->pos().y() - 5);
            //서버에 좌표 전송(this->pos().x(), this->pos().y() - 5);
        }
        break;
     case 16777236: //right
         if(position.x() + 5 < 666)
         {
             position.setX(position.x() + 5);
             qDebug() << "(" << position.x() <<", "<<position.y() <<") 로 이동";
             move(this->pos().x() + 5, this->pos().y());
            //서버에 좌표 전송(this->pos().x() + 5, this->pos().y());
         }
         break;
     case 16777237: //down
         if(position.y() + 5 < 360)
         {
             position.setY(position.y() + 5);
             qDebug() << "(" << position.x() <<", "<<position.y() <<") 로 이동";
             move(this->pos().x(), this->pos().y() + 5);
            //서버에 좌표 전송(this->pos().x(), this->pos().y() + 5);
         }
        break;
    }
}
