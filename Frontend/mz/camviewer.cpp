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
