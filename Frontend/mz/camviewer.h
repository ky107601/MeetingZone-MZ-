#ifndef CAMVIEWER_H
#define CAMVIEWER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include <QKeyEvent>

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

class camViewer : public QLabel
{
    Q_OBJECT
public:
    camViewer(QWidget *parent);
    QPoint position;
    QPoint offset;

    void moveByKey(int key);
    QByteArray XYToJson();
protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
Q_SIGNALS:
    void sendDataToWidget(const QByteArray& data);
};

#endif // CAMVIEWER_H
