/********************************************************************************
** Form generated from reading ui file 'Wizard.ui'
**
** Created: Mon Apr 20 22:15:59 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WIZARD_H
#define UI_WIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>

QT_BEGIN_NAMESPACE

class Ui_Wizard
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;
    QPushButton *finishButton;
    QPushButton *nextButton;
    QPushButton *backButton;
    QStackedWidget *stackedWidget;

    void setupUi(QDialog *Wizard)
    {
    if (Wizard->objectName().isEmpty())
        Wizard->setObjectName(QString::fromUtf8("Wizard"));
    Wizard->resize(392, 316);
    gridLayout = new QGridLayout(Wizard);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout1->setMargin(0);
#endif
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    cancelButton = new QPushButton(Wizard);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    gridLayout1->addWidget(cancelButton, 0, 1, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 0, 1, 1);

    finishButton = new QPushButton(Wizard);
    finishButton->setObjectName(QString::fromUtf8("finishButton"));

    gridLayout1->addWidget(finishButton, 0, 4, 1, 1);

    nextButton = new QPushButton(Wizard);
    nextButton->setObjectName(QString::fromUtf8("nextButton"));

    gridLayout1->addWidget(nextButton, 0, 3, 1, 1);

    backButton = new QPushButton(Wizard);
    backButton->setObjectName(QString::fromUtf8("backButton"));

    gridLayout1->addWidget(backButton, 0, 2, 1, 1);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

    stackedWidget = new QStackedWidget(Wizard);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

    gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);


    retranslateUi(Wizard);
    QObject::connect(finishButton, SIGNAL(clicked()), Wizard, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), Wizard, SLOT(reject()));

    stackedWidget->setCurrentIndex(-1);


    QMetaObject::connectSlotsByName(Wizard);
    } // setupUi

    void retranslateUi(QDialog *Wizard)
    {
    Wizard->setWindowTitle(QString());
    cancelButton->setText(QApplication::translate("Wizard", "Cancel", 0, QApplication::UnicodeUTF8));
    finishButton->setText(QApplication::translate("Wizard", "Finish", 0, QApplication::UnicodeUTF8));
    nextButton->setText(QApplication::translate("Wizard", "Next >", 0, QApplication::UnicodeUTF8));
    backButton->setText(QApplication::translate("Wizard", "< Back", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Wizard);
    } // retranslateUi

};

namespace Ui {
    class Wizard: public Ui_Wizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARD_H
