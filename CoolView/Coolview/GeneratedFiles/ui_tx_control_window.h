/********************************************************************************
** Form generated from reading UI file 'tx_control_window.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TX_CONTROL_WINDOW_H
#define UI_TX_CONTROL_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TxControlWindow
{
public:

    void setupUi(QWidget *TxControlWindow)
    {
        if (TxControlWindow->objectName().isEmpty())
            TxControlWindow->setObjectName(QStringLiteral("TxControlWindow"));
        TxControlWindow->resize(400, 300);

        retranslateUi(TxControlWindow);

        QMetaObject::connectSlotsByName(TxControlWindow);
    } // setupUi

    void retranslateUi(QWidget *TxControlWindow)
    {
        TxControlWindow->setWindowTitle(QApplication::translate("TxControlWindow", "TxControlWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class TxControlWindow: public Ui_TxControlWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TX_CONTROL_WINDOW_H
