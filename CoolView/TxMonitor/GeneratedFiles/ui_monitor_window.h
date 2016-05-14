/********************************************************************************
** Form generated from reading UI file 'monitor_window.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITOR_WINDOW_H
#define UI_MONITOR_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MonitorWindow
{
public:
    QVBoxLayout *MonitorWindowLayout;
    QTabWidget *tabWidget;
    QLabel *globalStatusLabel;

    void setupUi(QWidget *MonitorWindow)
    {
        if (MonitorWindow->objectName().isEmpty())
            MonitorWindow->setObjectName(QStringLiteral("MonitorWindow"));
        MonitorWindow->resize(400, 300);
        MonitorWindowLayout = new QVBoxLayout(MonitorWindow);
        MonitorWindowLayout->setSpacing(6);
        MonitorWindowLayout->setContentsMargins(11, 11, 11, 11);
        MonitorWindowLayout->setObjectName(QStringLiteral("MonitorWindowLayout"));
        tabWidget = new QTabWidget(MonitorWindow);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));

        MonitorWindowLayout->addWidget(tabWidget);

        globalStatusLabel = new QLabel(MonitorWindow);
        globalStatusLabel->setObjectName(QStringLiteral("globalStatusLabel"));

        MonitorWindowLayout->addWidget(globalStatusLabel);


        retranslateUi(MonitorWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MonitorWindow);
    } // setupUi

    void retranslateUi(QWidget *MonitorWindow)
    {
        MonitorWindow->setWindowTitle(QApplication::translate("MonitorWindow", "MonitorWindow", 0));
        globalStatusLabel->setText(QApplication::translate("MonitorWindow", "\345\205\250\345\261\200\347\212\266\346\200\201", 0));
    } // retranslateUi

};

namespace Ui {
    class MonitorWindow: public Ui_MonitorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITOR_WINDOW_H
