/********************************************************************************
** Form generated from reading UI file 'pending_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PENDING_WIDGET_H
#define UI_PENDING_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PendingWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *msgLabel;

    void setupUi(QWidget *PendingWidget)
    {
        if (PendingWidget->objectName().isEmpty())
            PendingWidget->setObjectName(QStringLiteral("PendingWidget"));
        PendingWidget->resize(233, 80);
        verticalLayout = new QVBoxLayout(PendingWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        msgLabel = new QLabel(PendingWidget);
        msgLabel->setObjectName(QStringLiteral("msgLabel"));
        msgLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(msgLabel);


        retranslateUi(PendingWidget);

        QMetaObject::connectSlotsByName(PendingWidget);
    } // setupUi

    void retranslateUi(QWidget *PendingWidget)
    {
        PendingWidget->setWindowTitle(QApplication::translate("PendingWidget", "Form", 0));
        msgLabel->setText(QApplication::translate("PendingWidget", "\346\255\243\345\234\250\345\210\235\345\247\213\345\214\226\357\274\214\350\257\267\347\250\215\345\220\216...", 0));
    } // retranslateUi

};

namespace Ui {
    class PendingWidget: public Ui_PendingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PENDING_WIDGET_H
