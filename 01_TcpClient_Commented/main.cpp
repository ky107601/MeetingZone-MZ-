#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // [01] 위젯 객체 생성
    QApplication a(argc, argv);
    Widget w;
    QString title ="TCP 클라이언트 (" + w.getMyIP().toString() + ")";
    w.setWindowTitle(title);
    w.show();

    return a.exec();
}
