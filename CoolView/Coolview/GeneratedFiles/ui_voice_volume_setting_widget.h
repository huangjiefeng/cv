/********************************************************************************
** Form generated from reading UI file 'voice_volume_setting_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOICE_VOLUME_SETTING_WIDGET_H
#define UI_VOICE_VOLUME_SETTING_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VoiceVolumeSettingWidget
{
public:
    QGridLayout *gridLayout;
    QSlider *voiceSlider;
    QLabel *voiceLabel;

    void setupUi(QWidget *VoiceVolumeSettingWidget)
    {
        if (VoiceVolumeSettingWidget->objectName().isEmpty())
            VoiceVolumeSettingWidget->setObjectName(QStringLiteral("VoiceVolumeSettingWidget"));
        VoiceVolumeSettingWidget->resize(94, 183);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VoiceVolumeSettingWidget->sizePolicy().hasHeightForWidth());
        VoiceVolumeSettingWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(VoiceVolumeSettingWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        voiceSlider = new QSlider(VoiceVolumeSettingWidget);
        voiceSlider->setObjectName(QStringLiteral("voiceSlider"));
        voiceSlider->setOrientation(Qt::Vertical);
        voiceSlider->setInvertedAppearance(false);
        voiceSlider->setInvertedControls(false);
        voiceSlider->setTickPosition(QSlider::TicksBothSides);
        voiceSlider->setTickInterval(0);

        gridLayout->addWidget(voiceSlider, 0, 0, 1, 1);

        voiceLabel = new QLabel(VoiceVolumeSettingWidget);
        voiceLabel->setObjectName(QStringLiteral("voiceLabel"));
        voiceLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(voiceLabel, 1, 0, 1, 1);


        retranslateUi(VoiceVolumeSettingWidget);

        QMetaObject::connectSlotsByName(VoiceVolumeSettingWidget);
    } // setupUi

    void retranslateUi(QWidget *VoiceVolumeSettingWidget)
    {
        VoiceVolumeSettingWidget->setWindowTitle(QApplication::translate("VoiceVolumeSettingWidget", "\351\237\263\351\207\217\350\256\276\347\275\256", 0));
        voiceLabel->setText(QApplication::translate("VoiceVolumeSettingWidget", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class VoiceVolumeSettingWidget: public Ui_VoiceVolumeSettingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOICE_VOLUME_SETTING_WIDGET_H
