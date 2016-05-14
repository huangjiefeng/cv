/********************************************************************************
** Form generated from reading UI file 'conference_list_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFERENCE_LIST_WIDGET_H
#define UI_CONFERENCE_LIST_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConferenceListWidget
{
public:
    QAction *enterConferenceAction;
    QAction *actionHelp;
    QAction *updateConferenceAction;
    QHBoxLayout *horizontalLayout_2;
    QToolBar *mainToolBar;
    QListWidget *listWidget;
    QWidget *conferenceInfoWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *confNameLabel;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_13;
    QLabel *label;
    QLabel *joinModeLabel;
    QLabel *confDateLabel;
    QLabel *label_5;
    QLabel *confStartTimeLabel;
    QLabel *confDurationLabel;
    QLabel *isMulticast;
    QLabel *label_4;
    QLabel *multicastAddress;
    QLabel *controlModeLabel;
    QLabel *label_17;
    QLabel *confLevelLabel;

    void setupUi(QWidget *ConferenceListWidget)
    {
        if (ConferenceListWidget->objectName().isEmpty())
            ConferenceListWidget->setObjectName(QStringLiteral("ConferenceListWidget"));
        ConferenceListWidget->resize(956, 220);
        ConferenceListWidget->setMinimumSize(QSize(0, 220));
        ConferenceListWidget->setStyleSheet(QStringLiteral(""));
        enterConferenceAction = new QAction(ConferenceListWidget);
        enterConferenceAction->setObjectName(QStringLiteral("enterConferenceAction"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/right-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        enterConferenceAction->setIcon(icon);
        actionHelp = new QAction(ConferenceListWidget);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/computer_help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon1);
        updateConferenceAction = new QAction(ConferenceListWidget);
        updateConferenceAction->setObjectName(QStringLiteral("updateConferenceAction"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/update.png"), QSize(), QIcon::Normal, QIcon::Off);
        updateConferenceAction->setIcon(icon2);
        horizontalLayout_2 = new QHBoxLayout(ConferenceListWidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        mainToolBar = new QToolBar(ConferenceListWidget);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy);
        mainToolBar->setMinimumSize(QSize(0, 0));
        mainToolBar->setMaximumSize(QSize(50, 16777215));
        mainToolBar->setLayoutDirection(Qt::RightToLeft);
        mainToolBar->setAutoFillBackground(false);
        mainToolBar->setMovable(false);
        mainToolBar->setOrientation(Qt::Vertical);
        mainToolBar->setIconSize(QSize(48, 48));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar->setFloatable(true);

        horizontalLayout_2->addWidget(mainToolBar);

        listWidget = new QListWidget(ConferenceListWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setAlternatingRowColors(true);

        horizontalLayout_2->addWidget(listWidget);

        conferenceInfoWidget = new QWidget(ConferenceListWidget);
        conferenceInfoWidget->setObjectName(QStringLiteral("conferenceInfoWidget"));
        verticalLayout_3 = new QVBoxLayout(conferenceInfoWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        confNameLabel = new QLabel(conferenceInfoWidget);
        confNameLabel->setObjectName(QStringLiteral("confNameLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(confNameLabel->sizePolicy().hasHeightForWidth());
        confNameLabel->setSizePolicy(sizePolicy2);
        confNameLabel->setMinimumSize(QSize(0, 40));
        confNameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(confNameLabel);

        widget = new QWidget(conferenceInfoWidget);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(600, 0));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_13, 6, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 1, 3, 1, 1);

        joinModeLabel = new QLabel(groupBox);
        joinModeLabel->setObjectName(QStringLiteral("joinModeLabel"));

        gridLayout->addWidget(joinModeLabel, 3, 1, 1, 1);

        confDateLabel = new QLabel(groupBox);
        confDateLabel->setObjectName(QStringLiteral("confDateLabel"));

        gridLayout->addWidget(confDateLabel, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_5, 0, 3, 1, 1);

        confStartTimeLabel = new QLabel(groupBox);
        confStartTimeLabel->setObjectName(QStringLiteral("confStartTimeLabel"));

        gridLayout->addWidget(confStartTimeLabel, 1, 1, 1, 1);

        confDurationLabel = new QLabel(groupBox);
        confDurationLabel->setObjectName(QStringLiteral("confDurationLabel"));

        gridLayout->addWidget(confDurationLabel, 1, 4, 1, 1);

        isMulticast = new QLabel(groupBox);
        isMulticast->setObjectName(QStringLiteral("isMulticast"));

        gridLayout->addWidget(isMulticast, 0, 4, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 3, 3, 1, 1);

        multicastAddress = new QLabel(groupBox);
        multicastAddress->setObjectName(QStringLiteral("multicastAddress"));

        gridLayout->addWidget(multicastAddress, 3, 4, 1, 1);

        controlModeLabel = new QLabel(groupBox);
        controlModeLabel->setObjectName(QStringLiteral("controlModeLabel"));

        gridLayout->addWidget(controlModeLabel, 6, 1, 1, 1);

        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QStringLiteral("label_17"));
        sizePolicy.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_17, 6, 3, 1, 1);

        confLevelLabel = new QLabel(groupBox);
        confLevelLabel->setObjectName(QStringLiteral("confLevelLabel"));

        gridLayout->addWidget(confLevelLabel, 6, 4, 1, 1);


        horizontalLayout->addWidget(groupBox);


        verticalLayout_3->addWidget(widget);


        horizontalLayout_2->addWidget(conferenceInfoWidget);


        mainToolBar->addAction(enterConferenceAction);
        mainToolBar->addAction(updateConferenceAction);
        mainToolBar->addAction(actionHelp);

        retranslateUi(ConferenceListWidget);

        QMetaObject::connectSlotsByName(ConferenceListWidget);
    } // setupUi

    void retranslateUi(QWidget *ConferenceListWidget)
    {
        ConferenceListWidget->setWindowTitle(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\345\210\227\350\241\250", 0));
        enterConferenceAction->setText(QApplication::translate("ConferenceListWidget", "\350\277\233\345\205\245\344\274\232\350\256\256", 0));
#ifndef QT_NO_TOOLTIP
        enterConferenceAction->setToolTip(QApplication::translate("ConferenceListWidget", "\350\277\233\345\205\245\344\274\232\350\256\256", 0));
#endif // QT_NO_TOOLTIP
        actionHelp->setText(QApplication::translate("ConferenceListWidget", "\345\205\263\344\272\216CoolView", 0));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("ConferenceListWidget", "\345\205\263\344\272\216CoolView", 0));
#endif // QT_NO_TOOLTIP
        updateConferenceAction->setText(QApplication::translate("ConferenceListWidget", "\345\210\267\346\226\260\344\274\232\350\256\256\345\210\227\350\241\250", 0));
#ifndef QT_NO_TOOLTIP
        updateConferenceAction->setToolTip(QApplication::translate("ConferenceListWidget", "\345\210\267\346\226\260\344\274\232\350\256\256\345\210\227\350\241\250", 0));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\350\257\246\346\203\205", 0));
        label_6->setText(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\346\227\245\346\234\237\357\274\232", 0));
        label_2->setText(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\347\261\273\345\236\213\357\274\232", 0));
        label_3->setText(QApplication::translate("ConferenceListWidget", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0));
        label_13->setText(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\346\250\241\345\274\217\357\274\232", 0));
        label->setText(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\346\227\266\351\225\277\357\274\232", 0));
        joinModeLabel->setText(QString());
        confDateLabel->setText(QString());
        label_5->setText(QApplication::translate("ConferenceListWidget", "\346\230\257\345\220\246\347\273\204\346\222\255\357\274\232", 0));
        confStartTimeLabel->setText(QString());
        confDurationLabel->setText(QString());
        isMulticast->setText(QString());
        label_4->setText(QApplication::translate("ConferenceListWidget", "\347\273\204\346\222\255\345\234\260\345\235\200\357\274\232", 0));
        multicastAddress->setText(QString());
        controlModeLabel->setText(QString());
        label_17->setText(QApplication::translate("ConferenceListWidget", "\344\274\232\350\256\256\347\272\247\345\210\253\357\274\232", 0));
        confLevelLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConferenceListWidget: public Ui_ConferenceListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFERENCE_LIST_WIDGET_H
