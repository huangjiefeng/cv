/********************************************************************************
** Form generated from reading UI file 'record_advanced_panel.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORD_ADVANCED_PANEL_H
#define UI_RECORD_ADVANCED_PANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RecordAdvancedPanel
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *titleLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *participantsLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *keywordsLineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QPlainTextEdit *descTextEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QPushButton *resetButton;

    void setupUi(QFrame *RecordAdvancedPanel)
    {
        if (RecordAdvancedPanel->objectName().isEmpty())
            RecordAdvancedPanel->setObjectName(QStringLiteral("RecordAdvancedPanel"));
        RecordAdvancedPanel->resize(300, 148);
        RecordAdvancedPanel->setStyleSheet(QStringLiteral("background-color: rgb(240,240,240);border-width:0px;"));
        verticalLayout_2 = new QVBoxLayout(RecordAdvancedPanel);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(RecordAdvancedPanel);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(50, 0));

        horizontalLayout->addWidget(label);

        titleLineEdit = new QLineEdit(RecordAdvancedPanel);
        titleLineEdit->setObjectName(QStringLiteral("titleLineEdit"));
        titleLineEdit->setStyleSheet(QStringLiteral("background-color: white;border-width:1px;"));
        titleLineEdit->setMaxLength(20);
        titleLineEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(titleLineEdit);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(RecordAdvancedPanel);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(50, 0));

        horizontalLayout_2->addWidget(label_2);

        participantsLineEdit = new QLineEdit(RecordAdvancedPanel);
        participantsLineEdit->setObjectName(QStringLiteral("participantsLineEdit"));
        participantsLineEdit->setStyleSheet(QStringLiteral("background-color: white;border-width:1px;"));
        participantsLineEdit->setMaxLength(20);
        participantsLineEdit->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(participantsLineEdit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(RecordAdvancedPanel);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(50, 0));

        horizontalLayout_3->addWidget(label_3);

        keywordsLineEdit = new QLineEdit(RecordAdvancedPanel);
        keywordsLineEdit->setObjectName(QStringLiteral("keywordsLineEdit"));
        keywordsLineEdit->setStyleSheet(QStringLiteral("background-color: white;border-width:1px;"));
        keywordsLineEdit->setMaxLength(20);
        keywordsLineEdit->setClearButtonEnabled(true);

        horizontalLayout_3->addWidget(keywordsLineEdit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(RecordAdvancedPanel);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(50, 0));

        horizontalLayout_4->addWidget(label_4);

        descTextEdit = new QPlainTextEdit(RecordAdvancedPanel);
        descTextEdit->setObjectName(QStringLiteral("descTextEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(descTextEdit->sizePolicy().hasHeightForWidth());
        descTextEdit->setSizePolicy(sizePolicy1);
        descTextEdit->setMinimumSize(QSize(0, 25));
        descTextEdit->setFocusPolicy(Qt::StrongFocus);
        descTextEdit->setStyleSheet(QStringLiteral("background-color: white;border-width:1px;"));

        horizontalLayout_4->addWidget(descTextEdit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(RecordAdvancedPanel);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(label_5);

        resetButton = new QPushButton(RecordAdvancedPanel);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        sizePolicy.setHeightForWidth(resetButton->sizePolicy().hasHeightForWidth());
        resetButton->setSizePolicy(sizePolicy);
        resetButton->setMinimumSize(QSize(50, 0));
        resetButton->setStyleSheet(QStringLiteral("border-width:1px;border-radius: 2px; background-color:rgb(200,200,200);margin:3px;"));

        horizontalLayout_5->addWidget(resetButton);


        verticalLayout_2->addLayout(horizontalLayout_5);


        retranslateUi(RecordAdvancedPanel);

        QMetaObject::connectSlotsByName(RecordAdvancedPanel);
    } // setupUi

    void retranslateUi(QFrame *RecordAdvancedPanel)
    {
        RecordAdvancedPanel->setWindowTitle(QApplication::translate("RecordAdvancedPanel", "RecordAdvancedPanel", 0));
        label->setText(QApplication::translate("RecordAdvancedPanel", "\345\275\225\345\203\217\346\240\207\351\242\230", 0));
        titleLineEdit->setPlaceholderText(QApplication::translate("RecordAdvancedPanel", "\350\207\252\345\212\250\347\224\237\346\210\220\346\240\207\351\242\230", 0));
        label_2->setText(QApplication::translate("RecordAdvancedPanel", "\344\270\216 \344\274\232 \350\200\205", 0));
        participantsLineEdit->setPlaceholderText(QApplication::translate("RecordAdvancedPanel", "\350\257\267\350\276\223\345\205\245\344\270\216\344\274\232\344\272\272\345\221\230", 0));
        label_3->setText(QApplication::translate("RecordAdvancedPanel", "\345\205\263 \351\224\256 \350\257\215", 0));
        keywordsLineEdit->setPlaceholderText(QApplication::translate("RecordAdvancedPanel", "\350\257\267\350\276\223\345\205\245\345\205\263\351\224\256\350\257\215", 0));
        label_4->setText(QApplication::translate("RecordAdvancedPanel", "\346\217\217    \350\277\260", 0));
        descTextEdit->setPlainText(QString());
        descTextEdit->setPlaceholderText(QApplication::translate("RecordAdvancedPanel", "\346\217\217\350\277\260", 0));
        label_5->setText(QApplication::translate("RecordAdvancedPanel", "\346\263\250\346\204\217\357\274\232\344\273\245\347\202\271\345\207\273\345\275\225\345\210\266\346\214\211\351\222\256\346\227\266\347\232\204\344\277\241\346\201\257\344\270\272\345\207\206", 0));
        resetButton->setText(QApplication::translate("RecordAdvancedPanel", "\351\207\215\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class RecordAdvancedPanel: public Ui_RecordAdvancedPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORD_ADVANCED_PANEL_H
