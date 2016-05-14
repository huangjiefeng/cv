/********************************************************************************
** Form generated from reading UI file 'device_setting_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICE_SETTING_WIDGET_H
#define UI_DEVICE_SETTING_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceSettingForm
{
public:
    QGridLayout *gridLayout_6;
    QTabWidget *settingTabWidget;

    void setupUi(QWidget *DeviceSettingForm)
    {
        if (DeviceSettingForm->objectName().isEmpty())
            DeviceSettingForm->setObjectName(QStringLiteral("DeviceSettingForm"));
        DeviceSettingForm->resize(624, 536);
        gridLayout_6 = new QGridLayout(DeviceSettingForm);
        gridLayout_6->setSpacing(0);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        settingTabWidget = new QTabWidget(DeviceSettingForm);
        settingTabWidget->setObjectName(QStringLiteral("settingTabWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(9);
        settingTabWidget->setFont(font);

        gridLayout_6->addWidget(settingTabWidget, 0, 0, 1, 1);


        retranslateUi(DeviceSettingForm);

        settingTabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(DeviceSettingForm);
    } // setupUi

    void retranslateUi(QWidget *DeviceSettingForm)
    {
        DeviceSettingForm->setWindowTitle(QApplication::translate("DeviceSettingForm", "\347\273\210\347\253\257\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceSettingForm: public Ui_DeviceSettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICE_SETTING_WIDGET_H
