/********************************************************************************
** Form generated from reading UI file 'cvconferenceroom.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVCONFERENCEROOM_H
#define UI_CVCONFERENCEROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CvConferenceRoomClass
{
public:

    void setupUi(QWidget *CvConferenceRoomClass)
    {
        if (CvConferenceRoomClass->objectName().isEmpty())
            CvConferenceRoomClass->setObjectName(QStringLiteral("CvConferenceRoomClass"));
        CvConferenceRoomClass->resize(600, 400);

        retranslateUi(CvConferenceRoomClass);

        QMetaObject::connectSlotsByName(CvConferenceRoomClass);
    } // setupUi

    void retranslateUi(QWidget *CvConferenceRoomClass)
    {
        CvConferenceRoomClass->setWindowTitle(QApplication::translate("CvConferenceRoomClass", "CvConferenceRoom", 0));
    } // retranslateUi

};

namespace Ui {
    class CvConferenceRoomClass: public Ui_CvConferenceRoomClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVCONFERENCEROOM_H
