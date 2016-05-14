/********************************************************************************
** Form generated from reading UI file 'virtual_terminal_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIRTUAL_TERMINAL_WIDGET_H
#define UI_VIRTUAL_TERMINAL_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VirtualTerminalWidget
{
public:
    QAction *cutFileAction;
    QAction *stopReceiveVideoAction;
    QHBoxLayout *horizontalLayout;
    QLabel *videoShowedLabel;

    void setupUi(QWidget *VirtualTerminalWidget)
    {
        if (VirtualTerminalWidget->objectName().isEmpty())
            VirtualTerminalWidget->setObjectName(QStringLiteral("VirtualTerminalWidget"));
        VirtualTerminalWidget->resize(94, 32);
        VirtualTerminalWidget->setMinimumSize(QSize(32, 32));
        VirtualTerminalWidget->setMaximumSize(QSize(94, 32));
        VirtualTerminalWidget->setStyleSheet(QLatin1String("QFrame {\n"
"  border-width: 0px\n"
"}"));
        cutFileAction = new QAction(VirtualTerminalWidget);
        cutFileAction->setObjectName(QStringLiteral("cutFileAction"));
        stopReceiveVideoAction = new QAction(VirtualTerminalWidget);
        stopReceiveVideoAction->setObjectName(QStringLiteral("stopReceiveVideoAction"));
        horizontalLayout = new QHBoxLayout(VirtualTerminalWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        videoShowedLabel = new QLabel(VirtualTerminalWidget);
        videoShowedLabel->setObjectName(QStringLiteral("videoShowedLabel"));
        videoShowedLabel->setMinimumSize(QSize(32, 32));
        videoShowedLabel->setMaximumSize(QSize(32, 32));
        videoShowedLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(videoShowedLabel);


        retranslateUi(VirtualTerminalWidget);

        QMetaObject::connectSlotsByName(VirtualTerminalWidget);
    } // setupUi

    void retranslateUi(QWidget *VirtualTerminalWidget)
    {
        VirtualTerminalWidget->setWindowTitle(QApplication::translate("VirtualTerminalWidget", "VirtualTerminalWidget", 0));
        cutFileAction->setText(QApplication::translate("VirtualTerminalWidget", "\345\275\225\345\210\266\345\210\206\346\256\265", 0));
#ifndef QT_NO_TOOLTIP
        cutFileAction->setToolTip(QApplication::translate("VirtualTerminalWidget", "\345\275\225\345\210\266\345\210\206\346\256\265", 0));
#endif // QT_NO_TOOLTIP
        stopReceiveVideoAction->setText(QApplication::translate("VirtualTerminalWidget", "\345\201\234\346\255\242\346\216\245\346\224\266\350\247\206\351\242\221", 0));
#ifndef QT_NO_TOOLTIP
        stopReceiveVideoAction->setToolTip(QApplication::translate("VirtualTerminalWidget", "\345\201\234\346\255\242\346\216\245\346\224\266\350\247\206\351\242\221", 0));
#endif // QT_NO_TOOLTIP
        videoShowedLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VirtualTerminalWidget: public Ui_VirtualTerminalWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIRTUAL_TERMINAL_WIDGET_H
