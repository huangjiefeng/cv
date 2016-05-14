/********************************************************************************
** Form generated from reading UI file 'db_browser_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DB_BROWSER_WIDGET_H
#define UI_DB_BROWSER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBBrowserWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *leftLayout;
    QTreeView *conferenceView;
    QHBoxLayout *confBottomLayout;
    QPushButton *conferenceRefreshButton;
    QVBoxLayout *rightLayout;
    QTreeView *episodeView;
    QTableView *fileView;

    void setupUi(QWidget *DBBrowserWidget)
    {
        if (DBBrowserWidget->objectName().isEmpty())
            DBBrowserWidget->setObjectName(QStringLiteral("DBBrowserWidget"));
        DBBrowserWidget->resize(829, 445);
        horizontalLayout = new QHBoxLayout(DBBrowserWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        leftLayout = new QVBoxLayout();
        leftLayout->setSpacing(6);
        leftLayout->setObjectName(QStringLiteral("leftLayout"));
        leftLayout->setSizeConstraint(QLayout::SetMaximumSize);
        conferenceView = new QTreeView(DBBrowserWidget);
        conferenceView->setObjectName(QStringLiteral("conferenceView"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(conferenceView->sizePolicy().hasHeightForWidth());
        conferenceView->setSizePolicy(sizePolicy);
        conferenceView->setMaximumSize(QSize(200, 16777215));

        leftLayout->addWidget(conferenceView);

        confBottomLayout = new QHBoxLayout();
        confBottomLayout->setSpacing(6);
        confBottomLayout->setObjectName(QStringLiteral("confBottomLayout"));
        conferenceRefreshButton = new QPushButton(DBBrowserWidget);
        conferenceRefreshButton->setObjectName(QStringLiteral("conferenceRefreshButton"));

        confBottomLayout->addWidget(conferenceRefreshButton);


        leftLayout->addLayout(confBottomLayout);


        horizontalLayout->addLayout(leftLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(6);
        rightLayout->setObjectName(QStringLiteral("rightLayout"));
        episodeView = new QTreeView(DBBrowserWidget);
        episodeView->setObjectName(QStringLiteral("episodeView"));

        rightLayout->addWidget(episodeView);

        fileView = new QTableView(DBBrowserWidget);
        fileView->setObjectName(QStringLiteral("fileView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileView->sizePolicy().hasHeightForWidth());
        fileView->setSizePolicy(sizePolicy1);
        fileView->setMaximumSize(QSize(16777215, 120));

        rightLayout->addWidget(fileView);


        horizontalLayout->addLayout(rightLayout);


        retranslateUi(DBBrowserWidget);

        QMetaObject::connectSlotsByName(DBBrowserWidget);
    } // setupUi

    void retranslateUi(QWidget *DBBrowserWidget)
    {
        DBBrowserWidget->setWindowTitle(QApplication::translate("DBBrowserWidget", "DBBrowserWidget", 0));
        conferenceRefreshButton->setText(QApplication::translate("DBBrowserWidget", "\345\210\267\346\226\260", 0));
    } // retranslateUi

};

namespace Ui {
    class DBBrowserWidget: public Ui_DBBrowserWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DB_BROWSER_WIDGET_H
