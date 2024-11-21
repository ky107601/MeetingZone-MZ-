#ifndef CAMVIEWER_H
#define CAMVIEWER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class camViewer : public QLabel
{
    Q_OBJECT
public:
    camViewer(QWidget *parent);
    QPoint position;
    QPoint offset;

protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // CAMVIEWER_H
