/********************************************************************************
** Form generated from reading UI file 'login_form.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_FORM_H
#define UI_LOGIN_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QPushButton *ipConfigButton;
    QPushButton *weblibConfigButton;
    QPushButton *deviceConfigButton;
    QPushButton *cancelButton;
    QToolButton *toolButton;
    QPushButton *loginButton;
    QCheckBox *autoLoginCheckbox;
    QCheckBox *rememberCheckBox;
    QLabel *warningLabel;
    QLineEdit *userNameLineEdit;
    QComboBox *realmComboBox;
    QLineEdit *passwordLineEdit;
    QPushButton *refreshIPListButton;
    QComboBox *IPListComboBox;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QStringLiteral("LoginForm"));
        LoginForm->resize(960, 540);
        ipConfigButton = new QPushButton(LoginForm);
        ipConfigButton->setObjectName(QStringLiteral("ipConfigButton"));
        ipConfigButton->setGeometry(QRect(616, 510, 75, 23));
        weblibConfigButton = new QPushButton(LoginForm);
        weblibConfigButton->setObjectName(QStringLiteral("weblibConfigButton"));
        weblibConfigButton->setGeometry(QRect(520, 510, 75, 23));
        deviceConfigButton = new QPushButton(LoginForm);
        deviceConfigButton->setObjectName(QStringLiteral("deviceConfigButton"));
        deviceConfigButton->setGeometry(QRect(716, 510, 75, 23));
        cancelButton = new QPushButton(LoginForm);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(814, 510, 75, 23));
        toolButton = new QToolButton(LoginForm);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(442, 510, 63, 22));
        toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolButton->setAutoRaise(true);
        toolButton->setArrowType(Qt::RightArrow);
        loginButton = new QPushButton(LoginForm);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(616, 360, 278, 40));
        autoLoginCheckbox = new QCheckBox(LoginForm);
        autoLoginCheckbox->setObjectName(QStringLiteral("autoLoginCheckbox"));
        autoLoginCheckbox->setGeometry(QRect(792, 318, 16, 16));
        rememberCheckBox = new QCheckBox(LoginForm);
        rememberCheckBox->setObjectName(QStringLiteral("rememberCheckBox"));
        rememberCheckBox->setGeometry(QRect(624, 318, 16, 16));
        warningLabel = new QLabel(LoginForm);
        warningLabel->setObjectName(QStringLiteral("warningLabel"));
        warningLabel->setGeometry(QRect(146, 512, 271, 16));
        userNameLineEdit = new QLineEdit(LoginForm);
        userNameLineEdit->setObjectName(QStringLiteral("userNameLineEdit"));
        userNameLineEdit->setGeometry(QRect(716, 70, 177, 20));
        realmComboBox = new QComboBox(LoginForm);
        realmComboBox->setObjectName(QStringLiteral("realmComboBox"));
        realmComboBox->setGeometry(QRect(716, 252, 177, 20));
        realmComboBox->setEditable(true);
        passwordLineEdit = new QLineEdit(LoginForm);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(716, 132, 177, 20));
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        refreshIPListButton = new QPushButton(LoginForm);
        refreshIPListButton->setObjectName(QStringLiteral("refreshIPListButton"));
        refreshIPListButton->setGeometry(QRect(864, 186, 35, 35));
        refreshIPListButton->setMaximumSize(QSize(35, 16777215));
        IPListComboBox = new QComboBox(LoginForm);
        IPListComboBox->setObjectName(QStringLiteral("IPListComboBox"));
        IPListComboBox->setGeometry(QRect(716, 190, 143, 20));
        IPListComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        refreshIPListButton->raise();
        warningLabel->raise();
        ipConfigButton->raise();
        rememberCheckBox->raise();
        cancelButton->raise();
        IPListComboBox->raise();
        autoLoginCheckbox->raise();
        userNameLineEdit->raise();
        realmComboBox->raise();
        passwordLineEdit->raise();
        deviceConfigButton->raise();
        toolButton->raise();
        weblibConfigButton->raise();
        loginButton->raise();

        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        LoginForm->setWindowTitle(QApplication::translate("LoginForm", "CoolView\347\231\273\345\275\225", 0));
        ipConfigButton->setText(QApplication::translate("LoginForm", "\347\275\221\347\273\234\351\205\215\347\275\256", 0));
        weblibConfigButton->setText(QApplication::translate("LoginForm", "Weblib\350\256\276\347\275\256", 0));
        deviceConfigButton->setText(QApplication::translate("LoginForm", "\350\256\276\345\244\207\350\256\276\347\275\256", 0));
        cancelButton->setText(QApplication::translate("LoginForm", "\345\205\263\346\234\272", 0));
        toolButton->setText(QApplication::translate("LoginForm", "\350\256\276\347\275\256", 0));
        loginButton->setText(QString());
        autoLoginCheckbox->setText(QString());
        rememberCheckBox->setText(QString());
        warningLabel->setText(QApplication::translate("LoginForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0));
        userNameLineEdit->setText(QString());
        realmComboBox->clear();
        realmComboBox->insertItems(0, QStringList()
         << QApplication::translate("LoginForm", "sip.ccnl.scut.edu.cn", 0)
         << QApplication::translate("LoginForm", "sip2.ccnl.scut.edu.cn", 0)
        );
        passwordLineEdit->setText(QString());
        refreshIPListButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_FORM_H
