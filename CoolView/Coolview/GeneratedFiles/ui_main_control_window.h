/********************************************************************************
** Form generated from reading UI file 'main_control_window.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_CONTROL_WINDOW_H
#define UI_MAIN_CONTROL_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainControlWindow
{
public:
    QAction *actionHelp;
    QAction *actionLogOut;
    QAction *actionQuit;
    QAction *actionRestart;
    QAction *actionDevice;
    QAction *actionConference;
    QAction *actionMouseSim;
    QAction *SaviAction;
    QAction *actionWebSite;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QScrollArea *memberVideoScrollArea;
    QWidget *conferenceContents;
    QVBoxLayout *verticalLayout;

    void setupUi(QMainWindow *MainControlWindow)
    {
        if (MainControlWindow->objectName().isEmpty())
            MainControlWindow->setObjectName(QStringLiteral("MainControlWindow"));
        MainControlWindow->resize(592, 527);
        MainControlWindow->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Resources/application.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainControlWindow->setWindowIcon(icon);
        actionHelp = new QAction(MainControlWindow);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/computer_help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon1);
        actionLogOut = new QAction(MainControlWindow);
        actionLogOut->setObjectName(QStringLiteral("actionLogOut"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/unlock.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLogOut->setIcon(icon2);
        actionQuit = new QAction(MainControlWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon3);
        actionQuit->setIconVisibleInMenu(false);
        actionRestart = new QAction(MainControlWindow);
        actionRestart->setObjectName(QStringLiteral("actionRestart"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/restart.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRestart->setIcon(icon4);
        actionDevice = new QAction(MainControlWindow);
        actionDevice->setObjectName(QStringLiteral("actionDevice"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/tools.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDevice->setIcon(icon5);
        actionConference = new QAction(MainControlWindow);
        actionConference->setObjectName(QStringLiteral("actionConference"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/image/users_chat.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConference->setIcon(icon6);
        actionConference->setAutoRepeat(false);
        actionConference->setMenuRole(QAction::TextHeuristicRole);
        actionConference->setIconVisibleInMenu(false);
        actionMouseSim = new QAction(MainControlWindow);
        actionMouseSim->setObjectName(QStringLiteral("actionMouseSim"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/image/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMouseSim->setIcon(icon7);
        SaviAction = new QAction(MainControlWindow);
        SaviAction->setObjectName(QStringLiteral("SaviAction"));
        actionWebSite = new QAction(MainControlWindow);
        actionWebSite->setObjectName(QStringLiteral("actionWebSite"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/image/home.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWebSite->setIcon(icon8);
        actionWebSite->setAutoRepeat(false);
        actionWebSite->setMenuRole(QAction::TextHeuristicRole);
        actionWebSite->setIconVisibleInMenu(false);
        centralWidget = new QWidget(MainControlWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        memberVideoScrollArea = new QScrollArea(centralWidget);
        memberVideoScrollArea->setObjectName(QStringLiteral("memberVideoScrollArea"));
        memberVideoScrollArea->setAcceptDrops(false);
        memberVideoScrollArea->setFrameShape(QFrame::NoFrame);
        memberVideoScrollArea->setWidgetResizable(true);
        conferenceContents = new QWidget();
        conferenceContents->setObjectName(QStringLiteral("conferenceContents"));
        conferenceContents->setGeometry(QRect(0, 0, 592, 527));
        conferenceContents->setAcceptDrops(false);
        verticalLayout = new QVBoxLayout(conferenceContents);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        memberVideoScrollArea->setWidget(conferenceContents);

        horizontalLayout->addWidget(memberVideoScrollArea);

        MainControlWindow->setCentralWidget(centralWidget);

        retranslateUi(MainControlWindow);

        QMetaObject::connectSlotsByName(MainControlWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainControlWindow)
    {
        MainControlWindow->setWindowTitle(QApplication::translate("MainControlWindow", "MainControlWindow", 0));
        actionHelp->setText(QApplication::translate("MainControlWindow", "\345\205\263\344\272\216CoolView", 0));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("MainControlWindow", "\345\205\263\344\272\216CoolView", 0));
#endif // QT_NO_TOOLTIP
        actionLogOut->setText(QApplication::translate("MainControlWindow", "\346\263\250\351\224\200", 0));
#ifndef QT_NO_TOOLTIP
        actionLogOut->setToolTip(QApplication::translate("MainControlWindow", "\346\263\250\351\224\200", 0));
#endif // QT_NO_TOOLTIP
        actionQuit->setText(QApplication::translate("MainControlWindow", "\351\200\200\345\207\272", 0));
        actionRestart->setText(QApplication::translate("MainControlWindow", "\351\207\215\345\220\257\350\256\276\345\244\207", 0));
#ifndef QT_NO_TOOLTIP
        actionRestart->setToolTip(QApplication::translate("MainControlWindow", "\351\207\215\345\220\257\350\256\276\345\244\207", 0));
#endif // QT_NO_TOOLTIP
        actionDevice->setText(QApplication::translate("MainControlWindow", "\347\273\210\347\253\257\350\256\276\347\275\256", 0));
#ifndef QT_NO_TOOLTIP
        actionDevice->setToolTip(QApplication::translate("MainControlWindow", "\347\273\210\347\253\257\350\256\276\347\275\256", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionDevice->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        actionDevice->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        actionConference->setText(QApplication::translate("MainControlWindow", "\344\274\232\350\256\256\345\210\227\350\241\250", 0));
        actionMouseSim->setText(QApplication::translate("MainControlWindow", "mouseSim", 0));
        SaviAction->setText(QString());
        actionWebSite->setText(QApplication::translate("MainControlWindow", "\344\274\232\350\256\256\347\256\241\347\220\206", 0));
#ifndef QT_NO_TOOLTIP
        actionWebSite->setToolTip(QApplication::translate("MainControlWindow", "\344\274\232\350\256\256\347\256\241\347\220\206", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainControlWindow: public Ui_MainControlWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_CONTROL_WINDOW_H
