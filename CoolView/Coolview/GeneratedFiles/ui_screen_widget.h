/********************************************************************************
** Form generated from reading UI file 'screen_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_WIDGET_H
#define UI_SCREEN_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenWidget
{
public:
    QAction *oneMultiOneAction;
    QAction *twoMultiTwoAction;
    QAction *threeMultiThreeAction;
    QAction *onePlusFiveLAction;
    QAction *showRTCPAction;
    QAction *documentAction;
    QAction *autoLayoutAction;
    QAction *oneMultiTwoTopAction;
    QAction *fourMultiFourAction;
    QAction *airplayAction;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;

    void setupUi(QWidget *ScreenWidget)
    {
        if (ScreenWidget->objectName().isEmpty())
            ScreenWidget->setObjectName(QStringLiteral("ScreenWidget"));
        ScreenWidget->resize(288, 162);
        ScreenWidget->setMinimumSize(QSize(288, 162));
        ScreenWidget->setMaximumSize(QSize(288, 162));
        ScreenWidget->setAcceptDrops(true);
        oneMultiOneAction = new QAction(ScreenWidget);
        oneMultiOneAction->setObjectName(QStringLiteral("oneMultiOneAction"));
        oneMultiOneAction->setCheckable(true);
        twoMultiTwoAction = new QAction(ScreenWidget);
        twoMultiTwoAction->setObjectName(QStringLiteral("twoMultiTwoAction"));
        twoMultiTwoAction->setCheckable(true);
        threeMultiThreeAction = new QAction(ScreenWidget);
        threeMultiThreeAction->setObjectName(QStringLiteral("threeMultiThreeAction"));
        threeMultiThreeAction->setCheckable(true);
        onePlusFiveLAction = new QAction(ScreenWidget);
        onePlusFiveLAction->setObjectName(QStringLiteral("onePlusFiveLAction"));
        onePlusFiveLAction->setCheckable(true);
        showRTCPAction = new QAction(ScreenWidget);
        showRTCPAction->setObjectName(QStringLiteral("showRTCPAction"));
        showRTCPAction->setCheckable(true);
        documentAction = new QAction(ScreenWidget);
        documentAction->setObjectName(QStringLiteral("documentAction"));
        documentAction->setCheckable(true);
        autoLayoutAction = new QAction(ScreenWidget);
        autoLayoutAction->setObjectName(QStringLiteral("autoLayoutAction"));
        autoLayoutAction->setCheckable(true);
        oneMultiTwoTopAction = new QAction(ScreenWidget);
        oneMultiTwoTopAction->setObjectName(QStringLiteral("oneMultiTwoTopAction"));
        oneMultiTwoTopAction->setCheckable(true);
        fourMultiFourAction = new QAction(ScreenWidget);
        fourMultiFourAction->setObjectName(QStringLiteral("fourMultiFourAction"));
        fourMultiFourAction->setCheckable(true);
        airplayAction = new QAction(ScreenWidget);
        airplayAction->setObjectName(QStringLiteral("airplayAction"));
        airplayAction->setCheckable(true);
        verticalLayout = new QVBoxLayout(ScreenWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(ScreenWidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setAcceptDrops(true);
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(1, 1, 1, 1);

        verticalLayout->addWidget(frame);


        retranslateUi(ScreenWidget);

        QMetaObject::connectSlotsByName(ScreenWidget);
    } // setupUi

    void retranslateUi(QWidget *ScreenWidget)
    {
        ScreenWidget->setWindowTitle(QApplication::translate("ScreenWidget", "ScreenWidget", 0));
        oneMultiOneAction->setText(QApplication::translate("ScreenWidget", "1*1", 0));
        twoMultiTwoAction->setText(QApplication::translate("ScreenWidget", "2*2", 0));
        threeMultiThreeAction->setText(QApplication::translate("ScreenWidget", "3*3", 0));
        onePlusFiveLAction->setText(QApplication::translate("ScreenWidget", "1+5", 0));
        showRTCPAction->setText(QApplication::translate("ScreenWidget", "QoS\345\256\236\346\227\266\344\277\241\346\201\257", 0));
#ifndef QT_NO_TOOLTIP
        showRTCPAction->setToolTip(QApplication::translate("ScreenWidget", "QoS\345\256\236\346\227\266\344\277\241\346\201\257", 0));
#endif // QT_NO_TOOLTIP
        documentAction->setText(QApplication::translate("ScreenWidget", "\345\261\217\345\271\225\345\205\261\344\272\253(\346\234\254\345\234\260\346\226\207\344\273\266)", 0));
#ifndef QT_NO_TOOLTIP
        documentAction->setToolTip(QApplication::translate("ScreenWidget", "\345\261\217\345\271\225\345\205\261\344\272\253(\346\234\254\345\234\260\346\226\207\344\273\266)", 0));
#endif // QT_NO_TOOLTIP
        autoLayoutAction->setText(QApplication::translate("ScreenWidget", "\350\207\252\345\212\250\345\270\203\345\261\200", 0));
        oneMultiTwoTopAction->setText(QApplication::translate("ScreenWidget", "\344\270\212\345\215\212\345\261\217\345\271\2251*2", 0));
#ifndef QT_NO_TOOLTIP
        oneMultiTwoTopAction->setToolTip(QApplication::translate("ScreenWidget", "\344\270\212\345\215\212\345\261\217\345\271\2251*2", 0));
#endif // QT_NO_TOOLTIP
        fourMultiFourAction->setText(QApplication::translate("ScreenWidget", "4*4", 0));
#ifndef QT_NO_TOOLTIP
        fourMultiFourAction->setToolTip(QApplication::translate("ScreenWidget", "4*4", 0));
#endif // QT_NO_TOOLTIP
        airplayAction->setText(QApplication::translate("ScreenWidget", "\345\261\217\345\271\225\345\205\261\344\272\253(AirPlay)", 0));
#ifndef QT_NO_TOOLTIP
        airplayAction->setToolTip(QApplication::translate("ScreenWidget", "\345\261\217\345\271\225\345\205\261\344\272\253(AirPlay)", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class ScreenWidget: public Ui_ScreenWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_WIDGET_H
