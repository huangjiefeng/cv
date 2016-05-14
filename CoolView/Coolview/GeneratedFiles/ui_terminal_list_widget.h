/********************************************************************************
** Form generated from reading UI file 'terminal_list_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERMINAL_LIST_WIDGET_H
#define UI_TERMINAL_LIST_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerminalListWidget
{
public:
    QAction *showAllTerminalsAction;
    QAction *showOnlineTerminalsAction;
    QAction *showOfflineTerminalsAction;
    QAction *leaveConferenceAction;
    QHBoxLayout *horizontalLayout;
    QToolBar *mainToolBar;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *conferenceTitleLabel;
    QScrollArea *memberVideoScrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;

    void setupUi(QWidget *TerminalListWidget)
    {
        if (TerminalListWidget->objectName().isEmpty())
            TerminalListWidget->setObjectName(QStringLiteral("TerminalListWidget"));
        TerminalListWidget->resize(765, 220);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TerminalListWidget->sizePolicy().hasHeightForWidth());
        TerminalListWidget->setSizePolicy(sizePolicy);
        TerminalListWidget->setMinimumSize(QSize(0, 220));
        TerminalListWidget->setAcceptDrops(true);
        showAllTerminalsAction = new QAction(TerminalListWidget);
        showAllTerminalsAction->setObjectName(QStringLiteral("showAllTerminalsAction"));
        showAllTerminalsAction->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/users_all.png"), QSize(), QIcon::Normal, QIcon::Off);
        showAllTerminalsAction->setIcon(icon);
        showOnlineTerminalsAction = new QAction(TerminalListWidget);
        showOnlineTerminalsAction->setObjectName(QStringLiteral("showOnlineTerminalsAction"));
        showOnlineTerminalsAction->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/online.png"), QSize(), QIcon::Normal, QIcon::Off);
        showOnlineTerminalsAction->setIcon(icon1);
        showOfflineTerminalsAction = new QAction(TerminalListWidget);
        showOfflineTerminalsAction->setObjectName(QStringLiteral("showOfflineTerminalsAction"));
        showOfflineTerminalsAction->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/offline.png"), QSize(), QIcon::Normal, QIcon::Off);
        showOfflineTerminalsAction->setIcon(icon2);
        leaveConferenceAction = new QAction(TerminalListWidget);
        leaveConferenceAction->setObjectName(QStringLiteral("leaveConferenceAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/left-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        leaveConferenceAction->setIcon(icon3);
        horizontalLayout = new QHBoxLayout(TerminalListWidget);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        mainToolBar = new QToolBar(TerminalListWidget);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy1);
        mainToolBar->setMinimumSize(QSize(0, 0));
        mainToolBar->setMaximumSize(QSize(50, 16777215));
        mainToolBar->setLayoutDirection(Qt::RightToLeft);
        mainToolBar->setAutoFillBackground(false);
        mainToolBar->setMovable(false);
        mainToolBar->setOrientation(Qt::Vertical);
        mainToolBar->setIconSize(QSize(48, 48));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar->setFloatable(true);

        horizontalLayout->addWidget(mainToolBar);

        widget = new QWidget(TerminalListWidget);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        conferenceTitleLabel = new QLabel(widget);
        conferenceTitleLabel->setObjectName(QStringLiteral("conferenceTitleLabel"));
        conferenceTitleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(conferenceTitleLabel);

        memberVideoScrollArea = new QScrollArea(widget);
        memberVideoScrollArea->setObjectName(QStringLiteral("memberVideoScrollArea"));
        memberVideoScrollArea->setAcceptDrops(false);
        memberVideoScrollArea->setFrameShape(QFrame::NoFrame);
        memberVideoScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        memberVideoScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 714, 208));
        scrollAreaWidgetContents->setAcceptDrops(false);
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        memberVideoScrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(memberVideoScrollArea);


        horizontalLayout->addWidget(widget);


        mainToolBar->addAction(leaveConferenceAction);
        mainToolBar->addAction(showAllTerminalsAction);
        mainToolBar->addAction(showOnlineTerminalsAction);
        mainToolBar->addAction(showOfflineTerminalsAction);

        retranslateUi(TerminalListWidget);

        QMetaObject::connectSlotsByName(TerminalListWidget);
    } // setupUi

    void retranslateUi(QWidget *TerminalListWidget)
    {
        TerminalListWidget->setWindowTitle(QApplication::translate("TerminalListWidget", "\344\274\232\345\234\272", 0));
        showAllTerminalsAction->setText(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\345\205\250\351\203\250\347\273\210\347\253\257", 0));
#ifndef QT_NO_TOOLTIP
        showAllTerminalsAction->setToolTip(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\345\205\250\351\203\250\347\273\210\347\253\257", 0));
#endif // QT_NO_TOOLTIP
        showOnlineTerminalsAction->setText(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\345\234\250\347\272\277\347\273\210\347\253\257", 0));
#ifndef QT_NO_TOOLTIP
        showOnlineTerminalsAction->setToolTip(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\345\234\250\347\272\277\347\273\210\347\253\257", 0));
#endif // QT_NO_TOOLTIP
        showOfflineTerminalsAction->setText(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\347\246\273\347\272\277\347\273\210\347\253\257", 0));
#ifndef QT_NO_TOOLTIP
        showOfflineTerminalsAction->setToolTip(QApplication::translate("TerminalListWidget", "\346\230\276\347\244\272\347\246\273\347\272\277\347\273\210\347\253\257", 0));
#endif // QT_NO_TOOLTIP
        leaveConferenceAction->setText(QApplication::translate("TerminalListWidget", "\347\246\273\345\274\200\344\274\232\350\256\256", 0));
#ifndef QT_NO_TOOLTIP
        leaveConferenceAction->setToolTip(QApplication::translate("TerminalListWidget", "\347\246\273\345\274\200\344\274\232\350\256\256", 0));
#endif // QT_NO_TOOLTIP
        conferenceTitleLabel->setText(QApplication::translate("TerminalListWidget", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class TerminalListWidget: public Ui_TerminalListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERMINAL_LIST_WIDGET_H
