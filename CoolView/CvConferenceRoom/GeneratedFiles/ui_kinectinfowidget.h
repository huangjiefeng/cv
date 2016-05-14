/********************************************************************************
** Form generated from reading UI file 'kinectinfowidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECTINFOWIDGET_H
#define UI_KINECTINFOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectInfoWidgetClass
{
public:
    QLabel *KinectInfoText;

    void setupUi(QWidget *KinectInfoWidgetClass)
    {
        if (KinectInfoWidgetClass->objectName().isEmpty())
            KinectInfoWidgetClass->setObjectName(QStringLiteral("KinectInfoWidgetClass"));
        KinectInfoWidgetClass->resize(450, 150);
        KinectInfoWidgetClass->setMinimumSize(QSize(450, 150));
        KinectInfoWidgetClass->setMaximumSize(QSize(450, 150));
        KinectInfoWidgetClass->setFocusPolicy(Qt::StrongFocus);
        KinectInfoText = new QLabel(KinectInfoWidgetClass);
        KinectInfoText->setObjectName(QStringLiteral("KinectInfoText"));
        KinectInfoText->setGeometry(QRect(10, 20, 381, 71));
        KinectInfoText->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        KinectInfoText->setScaledContents(true);

        retranslateUi(KinectInfoWidgetClass);

        QMetaObject::connectSlotsByName(KinectInfoWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *KinectInfoWidgetClass)
    {
        KinectInfoWidgetClass->setWindowTitle(QApplication::translate("KinectInfoWidgetClass", "Kinect\344\277\241\346\201\257\347\252\227\345\217\243", 0));
        KinectInfoText->setText(QApplication::translate("KinectInfoWidgetClass", "KinectInfoText", 0));
    } // retranslateUi

};

namespace Ui {
    class KinectInfoWidgetClass: public Ui_KinectInfoWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTINFOWIDGET_H
