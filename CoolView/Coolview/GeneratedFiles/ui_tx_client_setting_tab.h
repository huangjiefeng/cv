/********************************************************************************
** Form generated from reading UI file 'tx_client_setting_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TX_CLIENT_SETTING_TAB_H
#define UI_TX_CLIENT_SETTING_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TxClientSettingTab
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayoutRec;
    QCheckBox *ckbLocalRecord;
    QLabel *label_23;
    QSpinBox *spinBoxRecAutoSeg;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *txAddressLineEdit;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *localRecStatusLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *saveButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *TxClientSettingTab)
    {
        if (TxClientSettingTab->objectName().isEmpty())
            TxClientSettingTab->setObjectName(QStringLiteral("TxClientSettingTab"));
        TxClientSettingTab->resize(480, 297);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TxClientSettingTab->sizePolicy().hasHeightForWidth());
        TxClientSettingTab->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(TxClientSettingTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_2 = new QGroupBox(TxClientSettingTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayoutRec = new QHBoxLayout();
        horizontalLayoutRec->setSpacing(6);
        horizontalLayoutRec->setObjectName(QStringLiteral("horizontalLayoutRec"));
        ckbLocalRecord = new QCheckBox(groupBox_2);
        ckbLocalRecord->setObjectName(QStringLiteral("ckbLocalRecord"));

        horizontalLayoutRec->addWidget(ckbLocalRecord);

        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QStringLiteral("label_23"));

        horizontalLayoutRec->addWidget(label_23);

        spinBoxRecAutoSeg = new QSpinBox(groupBox_2);
        spinBoxRecAutoSeg->setObjectName(QStringLiteral("spinBoxRecAutoSeg"));
        spinBoxRecAutoSeg->setMinimumSize(QSize(60, 0));
        spinBoxRecAutoSeg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxRecAutoSeg->setMaximum(7200);

        horizontalLayoutRec->addWidget(spinBoxRecAutoSeg);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutRec->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayoutRec);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        txAddressLineEdit = new QLineEdit(groupBox_2);
        txAddressLineEdit->setObjectName(QStringLiteral("txAddressLineEdit"));

        horizontalLayout_3->addWidget(txAddressLineEdit);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(TxClientSettingTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        localRecStatusLabel = new QLabel(groupBox);
        localRecStatusLabel->setObjectName(QStringLiteral("localRecStatusLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(localRecStatusLabel->sizePolicy().hasHeightForWidth());
        localRecStatusLabel->setSizePolicy(sizePolicy1);
        localRecStatusLabel->setMinimumSize(QSize(0, 70));
        localRecStatusLabel->setTextFormat(Qt::PlainText);
        localRecStatusLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        localRecStatusLabel->setWordWrap(true);
        localRecStatusLabel->setIndent(-1);

        horizontalLayout->addWidget(localRecStatusLabel);


        verticalLayout->addWidget(groupBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        saveButton = new QPushButton(TxClientSettingTab);
        saveButton->setObjectName(QStringLiteral("saveButton"));

        horizontalLayout_2->addWidget(saveButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(TxClientSettingTab);

        QMetaObject::connectSlotsByName(TxClientSettingTab);
    } // setupUi

    void retranslateUi(QWidget *TxClientSettingTab)
    {
        TxClientSettingTab->setWindowTitle(QApplication::translate("TxClientSettingTab", "TxClientSettingTab", 0));
        groupBox_2->setTitle(QApplication::translate("TxClientSettingTab", "\346\234\254\345\234\260\345\275\225\345\210\266\350\256\276\347\275\256", 0));
        ckbLocalRecord->setText(QApplication::translate("TxClientSettingTab", "\345\220\257\347\224\250\346\234\254\345\234\260\345\275\225\345\210\266", 0));
        label_23->setText(QApplication::translate("TxClientSettingTab", "\350\207\252\345\212\250\345\210\206\345\211\262\351\227\264\351\232\224(\345\210\206)", 0));
#ifndef QT_NO_TOOLTIP
        spinBoxRecAutoSeg->setToolTip(QApplication::translate("TxClientSettingTab", "0\344\270\272\345\205\263\351\227\255\357\274\214\346\234\200\345\244\247\345\200\2747200", 0));
#endif // QT_NO_TOOLTIP
        spinBoxRecAutoSeg->setSpecialValueText(QApplication::translate("TxClientSettingTab", "\345\205\263\351\227\255", 0));
        label->setText(QApplication::translate("TxClientSettingTab", "\345\275\225\345\210\266\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200", 0));
        txAddressLineEdit->setPlaceholderText(QApplication::translate("TxClientSettingTab", "\347\244\272\344\276\213:192.168.1.123:8091 \346\210\226 \\\\tx_host_name:8091", 0));
        groupBox->setTitle(QApplication::translate("TxClientSettingTab", "\346\234\254\345\234\260\345\275\225\345\210\266\347\212\266\346\200\201", 0));
        localRecStatusLabel->setText(QApplication::translate("TxClientSettingTab", "\350\247\206\351\242\221\350\276\223\345\205\2451\357\274\232\n"
"\350\247\206\351\242\221\350\276\223\345\205\2452\357\274\232\n"
"\350\247\206\351\242\221\350\276\223\345\205\2453\357\274\232\n"
"\350\247\206\351\242\221\350\276\223\345\205\2454\357\274\232\n"
"\347\243\201\347\233\230\345\217\257\347\224\250\347\251\272\351\227\264\357\274\232", 0));
        saveButton->setText(QApplication::translate("TxClientSettingTab", "\344\277\235\345\255\230", 0));
    } // retranslateUi

};

namespace Ui {
    class TxClientSettingTab: public Ui_TxClientSettingTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TX_CLIENT_SETTING_TAB_H
