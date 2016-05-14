/********************************************************************************
** Form generated from reading UI file 'device_test_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICE_TEST_TAB_H
#define UI_DEVICE_TEST_TAB_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceTestTab
{
public:
    QGroupBox *frame_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_14;
    QComboBox *audioInputTest_InputDeviceBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_16;
    QComboBox *audioInputTest_OutputDeviceBox;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *audioInputTestStartButton;
    QPushButton *audioInputTestStopButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_13;
    QComboBox *audioOutputTestBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *audioOutputTestStartButton;
    QPushButton *audioOutputTestStopButton;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_26;
    QComboBox *webcamDeviceTestComboBox;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_27;
    QComboBox *crossbarlistTestcomboBox;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_28;
    QComboBox *crossbarclassTestcomboBox;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_29;
    QComboBox *videosizeTestcomboBox;
    QVBoxLayout *verticalLayout_4;
    QFrame *previewFrame;
    QPushButton *videoPreViewButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *serverTestButton;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_46;
    QLineEdit *UploadSeverAddressEdit;
    QPushButton *uploadLogButton;
    QTextEdit *testResultText;

    void setupUi(QWidget *DeviceTestTab)
    {
        if (DeviceTestTab->objectName().isEmpty())
            DeviceTestTab->setObjectName(QStringLiteral("DeviceTestTab"));
        DeviceTestTab->resize(612, 536);
        frame_3 = new QGroupBox(DeviceTestTab);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(310, 180, 291, 121));
        frame_3->setAlignment(Qt::AlignCenter);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_14 = new QLabel(frame_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_6->addWidget(label_14);

        audioInputTest_InputDeviceBox = new QComboBox(frame_3);
        audioInputTest_InputDeviceBox->setObjectName(QStringLiteral("audioInputTest_InputDeviceBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(audioInputTest_InputDeviceBox->sizePolicy().hasHeightForWidth());
        audioInputTest_InputDeviceBox->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(audioInputTest_InputDeviceBox);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_7->addWidget(label_16);

        audioInputTest_OutputDeviceBox = new QComboBox(frame_3);
        audioInputTest_OutputDeviceBox->setObjectName(QStringLiteral("audioInputTest_OutputDeviceBox"));
        sizePolicy.setHeightForWidth(audioInputTest_OutputDeviceBox->sizePolicy().hasHeightForWidth());
        audioInputTest_OutputDeviceBox->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(audioInputTest_OutputDeviceBox);


        verticalLayout_3->addLayout(horizontalLayout_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        audioInputTestStartButton = new QPushButton(frame_3);
        audioInputTestStartButton->setObjectName(QStringLiteral("audioInputTestStartButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(audioInputTestStartButton->sizePolicy().hasHeightForWidth());
        audioInputTestStartButton->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(audioInputTestStartButton);

        audioInputTestStopButton = new QPushButton(frame_3);
        audioInputTestStopButton->setObjectName(QStringLiteral("audioInputTestStopButton"));
        sizePolicy1.setHeightForWidth(audioInputTestStopButton->sizePolicy().hasHeightForWidth());
        audioInputTestStopButton->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(audioInputTestStopButton);


        verticalLayout_3->addLayout(horizontalLayout_5);

        groupBox = new QGroupBox(DeviceTestTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 180, 291, 121));
        groupBox->setAlignment(Qt::AlignCenter);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_3->addWidget(label_13);

        audioOutputTestBox = new QComboBox(groupBox);
        audioOutputTestBox->setObjectName(QStringLiteral("audioOutputTestBox"));
        sizePolicy.setHeightForWidth(audioOutputTestBox->sizePolicy().hasHeightForWidth());
        audioOutputTestBox->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(audioOutputTestBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        audioOutputTestStartButton = new QPushButton(groupBox);
        audioOutputTestStartButton->setObjectName(QStringLiteral("audioOutputTestStartButton"));
        sizePolicy1.setHeightForWidth(audioOutputTestStartButton->sizePolicy().hasHeightForWidth());
        audioOutputTestStartButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(audioOutputTestStartButton);

        audioOutputTestStopButton = new QPushButton(groupBox);
        audioOutputTestStopButton->setObjectName(QStringLiteral("audioOutputTestStopButton"));
        sizePolicy1.setHeightForWidth(audioOutputTestStopButton->sizePolicy().hasHeightForWidth());
        audioOutputTestStopButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(audioOutputTestStopButton);


        verticalLayout_2->addLayout(horizontalLayout_4);

        groupBox_2 = new QGroupBox(DeviceTestTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 591, 164));
        groupBox_2->setAlignment(Qt::AlignCenter);
        horizontalLayout_13 = new QHBoxLayout(groupBox_2);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_26 = new QLabel(groupBox_2);
        label_26->setObjectName(QStringLiteral("label_26"));

        horizontalLayout_9->addWidget(label_26);

        webcamDeviceTestComboBox = new QComboBox(groupBox_2);
        webcamDeviceTestComboBox->setObjectName(QStringLiteral("webcamDeviceTestComboBox"));
        sizePolicy.setHeightForWidth(webcamDeviceTestComboBox->sizePolicy().hasHeightForWidth());
        webcamDeviceTestComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(webcamDeviceTestComboBox);


        verticalLayout_7->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_27 = new QLabel(groupBox_2);
        label_27->setObjectName(QStringLiteral("label_27"));

        horizontalLayout_10->addWidget(label_27);

        crossbarlistTestcomboBox = new QComboBox(groupBox_2);
        crossbarlistTestcomboBox->setObjectName(QStringLiteral("crossbarlistTestcomboBox"));
        sizePolicy.setHeightForWidth(crossbarlistTestcomboBox->sizePolicy().hasHeightForWidth());
        crossbarlistTestcomboBox->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(crossbarlistTestcomboBox);


        verticalLayout_7->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_28 = new QLabel(groupBox_2);
        label_28->setObjectName(QStringLiteral("label_28"));

        horizontalLayout_11->addWidget(label_28);

        crossbarclassTestcomboBox = new QComboBox(groupBox_2);
        crossbarclassTestcomboBox->setObjectName(QStringLiteral("crossbarclassTestcomboBox"));
        sizePolicy.setHeightForWidth(crossbarclassTestcomboBox->sizePolicy().hasHeightForWidth());
        crossbarclassTestcomboBox->setSizePolicy(sizePolicy);

        horizontalLayout_11->addWidget(crossbarclassTestcomboBox);


        verticalLayout_7->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_29 = new QLabel(groupBox_2);
        label_29->setObjectName(QStringLiteral("label_29"));

        horizontalLayout_12->addWidget(label_29);

        videosizeTestcomboBox = new QComboBox(groupBox_2);
        videosizeTestcomboBox->setObjectName(QStringLiteral("videosizeTestcomboBox"));
        sizePolicy.setHeightForWidth(videosizeTestcomboBox->sizePolicy().hasHeightForWidth());
        videosizeTestcomboBox->setSizePolicy(sizePolicy);

        horizontalLayout_12->addWidget(videosizeTestcomboBox);


        verticalLayout_7->addLayout(horizontalLayout_12);


        horizontalLayout_13->addLayout(verticalLayout_7);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        previewFrame = new QFrame(groupBox_2);
        previewFrame->setObjectName(QStringLiteral("previewFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(previewFrame->sizePolicy().hasHeightForWidth());
        previewFrame->setSizePolicy(sizePolicy2);
        previewFrame->setMinimumSize(QSize(171, 101));
        previewFrame->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        previewFrame->setFrameShape(QFrame::StyledPanel);
        previewFrame->setFrameShadow(QFrame::Raised);

        verticalLayout_4->addWidget(previewFrame);

        videoPreViewButton = new QPushButton(groupBox_2);
        videoPreViewButton->setObjectName(QStringLiteral("videoPreViewButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(videoPreViewButton->sizePolicy().hasHeightForWidth());
        videoPreViewButton->setSizePolicy(sizePolicy3);

        verticalLayout_4->addWidget(videoPreViewButton);


        horizontalLayout_13->addLayout(verticalLayout_4);

        horizontalLayoutWidget = new QWidget(DeviceTestTab);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 310, 591, 61));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(horizontalLayoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy4);
        groupBox_3->setAlignment(Qt::AlignCenter);
        horizontalLayout_2 = new QHBoxLayout(groupBox_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        serverTestButton = new QPushButton(groupBox_3);
        serverTestButton->setObjectName(QStringLiteral("serverTestButton"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(serverTestButton->sizePolicy().hasHeightForWidth());
        serverTestButton->setSizePolicy(sizePolicy5);
        serverTestButton->setMinimumSize(QSize(150, 0));
        serverTestButton->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_2->addWidget(serverTestButton);


        horizontalLayout_8->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(horizontalLayoutWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setAlignment(Qt::AlignCenter);
        horizontalLayout = new QHBoxLayout(groupBox_4);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_46 = new QLabel(groupBox_4);
        label_46->setObjectName(QStringLiteral("label_46"));

        horizontalLayout->addWidget(label_46);

        UploadSeverAddressEdit = new QLineEdit(groupBox_4);
        UploadSeverAddressEdit->setObjectName(QStringLiteral("UploadSeverAddressEdit"));

        horizontalLayout->addWidget(UploadSeverAddressEdit);

        uploadLogButton = new QPushButton(groupBox_4);
        uploadLogButton->setObjectName(QStringLiteral("uploadLogButton"));
        sizePolicy3.setHeightForWidth(uploadLogButton->sizePolicy().hasHeightForWidth());
        uploadLogButton->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(uploadLogButton);


        horizontalLayout_8->addWidget(groupBox_4);

        testResultText = new QTextEdit(DeviceTestTab);
        testResultText->setObjectName(QStringLiteral("testResultText"));
        testResultText->setGeometry(QRect(10, 380, 591, 29));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(testResultText->sizePolicy().hasHeightForWidth());
        testResultText->setSizePolicy(sizePolicy6);
        testResultText->setMinimumSize(QSize(15, 0));
        testResultText->setMaximumSize(QSize(16777215, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(10);
        testResultText->setFont(font);
        testResultText->setReadOnly(true);

        retranslateUi(DeviceTestTab);

        QMetaObject::connectSlotsByName(DeviceTestTab);
    } // setupUi

    void retranslateUi(QWidget *DeviceTestTab)
    {
        DeviceTestTab->setWindowTitle(QApplication::translate("DeviceTestTab", "\350\256\276\345\244\207\346\265\213\350\257\225", 0));
        frame_3->setTitle(QApplication::translate("DeviceTestTab", "\345\243\260\351\237\263\350\276\223\345\205\245\350\256\276\345\244\207\346\265\213\350\257\225", 0));
        label_14->setText(QApplication::translate("DeviceTestTab", "\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_16->setText(QApplication::translate("DeviceTestTab", "\350\276\223\345\207\272\350\256\276\345\244\207", 0));
        audioInputTestStartButton->setText(QApplication::translate("DeviceTestTab", "\345\274\200\345\247\213\346\265\213\350\257\225", 0));
        audioInputTestStopButton->setText(QApplication::translate("DeviceTestTab", "\345\201\234\346\255\242", 0));
        groupBox->setTitle(QApplication::translate("DeviceTestTab", "\345\243\260\351\237\263\350\276\223\345\207\272\350\256\276\345\244\207\346\265\213\350\257\225", 0));
        label_13->setText(QApplication::translate("DeviceTestTab", "\350\276\223\345\207\272\350\256\276\345\244\207", 0));
        audioOutputTestStartButton->setText(QApplication::translate("DeviceTestTab", "\345\274\200\345\247\213\346\265\213\350\257\225", 0));
        audioOutputTestStopButton->setText(QApplication::translate("DeviceTestTab", "\345\201\234\346\255\242", 0));
        groupBox_2->setTitle(QApplication::translate("DeviceTestTab", "\350\247\206\351\242\221\351\242\204\350\247\210", 0));
        label_26->setText(QApplication::translate("DeviceTestTab", "\350\247\206\351\242\221\351\200\211\346\213\251\350\256\276\345\244\207", 0));
        label_27->setText(QApplication::translate("DeviceTestTab", "\351\253\230\346\270\205\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_28->setText(QApplication::translate("DeviceTestTab", "\351\253\230\346\270\205\350\276\223\345\205\245\347\261\273\345\236\213", 0));
        label_29->setText(QApplication::translate("DeviceTestTab", "\350\247\206\351\242\221\345\233\276\345\203\217\345\244\247\345\260\217", 0));
        videoPreViewButton->setText(QApplication::translate("DeviceTestTab", "\351\242\204\350\247\210", 0));
        groupBox_3->setTitle(QApplication::translate("DeviceTestTab", "\344\274\232\350\256\256\346\234\215\345\212\241\345\231\250\345\272\224\347\255\224\345\273\266\350\277\237\346\265\213\350\257\225", 0));
        serverTestButton->setText(QApplication::translate("DeviceTestTab", "\345\274\200\345\247\213\346\265\213\350\257\225", 0));
        groupBox_4->setTitle(QApplication::translate("DeviceTestTab", "\346\227\245\345\277\227\344\270\212\344\274\240", 0));
        label_46->setText(QApplication::translate("DeviceTestTab", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200", 0));
        uploadLogButton->setText(QApplication::translate("DeviceTestTab", "\344\270\212\344\274\240\346\227\245\345\277\227\344\277\241\346\201\257", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceTestTab: public Ui_DeviceTestTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICE_TEST_TAB_H
