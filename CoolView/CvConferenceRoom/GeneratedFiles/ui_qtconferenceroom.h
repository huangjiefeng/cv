/********************************************************************************
** Form generated from reading UI file 'qtconferenceroom.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTCONFERENCEROOM_H
#define UI_QTCONFERENCEROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtConferenceRoomClass
{
public:
    QGridLayout *gridLayout;
    QWidget *centralwidget;

    void setupUi(QWidget *QtConferenceRoomClass)
    {
        if (QtConferenceRoomClass->objectName().isEmpty())
            QtConferenceRoomClass->setObjectName(QStringLiteral("QtConferenceRoomClass"));
        QtConferenceRoomClass->resize(500, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtConferenceRoomClass->sizePolicy().hasHeightForWidth());
        QtConferenceRoomClass->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(QtConferenceRoomClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        centralwidget = new QWidget(QtConferenceRoomClass);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));

        gridLayout->addWidget(centralwidget, 0, 0, 1, 1);


        retranslateUi(QtConferenceRoomClass);

        QMetaObject::connectSlotsByName(QtConferenceRoomClass);
    } // setupUi

    void retranslateUi(QWidget *QtConferenceRoomClass)
    {
        QtConferenceRoomClass->setWindowTitle(QApplication::translate("QtConferenceRoomClass", "CoolView", 0));
    } // retranslateUi

};

namespace Ui {
    class QtConferenceRoomClass: public Ui_QtConferenceRoomClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTCONFERENCEROOM_H
