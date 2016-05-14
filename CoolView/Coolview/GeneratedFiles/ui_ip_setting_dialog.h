/********************************************************************************
** Form generated from reading UI file 'ip_setting_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IP_SETTING_DIALOG_H
#define UI_IP_SETTING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IPSettingDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QWidget *widget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_4;
    QComboBox *adaptorListCombo;
    QGroupBox *groupBox;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_5;
    QLineEdit *ipAddressLineEdit;
    QLineEdit *maskLineEdit;
    QLineEdit *gatewayLineEdit;
    QLineEdit *dnsLineEdit;
    QCheckBox *enableDHCPCheckbox;

    void setupUi(QDialog *IPSettingDialog)
    {
        if (IPSettingDialog->objectName().isEmpty())
            IPSettingDialog->setObjectName(QStringLiteral("IPSettingDialog"));
        IPSettingDialog->resize(308, 267);
        layoutWidget = new QWidget(IPSettingDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 230, 281, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        saveButton = new QPushButton(layoutWidget);
        saveButton->setObjectName(QStringLiteral("saveButton"));

        hboxLayout->addWidget(saveButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        widget = new QWidget(IPSettingDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 291, 211));
        formLayoutWidget = new QWidget(widget);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 271, 22));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        adaptorListCombo = new QComboBox(formLayoutWidget);
        adaptorListCombo->setObjectName(QStringLiteral("adaptorListCombo"));

        formLayout->setWidget(0, QFormLayout::FieldRole, adaptorListCombo);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 60, 271, 141));
        formLayoutWidget_2 = new QWidget(groupBox);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(9, 19, 251, 111));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget_2);
        label->setObjectName(QStringLiteral("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(formLayoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(formLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        label_5 = new QLabel(formLayoutWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_5);

        ipAddressLineEdit = new QLineEdit(formLayoutWidget_2);
        ipAddressLineEdit->setObjectName(QStringLiteral("ipAddressLineEdit"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, ipAddressLineEdit);

        maskLineEdit = new QLineEdit(formLayoutWidget_2);
        maskLineEdit->setObjectName(QStringLiteral("maskLineEdit"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, maskLineEdit);

        gatewayLineEdit = new QLineEdit(formLayoutWidget_2);
        gatewayLineEdit->setObjectName(QStringLiteral("gatewayLineEdit"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, gatewayLineEdit);

        dnsLineEdit = new QLineEdit(formLayoutWidget_2);
        dnsLineEdit->setObjectName(QStringLiteral("dnsLineEdit"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, dnsLineEdit);

        enableDHCPCheckbox = new QCheckBox(widget);
        enableDHCPCheckbox->setObjectName(QStringLiteral("enableDHCPCheckbox"));
        enableDHCPCheckbox->setGeometry(QRect(10, 40, 71, 16));
        widget->raise();
        layoutWidget->raise();

        retranslateUi(IPSettingDialog);
        QObject::connect(saveButton, SIGNAL(clicked()), IPSettingDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(IPSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *IPSettingDialog)
    {
        IPSettingDialog->setWindowTitle(QApplication::translate("IPSettingDialog", "IP\351\205\215\347\275\256", 0));
        saveButton->setText(QApplication::translate("IPSettingDialog", "\344\277\235\345\255\230", 0));
        cancelButton->setText(QApplication::translate("IPSettingDialog", "\345\217\226\346\266\210", 0));
        label_4->setText(QApplication::translate("IPSettingDialog", "\351\200\202\351\205\215\345\231\250\357\274\232", 0));
        groupBox->setTitle(QApplication::translate("IPSettingDialog", "\344\275\277\347\224\250\344\270\213\351\235\242\347\232\204\347\275\221\347\273\234\351\205\215\347\275\256", 0));
        label->setText(QApplication::translate("IPSettingDialog", "IP\345\234\260\345\235\200", 0));
        label_2->setText(QApplication::translate("IPSettingDialog", "\345\255\220\347\275\221\346\216\251\347\240\201", 0));
        label_3->setText(QApplication::translate("IPSettingDialog", "\351\273\230\350\256\244\347\275\221\345\205\263", 0));
        label_5->setText(QApplication::translate("IPSettingDialog", "DNS\346\234\215\345\212\241\345\231\250", 0));
        enableDHCPCheckbox->setText(QApplication::translate("IPSettingDialog", "\344\275\277\347\224\250DHCP", 0));
    } // retranslateUi

};

namespace Ui {
    class IPSettingDialog: public Ui_IPSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IP_SETTING_DIALOG_H
