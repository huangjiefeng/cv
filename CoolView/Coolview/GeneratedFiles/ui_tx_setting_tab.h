/********************************************************************************
** Form generated from reading UI file 'tx_setting_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TX_SETTING_TAB_H
#define UI_TX_SETTING_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TxSettingTab
{
public:
    QVBoxLayout *txSettingLayout;

    void setupUi(QWidget *TxSettingTab)
    {
        if (TxSettingTab->objectName().isEmpty())
            TxSettingTab->setObjectName(QStringLiteral("TxSettingTab"));
        TxSettingTab->resize(600, 500);
        txSettingLayout = new QVBoxLayout(TxSettingTab);
        txSettingLayout->setSpacing(6);
        txSettingLayout->setContentsMargins(11, 11, 11, 11);
        txSettingLayout->setObjectName(QStringLiteral("txSettingLayout"));

        retranslateUi(TxSettingTab);

        QMetaObject::connectSlotsByName(TxSettingTab);
    } // setupUi

    void retranslateUi(QWidget *TxSettingTab)
    {
        TxSettingTab->setWindowTitle(QApplication::translate("TxSettingTab", "\344\270\212\344\274\240\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class TxSettingTab: public Ui_TxSettingTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TX_SETTING_TAB_H
