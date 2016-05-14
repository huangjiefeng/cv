/********************************************************************************
** Form generated from reading UI file 'device_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICE_WIDGET_H
#define UI_DEVICE_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceWidget
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *deviceButton;
    QLabel *deviceNameLabel;

    void setupUi(QWidget *DeviceWidget)
    {
        if (DeviceWidget->objectName().isEmpty())
            DeviceWidget->setObjectName(QStringLiteral("DeviceWidget"));
        DeviceWidget->resize(98, 116);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DeviceWidget->sizePolicy().hasHeightForWidth());
        DeviceWidget->setSizePolicy(sizePolicy);
        DeviceWidget->setMinimumSize(QSize(98, 116));
        DeviceWidget->setMaximumSize(QSize(98, 116));
        verticalLayout = new QVBoxLayout(DeviceWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        deviceButton = new QPushButton(DeviceWidget);
        deviceButton->setObjectName(QStringLiteral("deviceButton"));
        deviceButton->setMinimumSize(QSize(80, 80));
        deviceButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(deviceButton);

        deviceNameLabel = new QLabel(DeviceWidget);
        deviceNameLabel->setObjectName(QStringLiteral("deviceNameLabel"));
        deviceNameLabel->setScaledContents(false);
        deviceNameLabel->setAlignment(Qt::AlignCenter);
        deviceNameLabel->setWordWrap(true);

        verticalLayout->addWidget(deviceNameLabel);


        retranslateUi(DeviceWidget);

        QMetaObject::connectSlotsByName(DeviceWidget);
    } // setupUi

    void retranslateUi(QWidget *DeviceWidget)
    {
        DeviceWidget->setWindowTitle(QApplication::translate("DeviceWidget", "DeviceWidget", 0));
        deviceButton->setText(QString());
        deviceNameLabel->setText(QApplication::translate("DeviceWidget", "device name", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceWidget: public Ui_DeviceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICE_WIDGET_H
