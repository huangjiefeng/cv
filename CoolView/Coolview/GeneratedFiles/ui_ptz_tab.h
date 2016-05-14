/********************************************************************************
** Form generated from reading UI file 'ptz_tab.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PTZ_TAB_H
#define UI_PTZ_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PtzTab
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_3;
    QComboBox *numberOfCom;
    QComboBox *baudRate;
    QLabel *label_4;
    QComboBox *dataBits;
    QLabel *label_5;
    QComboBox *stopBits;
    QLabel *label_6;
    QComboBox *checkBits;
    QPushButton *openComBtn;
    QPushButton *closeComBtn;
    QPushButton *setingBtn;
    QPushButton *leftBtn;
    QPushButton *rightBtn;
    QPushButton *upBtn;
    QPushButton *callPreset1Btn;
    QPushButton *downBtn;
    QPushButton *openIrisBtn;
    QPushButton *closeIrisBtn;
    QPushButton *focusFarBtn;
    QPushButton *focusNearBtn;
    QPushButton *zoomInBtn;
    QPushButton *zoomOutBtn;
    QComboBox *protocolBtn;
    QLabel *label_2;
    QLabel *label_7;
    QLineEdit *camAdressLed;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PtzTab)
    {
        if (PtzTab->objectName().isEmpty())
            PtzTab->setObjectName(QStringLiteral("PtzTab"));
        PtzTab->resize(470, 327);
        centralWidget = new QWidget(PtzTab);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(340, 10, 331, 241));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 54, 12));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(230, 30, 54, 12));
        numberOfCom = new QComboBox(groupBox);
        numberOfCom->setObjectName(QStringLiteral("numberOfCom"));
        numberOfCom->setGeometry(QRect(30, 60, 69, 22));
        baudRate = new QComboBox(groupBox);
        baudRate->setObjectName(QStringLiteral("baudRate"));
        baudRate->setGeometry(QRect(230, 60, 69, 22));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 100, 54, 12));
        dataBits = new QComboBox(groupBox);
        dataBits->setObjectName(QStringLiteral("dataBits"));
        dataBits->setGeometry(QRect(30, 130, 69, 22));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(130, 100, 54, 12));
        stopBits = new QComboBox(groupBox);
        stopBits->setObjectName(QStringLiteral("stopBits"));
        stopBits->setGeometry(QRect(130, 130, 69, 22));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(230, 100, 54, 12));
        checkBits = new QComboBox(groupBox);
        checkBits->setObjectName(QStringLiteral("checkBits"));
        checkBits->setGeometry(QRect(230, 130, 69, 22));
        openComBtn = new QPushButton(groupBox);
        openComBtn->setObjectName(QStringLiteral("openComBtn"));
        openComBtn->setGeometry(QRect(30, 180, 75, 23));
        closeComBtn = new QPushButton(groupBox);
        closeComBtn->setObjectName(QStringLiteral("closeComBtn"));
        closeComBtn->setGeometry(QRect(130, 180, 75, 23));
        setingBtn = new QPushButton(groupBox);
        setingBtn->setObjectName(QStringLiteral("setingBtn"));
        setingBtn->setGeometry(QRect(230, 180, 75, 23));
        leftBtn = new QPushButton(centralWidget);
        leftBtn->setObjectName(QStringLiteral("leftBtn"));
        leftBtn->setGeometry(QRect(20, 120, 75, 23));
        rightBtn = new QPushButton(centralWidget);
        rightBtn->setObjectName(QStringLiteral("rightBtn"));
        rightBtn->setGeometry(QRect(220, 120, 75, 23));
        upBtn = new QPushButton(centralWidget);
        upBtn->setObjectName(QStringLiteral("upBtn"));
        upBtn->setGeometry(QRect(120, 80, 75, 23));
        callPreset1Btn = new QPushButton(centralWidget);
        callPreset1Btn->setObjectName(QStringLiteral("callPreset1Btn"));
        callPreset1Btn->setGeometry(QRect(120, 120, 75, 23));
        downBtn = new QPushButton(centralWidget);
        downBtn->setObjectName(QStringLiteral("downBtn"));
        downBtn->setGeometry(QRect(120, 160, 75, 23));
        openIrisBtn = new QPushButton(centralWidget);
        openIrisBtn->setObjectName(QStringLiteral("openIrisBtn"));
        openIrisBtn->setGeometry(QRect(20, 230, 75, 23));
        closeIrisBtn = new QPushButton(centralWidget);
        closeIrisBtn->setObjectName(QStringLiteral("closeIrisBtn"));
        closeIrisBtn->setGeometry(QRect(20, 270, 75, 23));
        focusFarBtn = new QPushButton(centralWidget);
        focusFarBtn->setObjectName(QStringLiteral("focusFarBtn"));
        focusFarBtn->setGeometry(QRect(120, 230, 75, 23));
        focusNearBtn = new QPushButton(centralWidget);
        focusNearBtn->setObjectName(QStringLiteral("focusNearBtn"));
        focusNearBtn->setGeometry(QRect(120, 270, 75, 23));
        zoomInBtn = new QPushButton(centralWidget);
        zoomInBtn->setObjectName(QStringLiteral("zoomInBtn"));
        zoomInBtn->setGeometry(QRect(220, 230, 75, 23));
        zoomOutBtn = new QPushButton(centralWidget);
        zoomOutBtn->setObjectName(QStringLiteral("zoomOutBtn"));
        zoomOutBtn->setGeometry(QRect(220, 270, 75, 23));
        protocolBtn = new QComboBox(centralWidget);
        protocolBtn->setObjectName(QStringLiteral("protocolBtn"));
        protocolBtn->setGeometry(QRect(100, 20, 81, 22));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 20, 71, 21));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(200, 20, 61, 21));
        camAdressLed = new QLineEdit(centralWidget);
        camAdressLed->setObjectName(QStringLiteral("camAdressLed"));
        camAdressLed->setGeometry(QRect(270, 20, 31, 20));
        PtzTab->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PtzTab);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 470, 23));
        PtzTab->setMenuBar(menuBar);
        statusBar = new QStatusBar(PtzTab);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PtzTab->setStatusBar(statusBar);

        retranslateUi(PtzTab);

        numberOfCom->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(PtzTab);
    } // setupUi

    void retranslateUi(QMainWindow *PtzTab)
    {
        PtzTab->setWindowTitle(QApplication::translate("PtzTab", "\344\272\221\345\217\260\346\216\247\345\210\266", 0));
#ifndef QT_NO_WHATSTHIS
        PtzTab->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("PtzTab", "\344\270\262\345\217\243\345\217\267", 0));
        label_3->setText(QApplication::translate("PtzTab", "\346\263\242\347\211\271\347\216\207", 0));
        numberOfCom->setCurrentText(QString());
        label_4->setText(QApplication::translate("PtzTab", "\346\225\260\346\215\256\344\275\215", 0));
        label_5->setText(QApplication::translate("PtzTab", "\345\201\234\346\255\242\344\275\215", 0));
        label_6->setText(QApplication::translate("PtzTab", "\346\240\241\351\252\214\344\275\215", 0));
        openComBtn->setText(QApplication::translate("PtzTab", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        closeComBtn->setText(QApplication::translate("PtzTab", "\345\205\263\351\227\255\344\270\262\345\217\243", 0));
        setingBtn->setText(QApplication::translate("PtzTab", "\351\253\230\347\272\247\350\256\276\347\275\256", 0));
        leftBtn->setText(QApplication::translate("PtzTab", "\345\267\246", 0));
        rightBtn->setText(QApplication::translate("PtzTab", "\345\217\263", 0));
        upBtn->setText(QApplication::translate("PtzTab", "\344\270\212", 0));
        callPreset1Btn->setText(QApplication::translate("PtzTab", "\351\242\204\347\275\256\344\275\2151", 0));
        downBtn->setText(QApplication::translate("PtzTab", "\344\270\213", 0));
        openIrisBtn->setText(QApplication::translate("PtzTab", "\346\211\223\345\274\200\345\205\211\345\234\210", 0));
        closeIrisBtn->setText(QApplication::translate("PtzTab", "\345\205\263\351\227\255\345\205\211\345\234\210", 0));
        focusFarBtn->setText(QApplication::translate("PtzTab", "\350\201\232\347\204\246\350\277\234", 0));
        focusNearBtn->setText(QApplication::translate("PtzTab", "\350\201\232\347\204\246\350\277\221", 0));
        zoomInBtn->setText(QApplication::translate("PtzTab", "\346\224\276\345\244\247", 0));
        zoomOutBtn->setText(QApplication::translate("PtzTab", "\347\274\251\345\260\217", 0));
        label_2->setText(QApplication::translate("PtzTab", "\344\272\221\345\217\260\346\216\247\345\210\266\345\215\217\350\256\256", 0));
        label_7->setText(QApplication::translate("PtzTab", "\346\221\204\345\203\217\346\234\272\345\234\260\345\235\200", 0));
    } // retranslateUi

};

namespace Ui {
    class PtzTab: public Ui_PtzTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PTZ_TAB_H
