/********************************************************************************
** Form generated from reading UI file 'local_speaker_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCAL_SPEAKER_WIDGET_H
#define UI_LOCAL_SPEAKER_WIDGET_H

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

class Ui_LocalSpeakerWidget
{
public:
    QGridLayout *gridLayout;
    QToolButton *speakerVolumeButton;
    QLabel *speakerNameLabel;

    void setupUi(QWidget *LocalSpeakerWidget)
    {
        if (LocalSpeakerWidget->objectName().isEmpty())
            LocalSpeakerWidget->setObjectName(QStringLiteral("LocalSpeakerWidget"));
        LocalSpeakerWidget->resize(98, 116);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LocalSpeakerWidget->sizePolicy().hasHeightForWidth());
        LocalSpeakerWidget->setSizePolicy(sizePolicy);
        LocalSpeakerWidget->setMinimumSize(QSize(98, 116));
        LocalSpeakerWidget->setMaximumSize(QSize(98, 116));
        gridLayout = new QGridLayout(LocalSpeakerWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        speakerVolumeButton = new QToolButton(LocalSpeakerWidget);
        speakerVolumeButton->setObjectName(QStringLiteral("speakerVolumeButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/volume.png"), QSize(), QIcon::Normal, QIcon::Off);
        speakerVolumeButton->setIcon(icon);
        speakerVolumeButton->setIconSize(QSize(42, 42));
        speakerVolumeButton->setCheckable(true);
        speakerVolumeButton->setPopupMode(QToolButton::InstantPopup);

        gridLayout->addWidget(speakerVolumeButton, 0, 0, 1, 1);

        speakerNameLabel = new QLabel(LocalSpeakerWidget);
        speakerNameLabel->setObjectName(QStringLiteral("speakerNameLabel"));
        speakerNameLabel->setScaledContents(false);
        speakerNameLabel->setAlignment(Qt::AlignCenter);
        speakerNameLabel->setWordWrap(true);

        gridLayout->addWidget(speakerNameLabel, 1, 0, 1, 1);


        retranslateUi(LocalSpeakerWidget);

        QMetaObject::connectSlotsByName(LocalSpeakerWidget);
    } // setupUi

    void retranslateUi(QWidget *LocalSpeakerWidget)
    {
        LocalSpeakerWidget->setWindowTitle(QApplication::translate("LocalSpeakerWidget", "LocalSpeakerWidget", 0));
        speakerVolumeButton->setText(QApplication::translate("LocalSpeakerWidget", "...", 0));
        speakerNameLabel->setText(QApplication::translate("LocalSpeakerWidget", "\346\211\254\345\243\260\345\231\250", 0));
    } // retranslateUi

};

namespace Ui {
    class LocalSpeakerWidget: public Ui_LocalSpeakerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCAL_SPEAKER_WIDGET_H
