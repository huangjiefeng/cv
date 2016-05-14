/********************************************************************************
** Form generated from reading UI file 'local_mic_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCAL_MIC_WIDGET_H
#define UI_LOCAL_MIC_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LocalMicWidget
{
public:
    QGridLayout *gridLayout;
    QToolButton *micVolumeButton;
    QLabel *micNameLabel;

    void setupUi(QWidget *LocalMicWidget)
    {
        if (LocalMicWidget->objectName().isEmpty())
            LocalMicWidget->setObjectName(QStringLiteral("LocalMicWidget"));
        LocalMicWidget->resize(98, 116);
        LocalMicWidget->setMinimumSize(QSize(98, 116));
        LocalMicWidget->setMaximumSize(QSize(98, 116));
        gridLayout = new QGridLayout(LocalMicWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        micVolumeButton = new QToolButton(LocalMicWidget);
        micVolumeButton->setObjectName(QStringLiteral("micVolumeButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/mic.png"), QSize(), QIcon::Normal, QIcon::Off);
        micVolumeButton->setIcon(icon);
        micVolumeButton->setIconSize(QSize(64, 64));
        micVolumeButton->setCheckable(true);
        micVolumeButton->setPopupMode(QToolButton::InstantPopup);

        gridLayout->addWidget(micVolumeButton, 0, 0, 1, 1);

        micNameLabel = new QLabel(LocalMicWidget);
        micNameLabel->setObjectName(QStringLiteral("micNameLabel"));
        micNameLabel->setScaledContents(false);
        micNameLabel->setAlignment(Qt::AlignCenter);
        micNameLabel->setWordWrap(true);

        gridLayout->addWidget(micNameLabel, 1, 0, 1, 1);


        retranslateUi(LocalMicWidget);

        QMetaObject::connectSlotsByName(LocalMicWidget);
    } // setupUi

    void retranslateUi(QWidget *LocalMicWidget)
    {
        LocalMicWidget->setWindowTitle(QApplication::translate("LocalMicWidget", "LocalMicWidget", 0));
        micVolumeButton->setText(QApplication::translate("LocalMicWidget", "...", 0));
        micNameLabel->setText(QApplication::translate("LocalMicWidget", "\351\272\246\345\205\213\351\243\216", 0));
    } // retranslateUi

};

namespace Ui {
    class LocalMicWidget: public Ui_LocalMicWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCAL_MIC_WIDGET_H
