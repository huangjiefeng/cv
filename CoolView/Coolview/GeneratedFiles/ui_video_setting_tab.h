/********************************************************************************
** Form generated from reading UI file 'video_setting_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEO_SETTING_TAB_H
#define UI_VIDEO_SETTING_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
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

class Ui_VideoSettingTab
{
public:
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *videoGroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_24;
    QComboBox *cameraIndexComboBox;
    QLabel *label_7;
    QComboBox *webcamDeviceComboBox;
    QLabel *label_66;
    QComboBox *crossbarlistcomboBox;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_65;
    QComboBox *crossbarclasscomboBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QComboBox *videosizecomboBox;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_67;
    QSlider *videoQualitySlider;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer;
    QLabel *label_68;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *ckbKinectDevice;
    QCheckBox *ckbAdaptiveCodeRate;
    QCheckBox *ckbRecvAutoResync;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *saveButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VideoSettingTab)
    {
        if (VideoSettingTab->objectName().isEmpty())
            VideoSettingTab->setObjectName(QStringLiteral("VideoSettingTab"));
        VideoSettingTab->resize(343, 479);
        frame = new QFrame(VideoSettingTab);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(9, 9, 325, 461));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(325, 0));
        frame->setMaximumSize(QSize(325, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        videoGroupBox = new QGroupBox(frame);
        videoGroupBox->setObjectName(QStringLiteral("videoGroupBox"));
        verticalLayout_2 = new QVBoxLayout(videoGroupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_24 = new QLabel(videoGroupBox);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_12->addWidget(label_24);

        cameraIndexComboBox = new QComboBox(videoGroupBox);
        cameraIndexComboBox->setObjectName(QStringLiteral("cameraIndexComboBox"));

        horizontalLayout_12->addWidget(cameraIndexComboBox);


        verticalLayout_2->addLayout(horizontalLayout_12);

        label_7 = new QLabel(videoGroupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(label_7);

        webcamDeviceComboBox = new QComboBox(videoGroupBox);
        webcamDeviceComboBox->setObjectName(QStringLiteral("webcamDeviceComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(webcamDeviceComboBox->sizePolicy().hasHeightForWidth());
        webcamDeviceComboBox->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(webcamDeviceComboBox);

        label_66 = new QLabel(videoGroupBox);
        label_66->setObjectName(QStringLiteral("label_66"));
        label_66->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_2->addWidget(label_66);

        crossbarlistcomboBox = new QComboBox(videoGroupBox);
        crossbarlistcomboBox->setObjectName(QStringLiteral("crossbarlistcomboBox"));
        crossbarlistcomboBox->setMaximumSize(QSize(400, 16777215));

        verticalLayout_2->addWidget(crossbarlistcomboBox);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_65 = new QLabel(videoGroupBox);
        label_65->setObjectName(QStringLiteral("label_65"));
        label_65->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_8->addWidget(label_65);

        crossbarclasscomboBox = new QComboBox(videoGroupBox);
        crossbarclasscomboBox->setObjectName(QStringLiteral("crossbarclasscomboBox"));
        crossbarclasscomboBox->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_8->addWidget(crossbarclasscomboBox);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_8 = new QLabel(videoGroupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);
        label_8->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_7->addWidget(label_8);

        videosizecomboBox = new QComboBox(videoGroupBox);
        videosizecomboBox->setObjectName(QStringLiteral("videosizecomboBox"));
        videosizecomboBox->setMaximumSize(QSize(300, 16777215));

        horizontalLayout_7->addWidget(videosizecomboBox);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_67 = new QLabel(videoGroupBox);
        label_67->setObjectName(QStringLiteral("label_67"));
        label_67->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_10->addWidget(label_67);

        videoQualitySlider = new QSlider(videoGroupBox);
        videoQualitySlider->setObjectName(QStringLiteral("videoQualitySlider"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(videoQualitySlider->sizePolicy().hasHeightForWidth());
        videoQualitySlider->setSizePolicy(sizePolicy3);
        videoQualitySlider->setMaximumSize(QSize(250, 16777215));
        videoQualitySlider->setMinimum(29);
        videoQualitySlider->setMaximum(32);
        videoQualitySlider->setOrientation(Qt::Horizontal);
        videoQualitySlider->setTickPosition(QSlider::TicksBothSides);
        videoQualitySlider->setTickInterval(1);

        horizontalLayout_10->addWidget(videoQualitySlider);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer);

        label_68 = new QLabel(videoGroupBox);
        label_68->setObjectName(QStringLiteral("label_68"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_68->sizePolicy().hasHeightForWidth());
        label_68->setSizePolicy(sizePolicy4);
        label_68->setMinimumSize(QSize(135, 0));
        label_68->setMaximumSize(QSize(16777215, 16777215));
        label_68->setTextFormat(Qt::PlainText);
        label_68->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        label_68->setIndent(0);

        horizontalLayout_11->addWidget(label_68);


        verticalLayout_2->addLayout(horizontalLayout_11);

        label_66->raise();
        label_7->raise();
        crossbarlistcomboBox->raise();
        webcamDeviceComboBox->raise();

        verticalLayout_3->addWidget(videoGroupBox);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ckbKinectDevice = new QCheckBox(groupBox);
        ckbKinectDevice->setObjectName(QStringLiteral("ckbKinectDevice"));

        verticalLayout->addWidget(ckbKinectDevice);

        ckbAdaptiveCodeRate = new QCheckBox(groupBox);
        ckbAdaptiveCodeRate->setObjectName(QStringLiteral("ckbAdaptiveCodeRate"));

        verticalLayout->addWidget(ckbAdaptiveCodeRate);

        ckbRecvAutoResync = new QCheckBox(groupBox);
        ckbRecvAutoResync->setObjectName(QStringLiteral("ckbRecvAutoResync"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(ckbRecvAutoResync->sizePolicy().hasHeightForWidth());
        ckbRecvAutoResync->setSizePolicy(sizePolicy5);

        verticalLayout->addWidget(ckbRecvAutoResync);


        verticalLayout_3->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        saveButton = new QPushButton(frame);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(saveButton->sizePolicy().hasHeightForWidth());
        saveButton->setSizePolicy(sizePolicy6);
        saveButton->setMinimumSize(QSize(80, 30));
        saveButton->setMaximumSize(QSize(80, 30));

        horizontalLayout->addWidget(saveButton);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        retranslateUi(VideoSettingTab);

        QMetaObject::connectSlotsByName(VideoSettingTab);
    } // setupUi

    void retranslateUi(QWidget *VideoSettingTab)
    {
        VideoSettingTab->setWindowTitle(QApplication::translate("VideoSettingTab", "\350\247\206\351\242\221\350\256\276\345\244\207\350\256\276\347\275\256", 0));
        videoGroupBox->setTitle(QApplication::translate("VideoSettingTab", "\346\221\204\345\203\217\345\244\264\350\256\276\347\275\256", 0));
        label_24->setText(QApplication::translate("VideoSettingTab", "\346\221\204\345\203\217\346\234\272\347\274\226\345\217\267", 0));
        label_7->setText(QApplication::translate("VideoSettingTab", "\351\200\211\346\213\251\350\247\206\351\242\221\350\256\276\345\244\207", 0));
        label_66->setText(QApplication::translate("VideoSettingTab", "\351\253\230\346\270\205\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_65->setText(QApplication::translate("VideoSettingTab", "\351\253\230\346\270\205\350\276\223\345\205\245\347\261\273\345\236\213", 0));
        label_8->setText(QApplication::translate("VideoSettingTab", "\345\217\221\351\200\201\347\232\204\350\247\206\351\242\221\345\244\247\345\260\217", 0));
        label_67->setText(QApplication::translate("VideoSettingTab", "\345\217\221\351\200\201\345\233\276\345\203\217\350\264\250\351\207\217     ", 0));
        label_68->setText(QApplication::translate("VideoSettingTab", "\351\235\236\345\270\270\345\245\275    \345\276\210\345\245\275       \345\245\275     \346\231\256\351\200\232", "8"));
        groupBox->setTitle(QApplication::translate("VideoSettingTab", "\351\253\230\347\272\247\350\256\276\347\275\256", 0));
        ckbKinectDevice->setText(QApplication::translate("VideoSettingTab", "\345\220\257\347\224\250Kinect\344\275\223\346\204\237\350\256\276\345\244\207", 0));
        ckbAdaptiveCodeRate->setText(QApplication::translate("VideoSettingTab", "\345\220\257\347\224\250\350\207\252\351\200\202\345\272\224\350\247\206\351\242\221\347\240\201\347\216\207\346\216\247\345\210\266", 0));
        ckbRecvAutoResync->setText(QApplication::translate("VideoSettingTab", "\345\220\257\347\224\250\351\237\263\350\247\206\351\242\221\345\220\214\346\255\245\345\201\217\345\267\256\350\207\252\345\212\250\347\272\240\351\224\231", 0));
        saveButton->setText(QApplication::translate("VideoSettingTab", "\344\277\235\345\255\230", 0));
    } // retranslateUi

};

namespace Ui {
    class VideoSettingTab: public Ui_VideoSettingTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEO_SETTING_TAB_H
