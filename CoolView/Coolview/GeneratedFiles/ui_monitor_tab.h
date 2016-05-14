/********************************************************************************
** Form generated from reading UI file 'monitor_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITOR_TAB_H
#define UI_MONITOR_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MonitorTab
{
public:
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_3;
    QLabel *mainboardTempLabel;
    QLabel *label_6;
    QLabel *cpuTempLabel;
    QLabel *label_8;
    QLabel *cpuLoadLabel;
    QLabel *label_10;
    QLabel *ramDataLabel;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *outBandwidthLabel;
    QLabel *inBandwidthLabel;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *mainboardTempWarningEdit;
    QLabel *label_31;
    QComboBox *mainboardActionComboBox;
    QLabel *label_2;
    QLineEdit *cpuTempWarningEdit;
    QLabel *label_4;
    QComboBox *cpuActionComboBox;
    QLabel *label_5;
    QLabel *label_7;
    QPushButton *monitorConfigSaveButton;

    void setupUi(QWidget *MonitorTab)
    {
        if (MonitorTab->objectName().isEmpty())
            MonitorTab->setObjectName(QStringLiteral("MonitorTab"));
        MonitorTab->resize(560, 367);
        groupBox = new QGroupBox(MonitorTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 30, 251, 191));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(11);
        font.setBold(false);
        font.setWeight(50);
        groupBox->setFont(font);
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        mainboardTempLabel = new QLabel(groupBox);
        mainboardTempLabel->setObjectName(QStringLiteral("mainboardTempLabel"));
        mainboardTempLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::FieldRole, mainboardTempLabel);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

        cpuTempLabel = new QLabel(groupBox);
        cpuTempLabel->setObjectName(QStringLiteral("cpuTempLabel"));
        cpuTempLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::FieldRole, cpuTempLabel);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_8);

        cpuLoadLabel = new QLabel(groupBox);
        cpuLoadLabel->setObjectName(QStringLiteral("cpuLoadLabel"));
        cpuLoadLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::FieldRole, cpuLoadLabel);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_10);

        ramDataLabel = new QLabel(groupBox);
        ramDataLabel->setObjectName(QStringLiteral("ramDataLabel"));
        ramDataLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::FieldRole, ramDataLabel);

        groupBox_2 = new QGroupBox(MonitorTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(290, 30, 261, 191));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(12);
        groupBox_2->setFont(font1);
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout_3->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QStringLiteral("label_19"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_19);

        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QStringLiteral("label_20"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_20);

        outBandwidthLabel = new QLabel(groupBox_2);
        outBandwidthLabel->setObjectName(QStringLiteral("outBandwidthLabel"));
        outBandwidthLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, outBandwidthLabel);

        inBandwidthLabel = new QLabel(groupBox_2);
        inBandwidthLabel->setObjectName(QStringLiteral("inBandwidthLabel"));
        inBandwidthLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, inBandwidthLabel);

        groupBox_3 = new QGroupBox(MonitorTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 230, 531, 121));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(11);
        groupBox_3->setFont(font2);
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 3, 1, 1);

        mainboardTempWarningEdit = new QLineEdit(groupBox_3);
        mainboardTempWarningEdit->setObjectName(QStringLiteral("mainboardTempWarningEdit"));
        mainboardTempWarningEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(mainboardTempWarningEdit, 0, 1, 1, 1);

        label_31 = new QLabel(groupBox_3);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout->addWidget(label_31, 0, 0, 1, 1);

        mainboardActionComboBox = new QComboBox(groupBox_3);
        mainboardActionComboBox->setObjectName(QStringLiteral("mainboardActionComboBox"));

        gridLayout->addWidget(mainboardActionComboBox, 0, 4, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        cpuTempWarningEdit = new QLineEdit(groupBox_3);
        cpuTempWarningEdit->setObjectName(QStringLiteral("cpuTempWarningEdit"));
        cpuTempWarningEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(cpuTempWarningEdit, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        cpuActionComboBox = new QComboBox(groupBox_3);
        cpuActionComboBox->setObjectName(QStringLiteral("cpuActionComboBox"));

        gridLayout->addWidget(cpuActionComboBox, 1, 4, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 1, 2, 1, 1);

        monitorConfigSaveButton = new QPushButton(groupBox_3);
        monitorConfigSaveButton->setObjectName(QStringLiteral("monitorConfigSaveButton"));
        monitorConfigSaveButton->setMinimumSize(QSize(80, 60));

        gridLayout->addWidget(monitorConfigSaveButton, 0, 5, 2, 1);


        retranslateUi(MonitorTab);

        QMetaObject::connectSlotsByName(MonitorTab);
    } // setupUi

    void retranslateUi(QWidget *MonitorTab)
    {
        MonitorTab->setWindowTitle(QApplication::translate("MonitorTab", "\347\263\273\347\273\237\347\233\221\346\216\247", 0));
        groupBox->setTitle(QApplication::translate("MonitorTab", "\347\241\254\344\273\266\344\277\241\346\201\257", 0));
        label_3->setText(QApplication::translate("MonitorTab", "\344\270\273\346\235\277\346\270\251\345\272\246\357\274\232", 0));
        mainboardTempLabel->setText(QString());
        label_6->setText(QApplication::translate("MonitorTab", "CPU\346\270\251\345\272\246\357\274\232", 0));
        cpuTempLabel->setText(QString());
        label_8->setText(QApplication::translate("MonitorTab", "CPU\350\264\237\350\275\275\357\274\232", 0));
        cpuLoadLabel->setText(QString());
        label_10->setText(QApplication::translate("MonitorTab", "\345\211\251\344\275\231\345\217\257\347\224\250\345\206\205\345\255\230\357\274\232", 0));
        ramDataLabel->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MonitorTab", "\347\275\221\347\273\234\347\212\266\346\200\201\357\274\232", 0));
        label_19->setText(QApplication::translate("MonitorTab", "\344\270\212\350\241\214\345\270\246\345\256\275\351\200\237\347\216\207\357\274\232", 0));
        label_20->setText(QApplication::translate("MonitorTab", "\344\270\213\350\241\214\345\270\246\345\256\275\351\200\237\347\216\207\357\274\232", 0));
        outBandwidthLabel->setText(QString());
        inBandwidthLabel->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MonitorTab", "\347\233\221\346\216\247\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("MonitorTab", "\346\211\247\350\241\214\345\212\250\344\275\234\357\274\232", 0));
        label_31->setText(QApplication::translate("MonitorTab", "\344\270\273\346\235\277\346\270\251\345\272\246\346\212\245\350\255\246\351\230\210\345\200\274\357\274\232", 0));
        mainboardActionComboBox->clear();
        mainboardActionComboBox->insertItems(0, QStringList()
         << QApplication::translate("MonitorTab", "\346\227\240\345\212\250\344\275\234", 0)
         << QApplication::translate("MonitorTab", "\350\234\202\351\270\243", 0)
         << QApplication::translate("MonitorTab", "\345\205\263\346\234\272", 0)
         << QApplication::translate("MonitorTab", "\351\207\215\345\220\257", 0)
        );
        label_2->setText(QApplication::translate("MonitorTab", "CPU\346\270\251\345\272\246\346\212\245\350\255\246\351\230\210\345\200\274\357\274\232", 0));
        label_4->setText(QApplication::translate("MonitorTab", "\346\211\247\350\241\214\345\212\250\344\275\234\357\274\232", 0));
        cpuActionComboBox->clear();
        cpuActionComboBox->insertItems(0, QStringList()
         << QApplication::translate("MonitorTab", "\346\227\240\345\212\250\344\275\234", 0)
         << QApplication::translate("MonitorTab", "\350\234\202\351\270\243", 0)
         << QApplication::translate("MonitorTab", "\345\205\263\346\234\272", 0)
         << QApplication::translate("MonitorTab", "\351\207\215\345\220\257", 0)
        );
        label_5->setText(QApplication::translate("MonitorTab", "\302\260C ------>", 0));
        label_7->setText(QApplication::translate("MonitorTab", "\302\260C ------>", 0));
        monitorConfigSaveButton->setText(QApplication::translate("MonitorTab", "\344\277\235\345\255\230\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class MonitorTab: public Ui_MonitorTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITOR_TAB_H
