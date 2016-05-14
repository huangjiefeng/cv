/********************************************************************************
** Form generated from reading UI file 'record_status_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORD_STATUS_WIDGET_H
#define UI_RECORD_STATUS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecordStatusWidgetClass
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *TerminalTable;
    QTextBrowser *statBrowser;

    void setupUi(QWidget *RecordStatusWidgetClass)
    {
        if (RecordStatusWidgetClass->objectName().isEmpty())
            RecordStatusWidgetClass->setObjectName(QStringLiteral("RecordStatusWidgetClass"));
        RecordStatusWidgetClass->resize(688, 472);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecordStatusWidgetClass->sizePolicy().hasHeightForWidth());
        RecordStatusWidgetClass->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(RecordStatusWidgetClass);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        TerminalTable = new QTableWidget(RecordStatusWidgetClass);
        TerminalTable->setObjectName(QStringLiteral("TerminalTable"));

        verticalLayout->addWidget(TerminalTable);

        statBrowser = new QTextBrowser(RecordStatusWidgetClass);
        statBrowser->setObjectName(QStringLiteral("statBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(statBrowser->sizePolicy().hasHeightForWidth());
        statBrowser->setSizePolicy(sizePolicy1);
        statBrowser->setMinimumSize(QSize(0, 100));
        statBrowser->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(statBrowser);


        retranslateUi(RecordStatusWidgetClass);

        QMetaObject::connectSlotsByName(RecordStatusWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *RecordStatusWidgetClass)
    {
        RecordStatusWidgetClass->setWindowTitle(QApplication::translate("RecordStatusWidgetClass", "\345\275\225\345\210\266\347\212\266\346\200\201", 0));
    } // retranslateUi

};

namespace Ui {
    class RecordStatusWidgetClass: public Ui_RecordStatusWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORD_STATUS_WIDGET_H
