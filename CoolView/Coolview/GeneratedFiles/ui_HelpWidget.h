/********************************************************************************
** Form generated from reading UI file 'HelpWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPWIDGET_H
#define UI_HELPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelpForm
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *LogoLabel;
    QVBoxLayout *verticalLayout;
    QLabel *CoolviewVersionStringLabel;
    QLabel *DependenciesLabel;
    QLabel *LinkLabel;
    QPlainTextEdit *DetailTextEdit;

    void setupUi(QWidget *HelpForm)
    {
        if (HelpForm->objectName().isEmpty())
            HelpForm->setObjectName(QStringLiteral("HelpForm"));
        HelpForm->resize(741, 462);
        horizontalLayout = new QHBoxLayout(HelpForm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        LogoLabel = new QLabel(HelpForm);
        LogoLabel->setObjectName(QStringLiteral("LogoLabel"));
        LogoLabel->setMinimumSize(QSize(200, 0));
        LogoLabel->setMaximumSize(QSize(16777215, 16777215));
        LogoLabel->setStyleSheet(QStringLiteral("border-image: url(:/image/helplogo.png);"));
        LogoLabel->setAlignment(Qt::AlignCenter);
        LogoLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(LogoLabel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        CoolviewVersionStringLabel = new QLabel(HelpForm);
        CoolviewVersionStringLabel->setObjectName(QStringLiteral("CoolviewVersionStringLabel"));
        CoolviewVersionStringLabel->setAlignment(Qt::AlignCenter);
        CoolviewVersionStringLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(CoolviewVersionStringLabel);

        DependenciesLabel = new QLabel(HelpForm);
        DependenciesLabel->setObjectName(QStringLiteral("DependenciesLabel"));
        DependenciesLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(DependenciesLabel);

        LinkLabel = new QLabel(HelpForm);
        LinkLabel->setObjectName(QStringLiteral("LinkLabel"));
        LinkLabel->setAlignment(Qt::AlignCenter);
        LinkLabel->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);

        verticalLayout->addWidget(LinkLabel);

        DetailTextEdit = new QPlainTextEdit(HelpForm);
        DetailTextEdit->setObjectName(QStringLiteral("DetailTextEdit"));
        DetailTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        DetailTextEdit->setCenterOnScroll(true);

        verticalLayout->addWidget(DetailTextEdit);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(HelpForm);

        QMetaObject::connectSlotsByName(HelpForm);
    } // setupUi

    void retranslateUi(QWidget *HelpForm)
    {
        HelpForm->setWindowTitle(QApplication::translate("HelpForm", "\345\205\263\344\272\216CoolView", 0));
        CoolviewVersionStringLabel->setText(QApplication::translate("HelpForm", "CoolView\347\211\210\346\234\254\357\274\232", 0));
        DependenciesLabel->setText(QString());
        LinkLabel->setText(QApplication::translate("HelpForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://www.dcampus.com/\"><span style=\" text-decoration: underline; color:#0000ff;\">http://www.dcampus.com/</span></a></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class HelpForm: public Ui_HelpForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPWIDGET_H
