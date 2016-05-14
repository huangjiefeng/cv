/********************************************************************************
** Form generated from reading UI file 'audio_setting_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIO_SETTING_TAB_H
#define UI_AUDIO_SETTING_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AudioSettingTab
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *audioFrame;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *inputGroupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_9;
    QComboBox *inputComboBox_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_10;
    QSlider *inputVolumnSlider;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QComboBox *audioCodecComboBox;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_11;
    QComboBox *audioSampleRateComboBox;
    QLabel *label_22;
    QComboBox *audioChannelComboBox;
    QGroupBox *outputGroupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_17;
    QComboBox *outputComboBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_18;
    QSlider *outputVolumnSlider;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QComboBox *aecComboBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton;

    void setupUi(QWidget *AudioSettingTab)
    {
        if (AudioSettingTab->objectName().isEmpty())
            AudioSettingTab->setObjectName(QStringLiteral("AudioSettingTab"));
        AudioSettingTab->resize(553, 305);
        AudioSettingTab->setMaximumSize(QSize(600, 16777215));
        verticalLayout = new QVBoxLayout(AudioSettingTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        audioFrame = new QFrame(AudioSettingTab);
        audioFrame->setObjectName(QStringLiteral("audioFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(audioFrame->sizePolicy().hasHeightForWidth());
        audioFrame->setSizePolicy(sizePolicy);
        audioFrame->setFrameShape(QFrame::StyledPanel);
        audioFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(audioFrame);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        inputGroupBox = new QGroupBox(audioFrame);
        inputGroupBox->setObjectName(QStringLiteral("inputGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inputGroupBox->sizePolicy().hasHeightForWidth());
        inputGroupBox->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(9);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        inputGroupBox->setFont(font);
        verticalLayout_3 = new QVBoxLayout(inputGroupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_9 = new QLabel(inputGroupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);

        horizontalLayout_19->addWidget(label_9);

        inputComboBox_2 = new QComboBox(inputGroupBox);
        inputComboBox_2->setObjectName(QStringLiteral("inputComboBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(inputComboBox_2->sizePolicy().hasHeightForWidth());
        inputComboBox_2->setSizePolicy(sizePolicy2);

        horizontalLayout_19->addWidget(inputComboBox_2);


        verticalLayout_3->addLayout(horizontalLayout_19);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_10 = new QLabel(inputGroupBox);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font);

        horizontalLayout->addWidget(label_10);

        inputVolumnSlider = new QSlider(inputGroupBox);
        inputVolumnSlider->setObjectName(QStringLiteral("inputVolumnSlider"));
        inputVolumnSlider->setMaximumSize(QSize(16777215, 16777215));
        inputVolumnSlider->setOrientation(Qt::Horizontal);
        inputVolumnSlider->setTickPosition(QSlider::NoTicks);

        horizontalLayout->addWidget(inputVolumnSlider);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(inputGroupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_4->addWidget(label_6);

        audioCodecComboBox = new QComboBox(inputGroupBox);
        audioCodecComboBox->setObjectName(QStringLiteral("audioCodecComboBox"));
        sizePolicy2.setHeightForWidth(audioCodecComboBox->sizePolicy().hasHeightForWidth());
        audioCodecComboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(audioCodecComboBox);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_11 = new QLabel(inputGroupBox);
        label_11->setObjectName(QStringLiteral("label_11"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy3);
        label_11->setMinimumSize(QSize(52, 0));

        horizontalLayout_9->addWidget(label_11);

        audioSampleRateComboBox = new QComboBox(inputGroupBox);
        audioSampleRateComboBox->setObjectName(QStringLiteral("audioSampleRateComboBox"));
        sizePolicy2.setHeightForWidth(audioSampleRateComboBox->sizePolicy().hasHeightForWidth());
        audioSampleRateComboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_9->addWidget(audioSampleRateComboBox);

        label_22 = new QLabel(inputGroupBox);
        label_22->setObjectName(QStringLiteral("label_22"));

        horizontalLayout_9->addWidget(label_22);

        audioChannelComboBox = new QComboBox(inputGroupBox);
        audioChannelComboBox->setObjectName(QStringLiteral("audioChannelComboBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(audioChannelComboBox->sizePolicy().hasHeightForWidth());
        audioChannelComboBox->setSizePolicy(sizePolicy4);
        audioChannelComboBox->setMinimumSize(QSize(40, 0));

        horizontalLayout_9->addWidget(audioChannelComboBox);


        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_5->addWidget(inputGroupBox);

        outputGroupBox = new QGroupBox(audioFrame);
        outputGroupBox->setObjectName(QStringLiteral("outputGroupBox"));
        sizePolicy1.setHeightForWidth(outputGroupBox->sizePolicy().hasHeightForWidth());
        outputGroupBox->setSizePolicy(sizePolicy1);
        outputGroupBox->setStyleSheet(QStringLiteral(""));
        verticalLayout_4 = new QVBoxLayout(outputGroupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_17 = new QLabel(outputGroupBox);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_2->addWidget(label_17);

        outputComboBox_2 = new QComboBox(outputGroupBox);
        outputComboBox_2->setObjectName(QStringLiteral("outputComboBox_2"));
        sizePolicy2.setHeightForWidth(outputComboBox_2->sizePolicy().hasHeightForWidth());
        outputComboBox_2->setSizePolicy(sizePolicy2);
        outputComboBox_2->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(outputComboBox_2);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_18 = new QLabel(outputGroupBox);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_3->addWidget(label_18);

        outputVolumnSlider = new QSlider(outputGroupBox);
        outputVolumnSlider->setObjectName(QStringLiteral("outputVolumnSlider"));
        outputVolumnSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(outputVolumnSlider);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        horizontalLayout_5->addWidget(outputGroupBox);


        verticalLayout->addWidget(audioFrame);

        groupBox = new QGroupBox(AudioSettingTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy5);

        horizontalLayout_6->addWidget(label);

        aecComboBox = new QComboBox(groupBox);
        aecComboBox->setObjectName(QStringLiteral("aecComboBox"));
        sizePolicy4.setHeightForWidth(aecComboBox->sizePolicy().hasHeightForWidth());
        aecComboBox->setSizePolicy(sizePolicy4);
        aecComboBox->setMinimumSize(QSize(150, 0));

        horizontalLayout_6->addWidget(aecComboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        saveButton = new QPushButton(AudioSettingTab);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setMinimumSize(QSize(80, 30));
        saveButton->setMaximumSize(QSize(80, 30));

        horizontalLayout_7->addWidget(saveButton);


        verticalLayout->addLayout(horizontalLayout_7);


        retranslateUi(AudioSettingTab);

        QMetaObject::connectSlotsByName(AudioSettingTab);
    } // setupUi

    void retranslateUi(QWidget *AudioSettingTab)
    {
        AudioSettingTab->setWindowTitle(QApplication::translate("AudioSettingTab", "\351\237\263\351\242\221\350\256\276\345\244\207\350\256\276\347\275\256", 0));
        inputGroupBox->setTitle(QApplication::translate("AudioSettingTab", "\345\243\260\351\237\263\350\276\223\345\205\245\350\256\276\347\275\256", 0));
        label_9->setText(QApplication::translate("AudioSettingTab", "\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_10->setText(QApplication::translate("AudioSettingTab", "\351\237\263\351\207\217\346\216\247\345\210\266", 0));
        label_6->setText(QApplication::translate("AudioSettingTab", "\347\274\226\347\240\201\346\226\271\345\274\217", 0));
        label_11->setText(QApplication::translate("AudioSettingTab", "\351\207\207\346\240\267\347\216\207", 0));
        label_22->setText(QApplication::translate("AudioSettingTab", "\345\243\260\351\201\223", 0));
        outputGroupBox->setTitle(QApplication::translate("AudioSettingTab", "\345\243\260\351\237\263\350\276\223\345\207\272\350\256\276\347\275\256", 0));
        label_17->setText(QApplication::translate("AudioSettingTab", "\350\276\223\345\207\272\350\256\276\345\244\207", 0));
        label_18->setText(QApplication::translate("AudioSettingTab", "\351\237\263\351\207\217\346\216\247\345\210\266", 0));
        groupBox->setTitle(QApplication::translate("AudioSettingTab", "\351\253\230\347\272\247\351\200\211\351\241\271", 0));
        label->setText(QApplication::translate("AudioSettingTab", "\345\233\236\345\243\260\346\266\210\351\231\244", 0));
        saveButton->setText(QApplication::translate("AudioSettingTab", "\344\277\235\345\255\230", 0));
    } // retranslateUi

};

namespace Ui {
    class AudioSettingTab: public Ui_AudioSettingTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIO_SETTING_TAB_H
