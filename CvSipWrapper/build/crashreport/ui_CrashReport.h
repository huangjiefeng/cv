/********************************************************************************
** Form generated from reading ui file 'CrashReport.ui'
**
** Created: Sat Dec 13 10:42:20 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CRASHREPORT_H
#define UI_CRASHREPORT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CrashReport
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QLabel *mainLabel;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QTextEdit *descTextEdit;
    QLineEdit *mailLineEdit;
    QLabel *label_4;
    QLabel *label;
    QProgressBar *progressBar;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *sendButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *CrashReport)
    {
    if (CrashReport->objectName().isEmpty())
        CrashReport->setObjectName(QString::fromUtf8("CrashReport"));
    CrashReport->resize(562, 523);
    vboxLayout = new QVBoxLayout(CrashReport);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout->setMargin(9);
#endif
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    groupBox = new QGroupBox(CrashReport);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout1 = new QVBoxLayout(groupBox);
#ifndef Q_OS_MAC
    vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout1->setMargin(9);
#endif
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    mainLabel = new QLabel(groupBox);
    mainLabel->setObjectName(QString::fromUtf8("mainLabel"));

    vboxLayout1->addWidget(mainLabel);


    vboxLayout->addWidget(groupBox);

    groupBox_2 = new QGroupBox(CrashReport);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    gridLayout = new QGridLayout(groupBox_2);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 3, 0, 1, 1);

    descTextEdit = new QTextEdit(groupBox_2);
    descTextEdit->setObjectName(QString::fromUtf8("descTextEdit"));

    gridLayout->addWidget(descTextEdit, 2, 0, 1, 1);

    mailLineEdit = new QLineEdit(groupBox_2);
    mailLineEdit->setObjectName(QString::fromUtf8("mailLineEdit"));

    gridLayout->addWidget(mailLineEdit, 4, 0, 1, 1);

    label_4 = new QLabel(groupBox_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 0, 0, 1, 1);

    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 1, 0, 1, 1);


    vboxLayout->addWidget(groupBox_2);

    progressBar = new QProgressBar(CrashReport);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setValue(24);
    progressBar->setOrientation(Qt::Horizontal);

    vboxLayout->addWidget(progressBar);

    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    sendButton = new QPushButton(CrashReport);
    sendButton->setObjectName(QString::fromUtf8("sendButton"));

    hboxLayout->addWidget(sendButton);

    cancelButton = new QPushButton(CrashReport);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);


    vboxLayout->addLayout(hboxLayout);

    QWidget::setTabOrder(descTextEdit, mailLineEdit);
    QWidget::setTabOrder(mailLineEdit, sendButton);
    QWidget::setTabOrder(sendButton, cancelButton);

    retranslateUi(CrashReport);
    QObject::connect(cancelButton, SIGNAL(clicked()), CrashReport, SLOT(reject()));

    QMetaObject::connectSlotsByName(CrashReport);
    } // setupUi

    void retranslateUi(QDialog *CrashReport)
    {
    groupBox->setTitle(QApplication::translate("CrashReport", "Program Failure", 0, QApplication::UnicodeUTF8));
    mainLabel->setText(QApplication::translate("CrashReport", "The @product@ execution failed and has generated a diagnostic file.\n"
"This file contains useful information to solve the problem you are encountering.\n"
"Thanks for your contribution.\n"
"\n"
"The @product@ Development Team.", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("CrashReport", "Optional Information", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CrashReport", "Your Mail:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("CrashReport", "In order to help the developers you can complete this report. This step is optional.", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CrashReport", "Use Case Description:", 0, QApplication::UnicodeUTF8));
    sendButton->setText(QApplication::translate("CrashReport", "Send", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("CrashReport", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CrashReport);
    } // retranslateUi

};

namespace Ui {
    class CrashReport: public Ui_CrashReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CRASHREPORT_H
