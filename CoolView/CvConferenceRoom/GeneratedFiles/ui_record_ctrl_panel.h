/********************************************************************************
** Form generated from reading UI file 'record_ctrl_panel.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORD_CTRL_PANEL_H
#define UI_RECORD_CTRL_PANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecordCtrlPanel
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *RecordButton;
    QLabel *stateLabel;
    QPushButton *RecInfoEditButton;

    void setupUi(QWidget *RecordCtrlPanel)
    {
        if (RecordCtrlPanel->objectName().isEmpty())
            RecordCtrlPanel->setObjectName(QStringLiteral("RecordCtrlPanel"));
        RecordCtrlPanel->resize(382, 24);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecordCtrlPanel->sizePolicy().hasHeightForWidth());
        RecordCtrlPanel->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(RecordCtrlPanel);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        RecordButton = new QPushButton(RecordCtrlPanel);
        RecordButton->setObjectName(QStringLiteral("RecordButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RecordButton->sizePolicy().hasHeightForWidth());
        RecordButton->setSizePolicy(sizePolicy1);
        RecordButton->setMinimumSize(QSize(40, 0));
        RecordButton->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/CvConferenceRoom/Resources/Record-Start.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecordButton->setIcon(icon);
        RecordButton->setFlat(true);

        horizontalLayout->addWidget(RecordButton);

        stateLabel = new QLabel(RecordCtrlPanel);
        stateLabel->setObjectName(QStringLiteral("stateLabel"));
        stateLabel->setIndent(5);

        horizontalLayout->addWidget(stateLabel);

        RecInfoEditButton = new QPushButton(RecordCtrlPanel);
        RecInfoEditButton->setObjectName(QStringLiteral("RecInfoEditButton"));
        sizePolicy1.setHeightForWidth(RecInfoEditButton->sizePolicy().hasHeightForWidth());
        RecInfoEditButton->setSizePolicy(sizePolicy1);
        RecInfoEditButton->setMinimumSize(QSize(40, 0));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/CvConferenceRoom/Resources/Record-Edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecInfoEditButton->setIcon(icon1);
        RecInfoEditButton->setFlat(true);

        horizontalLayout->addWidget(RecInfoEditButton);


        retranslateUi(RecordCtrlPanel);

        QMetaObject::connectSlotsByName(RecordCtrlPanel);
    } // setupUi

    void retranslateUi(QWidget *RecordCtrlPanel)
    {
        RecordCtrlPanel->setWindowTitle(QApplication::translate("RecordCtrlPanel", "RecordCtrlPanel", 0));
        RecordButton->setText(QString());
        stateLabel->setText(QApplication::translate("RecordCtrlPanel", "stateLabel", 0));
        RecInfoEditButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RecordCtrlPanel: public Ui_RecordCtrlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORD_CTRL_PANEL_H
