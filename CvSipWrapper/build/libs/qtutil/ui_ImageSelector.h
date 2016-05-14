/********************************************************************************
** Form generated from reading ui file 'ImageSelector.ui'
**
** Created: Mon Apr 20 22:15:59 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IMAGESELECTOR_H
#define UI_IMAGESELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include "qtutil/ThumbnailView.h"

QT_BEGIN_NAMESPACE

class Ui_ImageSelector
{
public:
    QGridLayout *gridLayout;
    ThumbnailView *thumbnailListView;
    QListView *startDirListView;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QHBoxLayout *hboxLayout1;
    QPushButton *goUpButton;
    QComboBox *dirComboBox;
    QSpacerItem *spacerItem1;
    QPushButton *refreshButton;

    void setupUi(QDialog *ImageSelector)
    {
    if (ImageSelector->objectName().isEmpty())
        ImageSelector->setObjectName(QString::fromUtf8("ImageSelector"));
    ImageSelector->resize(582, 340);
    ImageSelector->setSizeGripEnabled(true);
    gridLayout = new QGridLayout(ImageSelector);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    thumbnailListView = new ThumbnailView(ImageSelector);
    thumbnailListView->setObjectName(QString::fromUtf8("thumbnailListView"));
    thumbnailListView->setMovement(QListView::Static);
    thumbnailListView->setFlow(QListView::LeftToRight);
    thumbnailListView->setProperty("isWrapping", QVariant(true));
    thumbnailListView->setResizeMode(QListView::Fixed);
    thumbnailListView->setViewMode(QListView::IconMode);

    gridLayout->addWidget(thumbnailListView, 1, 1, 1, 1);

    startDirListView = new QListView(ImageSelector);
    startDirListView->setObjectName(QString::fromUtf8("startDirListView"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(startDirListView->sizePolicy().hasHeightForWidth());
    startDirListView->setSizePolicy(sizePolicy);
    startDirListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    startDirListView->setIconSize(QSize(32, 32));
    startDirListView->setFlow(QListView::TopToBottom);
    startDirListView->setViewMode(QListView::ListMode);

    gridLayout->addWidget(startDirListView, 1, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(ImageSelector);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(ImageSelector);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);


    gridLayout->addLayout(hboxLayout, 2, 0, 1, 2);

    hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout1->setSpacing(6);
#endif
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    goUpButton = new QPushButton(ImageSelector);
    goUpButton->setObjectName(QString::fromUtf8("goUpButton"));
    goUpButton->setAutoDefault(false);

    hboxLayout1->addWidget(goUpButton);

    dirComboBox = new QComboBox(ImageSelector);
    dirComboBox->setObjectName(QString::fromUtf8("dirComboBox"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(dirComboBox->sizePolicy().hasHeightForWidth());
    dirComboBox->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(dirComboBox);

    spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    refreshButton = new QPushButton(ImageSelector);
    refreshButton->setObjectName(QString::fromUtf8("refreshButton"));

    hboxLayout1->addWidget(refreshButton);


    gridLayout->addLayout(hboxLayout1, 0, 1, 1, 1);


    retranslateUi(ImageSelector);
    QObject::connect(okButton, SIGNAL(clicked()), ImageSelector, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), ImageSelector, SLOT(reject()));
    QObject::connect(thumbnailListView, SIGNAL(doubleClicked(QModelIndex)), okButton, SLOT(animateClick()));

    QMetaObject::connectSlotsByName(ImageSelector);
    } // setupUi

    void retranslateUi(QDialog *ImageSelector)
    {
    okButton->setText(QApplication::translate("ImageSelector", "OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("ImageSelector", "Cancel", 0, QApplication::UnicodeUTF8));
    goUpButton->setText(QApplication::translate("ImageSelector", "Go Up", 0, QApplication::UnicodeUTF8));
    refreshButton->setText(QApplication::translate("ImageSelector", "Refresh", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ImageSelector);
    } // retranslateUi

};

namespace Ui {
    class ImageSelector: public Ui_ImageSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESELECTOR_H
