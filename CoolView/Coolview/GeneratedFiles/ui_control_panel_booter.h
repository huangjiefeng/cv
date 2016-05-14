/********************************************************************************
** Form generated from reading UI file 'control_panel_booter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROL_PANEL_BOOTER_H
#define UI_CONTROL_PANEL_BOOTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanelBooter
{
public:

    void setupUi(QWidget *ControlPanelBooter)
    {
        if (ControlPanelBooter->objectName().isEmpty())
            ControlPanelBooter->setObjectName(QStringLiteral("ControlPanelBooter"));
        ControlPanelBooter->resize(128, 128);

        retranslateUi(ControlPanelBooter);

        QMetaObject::connectSlotsByName(ControlPanelBooter);
    } // setupUi

    void retranslateUi(QWidget *ControlPanelBooter)
    {
        ControlPanelBooter->setWindowTitle(QApplication::translate("ControlPanelBooter", "ControlPanelBooter", 0));
    } // retranslateUi

};

namespace Ui {
    class ControlPanelBooter: public Ui_ControlPanelBooter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROL_PANEL_BOOTER_H
