/********************************************************************************
** Form generated from reading UI file 'upload_status_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPLOAD_STATUS_WIDGET_H
#define UI_UPLOAD_STATUS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UploadStatusWidget
{
public:
    QVBoxLayout *layout;

    void setupUi(QWidget *UploadStatusWidget)
    {
        if (UploadStatusWidget->objectName().isEmpty())
            UploadStatusWidget->setObjectName(QStringLiteral("UploadStatusWidget"));
        UploadStatusWidget->resize(400, 300);
        layout = new QVBoxLayout(UploadStatusWidget);
        layout->setSpacing(6);
        layout->setContentsMargins(11, 11, 11, 11);
        layout->setObjectName(QStringLiteral("layout"));

        retranslateUi(UploadStatusWidget);

        QMetaObject::connectSlotsByName(UploadStatusWidget);
    } // setupUi

    void retranslateUi(QWidget *UploadStatusWidget)
    {
        UploadStatusWidget->setWindowTitle(QApplication::translate("UploadStatusWidget", "UploadStatusWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class UploadStatusWidget: public Ui_UploadStatusWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPLOAD_STATUS_WIDGET_H
