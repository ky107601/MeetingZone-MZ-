/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *serverIP;
    QLineEdit *serverPort;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *connectButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(400, 300);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setMinimumSize(QSize(100, 30));

        horizontalLayout->addWidget(label);

        serverIP = new QLineEdit(Widget);
        serverIP->setObjectName("serverIP");

        horizontalLayout->addWidget(serverIP);

        serverPort = new QLineEdit(Widget);
        serverPort->setObjectName("serverPort");

        horizontalLayout->addWidget(serverPort);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");

        verticalLayout->addLayout(horizontalLayout_3);

        connectButton = new QPushButton(Widget);
        connectButton->setObjectName("connectButton");
        connectButton->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(connectButton);

        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName("textEdit");

        verticalLayout_2->addWidget(textEdit);


        verticalLayout->addWidget(groupBox);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\354\240\221\354\206\215\355\225\240 \354\204\234\353\262\204 IP", nullptr));
        serverIP->setText(QString());
        serverIP->setPlaceholderText(QCoreApplication::translate("Widget", "Server IP", nullptr));
        serverPort->setInputMask(QString());
        serverPort->setText(QString());
        serverPort->setPlaceholderText(QCoreApplication::translate("Widget", "Server Port", nullptr));
        connectButton->setText(QCoreApplication::translate("Widget", "\354\204\234\353\262\204 \354\240\221\354\206\215", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Widget", "\353\260\233\354\235\200 \353\251\224\354\204\270\354\247\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
